
#include <string>
typedef uintptr_t word;
void *radamsa_realloc(void *ptr, size_t size);
void *radamsa_malloc(size_t size);
void radamsa_free(void *ptr);
extern "C" int Mutate(char *str, char *out_str);
word boot(int nargs, char **argv);
