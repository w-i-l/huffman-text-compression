#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "Headers/util.h"
#include "Headers/code.h"

// adds to symbols array the missing nodes
// to complete the huffman heap
void complete_nodes(data* symbols, int* no){

    // current index of symbols
    int i = *no;
    // the smallest items in array
    int j=0,k=1;


    while(j < 2*(*no)-1 && k < 2*(*no)-1 ){

        // symbols[i] is the new node that combines
        // the smallest probability nodes
        // calculating the probability
        symbols[i].probability = symbols[j].probability + symbols[k].probability;

        //combines the two symbols of the nodes
        strcpy(symbols[i].word,symbols[j].word);
        strcat(symbols[i].word,symbols[k].word);

        i++;
        // rearrange the array
        sort(symbols,i);

        j+=2;
        k+=2;
    }

    // update the array item's number
    *no = i;

}



// encode the symbol based on its position in the heap
void encode(data* symbols, int no, int reverse){

    // to change the encoded version from standard(i.e. left = 0, right = 1)
    char left[2], right[2];
    strcpy(left,(reverse == 0 ? "1" : "0"));
    strcpy(right, reverse == 0 ? "0" : "1");

    for(int i=1; i<no; i++){

        // if it is the first two nodes we manually allocate
        if(i==1 || i == 2){
            strcat(symbols[i].encode, i%2==0 ? left: right);
            symbols[i].encode_length = 1;
        }
        else{
            // we search for its parent node and use its encoded string
            strcat(symbols[i].encode,find(symbols,no,symbols[i].word,i-1));
            // than add the corresponding code
            strcat(symbols[i].encode, i%2==0 ? left : right);

            symbols[i].encode_length = strlen(symbols[i].encode);
        }
    }
}


// check if symbol_to_check is already declared in symbols
// if yes return its index else 0
int data_in(data* symbols, int no, char symbol_to_check[2]){
    for(int i=0; i<no; i++){
        if(strcmp(symbols[i].word,symbol_to_check) == 0){
            return i;
        }
    }

    // if it is the first position
    return -1;
}


// search for a symbol in symbols array
// if it find it return the symbol
// else NULL
char* decode_symbol(data* symbols, int no, char* encoded){
    for(int i=0; i<no; i++){
        if(symbols[i].original == 1 && strcmp(symbols[i].encode,encoded) == 0){
            return symbols[i].word;
        }
    }
    // char* result =  malloc(2*sizeof(char));
    // strcpy(result,"@");
    return NULL;
}
