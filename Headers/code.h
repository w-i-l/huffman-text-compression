#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED

void encode(data* symbols, int no, int reverse);
void complete_nodes(data* symbols, int* no);
char* decode_symbol(data* symbols, int no, char* encoded);
int data_in(data* symbols, int no, char symbol_to_check[2]);

#endif //CODE_H_INCLUDED