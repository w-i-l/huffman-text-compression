#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "Headers/util.h"


// sort the symbols array based on probability
// sorted is the flag for optimize
// bubble sort
void sort(data* symbols, int no){

    for(int i=0; i<no; i++){
        int sorted = 1;
        for(int j=i; j<no; j++){
            if(symbols[i].probability > symbols[j].probability
                || (symbols[i].probability == symbols[j].probability && symbols[i].word[0] < symbols[j].word[0] )){
                data aux = symbols[i];
                symbols[i] = symbols[j];
                symbols[j] = aux;
                sorted = 0;
            }
        }
        if(sorted){
            return;
        }
    }
}

// reverse the symbols array
void reverse(data* symbols, int no){
    for(int i=0; i<no/2; i++){
        data aux = symbols[i];
        symbols[i] = symbols[no-1-i];
        symbols[no-1-i] = aux;
    }
}

// find the last data struct which word contains
// the to_find string
// returns its encoding string
const char* find(data* symbols, int no,const char* to_find, int index){
    for(int i=index; i>0; i--){
        if(strstr(symbols[i].word,to_find) != NULL){
            return symbols[i].encode;
        }
    }
}

// free the memory and erase the content
void empty_symbols(data* symbols, int* no){
    for(int i=0; i<*no; i++){
        strcpy(symbols[i].encode, "");
        strcpy(symbols[i].word, "");
        symbols[i].original = 0;
        symbols[i].encode_length = 0;
    }

    free(symbols);
    *no = 0;
}


// display all the symbols along with
// their encoded strings
void display_encoded(data* symbols, int no, int only_original){
    // the .original attribute means if the symbol
    // was read from the user
    // 1 means read from the user (i.e. 'A', 'B')
    // 0 if it was created by program (i.e. "AB")

    for(int i=0; i<79; i++)
        printf("%c",'-');

    printf("\n");

    if(only_original){
        for(int i=0; i<no; i++){
            if(symbols[i].original){
                printf("|%3d. | %2s | encode: %20s | encode-length: %2d | prob: %f|\n",i, symbols[i].word,symbols[i].encode,symbols[i].encode_length,symbols[i].probability);
                
                for(int i=0; i<79; i++)
                    printf("%c",'-');
                
                printf("\n");
            }
        }
    }
    else{
        for(int i=0; i<no; i++){
            printf("|%3d. | %20s | encode: %20s | encode-length: %2d | prob: %f|\n",i, symbols[i].word,symbols[i].encode,symbols[i].encode_length,symbols[i].probability);
        }
    }
}


// calculate and display the shannon entropy
// and current entropy for our encoding
void display_entropy(data* symbols, int no){
    float shanon_entropy = 0, current_entropy = 0;

    for(int i=0; i<no; i++){
        if(symbols[i].original){
            shanon_entropy -= symbols[i].probability * log2f(symbols[i].probability);
            current_entropy += symbols[i].probability * (float)strlen(symbols[i].encode);
        }
    }
    printf("Shanon Entropy: %f\nCurrent Entropy: %f\n",shanon_entropy,current_entropy);
}