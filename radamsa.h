
#include <string>
typedef uintptr_t word;
void *radamsa_realloc(void *ptr, size_t size);
void *radamsa_malloc(size_t size);
void radamsa_free(void *ptr);
extern "C" char* Mutate(char *str);
word boot(int nargs, char **argv);
