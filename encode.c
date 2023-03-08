#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "Headers/util.h"
#include "Headers/code.h"
#include "Headers/encode.h"


#define BUFFER_TYPE int


// return 0 if the word was written entirely
// return -1 if the word can't be written
// return the number of chars left if the word was written partially
int add_to_buffer(BUFFER_TYPE* buffer, BUFFER_TYPE* buffer_size, char* to_encode){


    int encode_size = strlen(to_encode);

    // the space left to wite in buffer
    int remaining_space = (int)(sizeof(*buffer)*8) - *buffer_size;
    
    // if we can't add to buffer
    if(remaining_space == 0){
        return -1;
    }
    // pointer for parsing the encode
    char* copy = to_encode;

    // if we can entirely write current encoded word
    if(remaining_space >= encode_size){

        // parsing the word till null char
        while(*copy != 0){
            // we shift to left to create space for a new bit
            *buffer <<= 1;
            // add the bit
            *buffer |= *copy == '1' ? 1: 0;
            // next char
            copy++;
        }

        // update the value of the buffer_size
        // substracting the original pointer
        // from the current one
        *buffer_size += (int)(copy - to_encode);

        return 0;
    }
    // if we can partially write current encoded word
    else{

        // how many space we have left to write
        int left_space = remaining_space;

        while(left_space){
            // we shift to left to create space for a new bit
            *buffer <<= 1;
            // add the bit
            *buffer |= *copy == '1' ? 1: 0;
            // next char
            copy++;

            left_space--;
        }

        // update the value of buffer_size
        // it would be sizeof(buffer)*8
        *buffer_size += remaining_space - left_space;

        return encode_size - remaining_space;
    }
}


void read_from_buffer(BUFFER_TYPE buffer_read, BUFFER_TYPE* buffer_read_index, char* buffer_write, int* buffer_write_index){

    // get the bit from the buffer_read_index
    int bit = buffer_read & (1 << *buffer_read_index);

    buffer_write[*buffer_write_index] = bit == 0 ? '0' : '1';
    // adding the null char
    // it will be overwrited if another char will be appended
    buffer_write[++(*buffer_write_index)] = 0;

    // index value update 
    --(*buffer_read_index);

}


// write the dictionary to filename
// it appends a separator at the end
void write_dictionary(const char* filename, data* symbols, int no){

    FILE* f = fopen(filename,"wb");

     if(f == NULL){
        printf("Can't oppen the file %s\n",filename);
        exit(-1);
    }

    // write the number of nodes
    fwrite(&no, sizeof(no), 1, f);

    // write all symbols with their probabilities
    for(int i=0; i<no; i++){
        if(symbols[i].original == 1){
            // symbol
            fwrite(symbols[i].word, (strlen(symbols[i].word) + 1) * sizeof(char), 1, f);
            // probability
            fwrite(&symbols[i].probability, sizeof(symbols[i].probability), 1, f);
        }
    }
    
    // we use to make distinction between dictionary
    // and the encoded data
    // here we use the maximum number of zeros plus one
    // we will write later the shift amount
    int separator =  0;
    
    // if(separator == NULL){
    //     printf("Problem with memory allocation\n");
    //     exit(-1);
    // }

    // space for shift amount
    //printf("writing separator at: %d\n", ftell(f));
    fwrite(&separator, sizeof(int), 1, f);

    
    // free(separator);
    fclose(f);

}


// encode every character from the filename
// and write it to out_file
void encode_file(const char* filename, const char* out_file, data* symbols, int no){

    FILE* in = fopen(filename,"r");
    FILE* out = fopen(out_file,"ab");

    if(out == NULL){
        printf("Can't oppen the file %s\n",out_file);
        exit(-1);
    }

    if(in == NULL){
        printf("Can't oppen the file %s\n",filename);
        exit(-1);
    }

    char current_char[2];

    // we use a int as buffer
    // buffer_size represent the number of bits written
    BUFFER_TYPE buffer = 0;
    BUFFER_TYPE buffer_size = 0;

    int shift_index = 0;
    

    // write the encode for every character
    while(fgets(current_char,2,in) != NULL){

        // get the encode for the current char
        int index = data_in(symbols, no, current_char);

        // append to buffer
        int result = add_to_buffer(&buffer, &buffer_size, symbols[index].encode);

        // if an error occured and the buffer was full
        if(result == -1){
            printf("Buffer wasn't empty!\n");
            exit(-1);
        }
        else{
            // if the encode was added partially
            if(result > 0){

                // we write to file
                fwrite(&buffer, sizeof(buffer), 1, out);


                //empy the buffer
                buffer = 0;
                buffer_size = 0;

                // write the remaining bytes
                add_to_buffer(&buffer, &buffer_size, symbols[index].encode + (symbols[index].encode_length - result));
            }
        }

        // if the buffer is full
        if( buffer_size == (int)(sizeof(buffer)*8)){

            //we write to file
            fwrite(&buffer, sizeof(buffer), 1, out);

            //empy the buffer
            buffer = 0;
            buffer_size = 0;
        }
    }


    // check if the buffer is empty after the end of while
    if(buffer_size){

        int shift_amount = (int)(sizeof(buffer) * 8) - buffer_size;

        // we shift the content to avoid having leading 0s
        // i.e 000...0101 should be written as 101000...00
        buffer <<= (int)(sizeof(buffer) * 8) - buffer_size;

        fwrite(&buffer, sizeof(buffer), 1, out);

        // write to empty space
    }
    
    fclose(in);
    fclose(out);

}


// allocate a new dictionary and read it
// from in_file, then return it
data* read_dictionary(const char* in_file, int* no, long* seek_pos){

    FILE* in = fopen(in_file,"rb");

     if(in == NULL){
        printf("Can't oppen the file %s\n",in_file);
        exit(-1);
    }

    // read how many items should the heap have
    fread(no, sizeof(int), 1, in);

    data* symbols = (data*)malloc(*no * sizeof(data));

    if(symbols == NULL){
        printf("Problem with memory allocation\n");
        exit(-1);
    }

    // now the no holds how many items
    // should be read
    *no = (*no + 1) / 2;

    // symbols_index - current index in dict
    // buffer_index - current buffer in dict
    // is_word - to check if we have to read a symbol or its probability
    int symbols_index = 0, is_word = 1;

    char current_char[3];

    float current_probability = 0;

    while(symbols_index < *no && fgets(current_char,2,in) != NULL){
        
        // to read the current symbol
        if(is_word == 1){
            
            strcpy(symbols[symbols_index].word, current_char);
            is_word = 0;
        }
        // we have read the symbol now we save its probability
        else{

            fread(&current_probability, sizeof(current_probability), 1, in);

            symbols[symbols_index].probability = current_probability;
            symbols[symbols_index].original = 1;
            is_word = 1;
            symbols_index ++;
        }
    }

    // return the end position of the dictionary
    *seek_pos = ftell(in);
    //printf(" readind dict: %d\n", *seek_pos);

    fclose(in);

    return symbols;

}



// decode every character from the filename
// and write it to out_file
void decode_file(const char* filename, const char* out_file, data* symbols, int no, long seek_pos){
    
    FILE* in = fopen(filename,"rb");
    FILE* out = fopen(out_file,"w+");

    if(in == NULL){
        printf("Can't oppen the file %s\n",filename);
        exit(-1);
    }

    if(out == NULL){
        printf("Can't oppen the file %s\n",out_file);
        exit(-1);
    }

    int shift_amount = 0;

    // goes to the end of dictionary plus
    // the separator length
    // fseek(in,seek_pos+symbols[no-1].encode_length+1,SEEK_SET);
    fseek(in,seek_pos,SEEK_SET);
    fread(&shift_amount, sizeof(int), 1, in);

    // buffer_file_index store the MSB index written
    // i.e. 0000110100 here the index is 6 after writting
    BUFFER_TYPE buffer_file = 0;
    BUFFER_TYPE buffer_file_index = (sizeof(buffer_file) * 8) - 1;

    char* buffer = malloc(MAX_CHARS * sizeof(char));

    if(buffer == NULL){
        printf("Problem with memory allocation!\n");
        exit(-1);
    }

    // read char
    char current_char;
    int buffer_index = 0;

    char* result = NULL;

    // we read until the EOF
    while(fread(&buffer_file,sizeof(buffer_file),1,in) == 1 ){

        // reset the index
        buffer_file_index = (sizeof(buffer_file) * 8) - 1;

        result = NULL;

        while(buffer_file_index != -1){

            // looping until a symbol is found
            // or no more bits to read
            while(buffer_file_index != -1 && result == NULL){

                result = decode_symbol(symbols, no, buffer);
                
                // if we have a valid symbol
                if(result != NULL){
                    
                    fprintf(out, "%s", result);

                    // reset
                    buffer_index = 0;
                    result = NULL;
                }

                // read one bit
                read_from_buffer(buffer_file, &buffer_file_index, buffer, &buffer_index);

                result = decode_symbol(symbols, no, buffer);

            }

            if(buffer_file_index != -1){
                // reset the symbol buffer
                buffer_index = 0;
                result = NULL;
            }
        }
    }

    
    // last word


    fclose(in);
    fclose(out);

}
