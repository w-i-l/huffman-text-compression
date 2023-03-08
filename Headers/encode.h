#ifndef ENCODE_H_INCLUDE
#define ENCODE_H_INCLUDE


void write_dictionary(const char* filename, data* symbols, int no);
void encode_file(const char* filename, const char* out_file, data* symbols, int no);
data* read_dictionary(const char* in_file, int* no, long* seek_pos);
void decode_file(const char* filename, const char* out_file, data* symbols, int no, long seek_pos);

#endif //ENCODE_H_INCLUDE