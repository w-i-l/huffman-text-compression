#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Headers/util.h"
#include "Headers/code.h"
#include "Headers/encode.h"
#include "Headers/menu.h"
#include "Headers/test.h"



// return a data array with all the different
// symbols found in the file given
// it also computes the apparition probability
data* read_data(const char* filename, int* no){
    
    FILE* f = fopen(filename,"r");

    if(f == NULL){
        printf("Can't oppen the file %s\n",filename);
        exit(-1);
    }

    // alocate the array
    data* symbols = (data*)malloc((2*MAX_DIFFERENT_CHARS -1)* sizeof(data));

    // symbols's index
    *no = 0;

    if(symbols == NULL){
        printf("Can't allocate memory\n");
        exit(-1);
    }

    // how many chars the file contains
    int char_number = 0;
    char current_char[2];

    while(fgets(current_char,2,f) != NULL){

        // store the current_char index in symbols
        int current_char_index = data_in(symbols,*no,current_char);

        // if the current_char is not in the array
        if( current_char_index == -1){
            strcpy(symbols[*no].word, current_char);
            // store that the value is a leaf or
            // a final symbol
            symbols[*no].original = 1;
            // store how many times the symbol appears
            symbols[*no].probability = 1;
            (*no)++;
        }
        // if the word is in the array
        else{
            symbols[current_char_index].probability += 1;
        }

        char_number++;
    }

    if (!feof(f))
        printf("Something went wrong. \n");
       

    // update the probability with the correct value
    for(int i=0; i<*no; i++){
        symbols[i].probability = symbols[i].probability / char_number;
    }

    fclose(f);
    return symbols;
    
}

void menu(){
    data* symbols = NULL;
    int no = 0;
    long seek_pos=0;
    char option = 0;
    char in[30], out[30];
    clock_t start, end, diff;
    // option = getc(stdin);
    
    while(option != 'q'){
        
        

        printf("\n\n%10s\n","MENU");
        printf("%s\n","Choose an option:");
        printf("%s\n","1. Read data from a file");
        printf("%s\n","2. Encode data");
        printf("%s\n","3. Write data to a file");
        printf("%s\n","4. Decode data from a file");
        printf("%s\n","5. Display data");
        printf("%s\n","6. Display entropy");
        printf("%s\n","7. Test the files");
        printf("%s\n","Press q for exit");

        
        option = getc(stdin);

        system("cls");

        switch(option){

            case '1':
                printf("Please enter the filename: ");
                scanf("%s",in);

                start = clock();

                // if was another array read
                empty_symbols(symbols, &no);

                symbols = read_data(in,&no);

                end = clock();
                diff = (double)(end - start) / CLOCKS_PER_SEC;
                printf("\nData Read\n");
                printf("Estimated time to process %d min %d sec\n", (int)diff / 60, (int)diff % 60);
                break;

            case '2':
                if(no == 0){
                    printf("You have no data read!");
                }
                else{
                    sort(symbols,no);

                    complete_nodes(symbols,&no);

                    reverse(symbols,no);
                    encode(symbols,no,0);
                    
                    sort(symbols,no);
                    reverse(symbols,no);

                    printf("\nData Encoded\n");
                }
                break;

            case '3':
                if(no == 0){
                    printf("You have no data read!");
                }
                else{
                    printf("\nPlease enter the input filename: ");
                    scanf("%s",in);

                    printf("\nPlease enter the output filename: ");
                    scanf("%s",out);

                    start = clock();

                    write_dictionary(out, symbols, no);
                    encode_file(in, out, symbols, no);

                    end = clock();
                    diff = (double)(end - start) / CLOCKS_PER_SEC;
                    printf("\nFile Encoded\n");
                    printf("Estimated time to process %d min %d sec\n", (int)diff / 60, (int)diff % 60);
                }
                break;

            case '4':
                printf("\nPlease enter the input filename: ");
                scanf("%s",in);

                printf("\nPlease enter the output filename: ");
                scanf("%s",out);

                // if was another array read
                empty_symbols(symbols, &no);

                start = clock();
                symbols = read_dictionary(in, &no, &seek_pos);

                sort(symbols, no);
                complete_nodes(symbols, &no);

                reverse(symbols, no);
                encode(symbols, no, 0);
                
                decode_file(in, out, symbols, no, seek_pos);
                
                end = clock();
                diff = (double)(end - start) / CLOCKS_PER_SEC;
                printf("\nFile Decoded\n");
                printf("Estimated time to process %d min %d sec\n", (int)diff / 60, (int)diff % 60);
                break;

            case '5':
                if(no == 0){
                        printf("You have no data read!");
                    }
                else{
                    int only_original;
                    printf("You want to display only symbols?: ");
                    scanf("%d",&only_original);

                    display_encoded(symbols, no, only_original);

                    printf("\nData Displayed\n");
                }
                break;

            case '6':
                if(no == 0){
                        printf("You have no data read!");
                    }
                else{
                    display_entropy(symbols,no);

                    printf("Entropy Displayed\n");
                }
                break;

            case '7':
                printf("\nPlease enter the input filename: ");
                scanf("%s",in);

                printf("\nPlease enter the output filename: ");
                scanf("%s",out);

                test(in, out);

                printf("\nFile Tested\n");
                break;

            case 'q':
                exit(0);

            default:
                printf("Enter a valid char!\n");
                break;
                
        }

        getc(stdin);

    }

    free(symbols);
}
