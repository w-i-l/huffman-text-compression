#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "Headers/util.h"
#include "Headers/code.h"
#include "Headers/encode.h"
#include "Headers/test.h"


void test(const char* in_file, const char* out_file){

    FILE *in, *out;

    in = fopen(in_file,"r");
    out = fopen(out_file,"r");

    if(in == NULL){
        printf("Can't oppen the file %s\n",in_file);
        exit(-1);
    }

    if(out == NULL){
        printf("Can't oppen the file %s\n",out_file);
        exit(-1);
    }

    char in_current = 0, out_current = 0;

    in_current = getc(in);
    out_current = getc(out);

    while(in_current != EOF && out_current != EOF){
        
        if(in_current != out_current){
            printf("Char %c from %s is different from char %c from %s!\n", in_current, in_file, out_current, out_file);
            return;
        }
        in_current = getc(in);
        out_current = getc(out);
    }
    // test to see if both files were read
    if(in_current != -1 || out_current != -1){
        printf("Different content!\n");
        printf("Char %d from %s is different from char %d from %s!\n", in_current, in_file, out_current, out_file);
        return;
    }
    printf("Same content!\n");
}