#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

// how many different chars we expect to find
// in a file(i.e. a-z 26, A-Z 26, punctuation > 10)
#define MAX_DIFFERENT_CHARS 70

#define MAX_CHARS 1000
typedef struct{
    char word[MAX_CHARS];
    char encode[MAX_CHARS];
    int encode_length;
    int original;
    float probability;
}data;


void sort(data* symbols, int no);
void reverse(data* symbols, int no);
const char* find(data* symbols, int no,const char* to_find, int index);
void empty_symbols(data* symbols, int* no);
void display_encoded(data* symbols, int no, int only_original);
void display_entropy(data* symbols, int no);


#endif // UTIL_H_INCLUDED
