/* Copyright 2015 Peter Goodman, all rights reserved. */

#include <set>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "radamsa.h"

static struct X {
  // Input/output to Radamsa.
  const std::string *input;
  std::string output;

  // Where are we in the process of reading `input`?
  size_t input_index;

  // Set of objects that remain unfreed by Radamsa.
  std::set<void *> allocations;

} gRadamsa;

// Radamsa's `main` function.
//extern "C" void *boot(int argc, const char *argv[]);
#include <stdio.h>
#include <cstring>
// Pretend to do a `read` system call from within Radamsa.
extern "C" ssize_t radamsa_read(int, uint8_t *buff, size_t buff_size) {
  auto count = std::min(buff_size, gRadamsa.input->length() - gRadamsa.input_index);

  std::memcpy(buff, gRadamsa.input->data() + gRadamsa.input_index, count);
  gRadamsa.input_index += count;
  return static_cast<ssize_t>(count);
}

// Pretend to do a `write` system call from within Radamsa.
extern "C" ssize_t radamsa_write(int, uint8_t *buff, size_t buff_size) {
  gRadamsa.output.insert(gRadamsa.output.end(), buff, buff + buff_size);
  return static_cast<ssize_t>(buff_size);
}

// Interposes on `malloc`s performed by Radamsa.
void *radamsa_malloc(size_t size) {
  auto ptr = malloc(size);
  gRadamsa.allocations.insert(ptr);
  return ptr;
}

// Interposes on `free`s performed by Radamsa.
void radamsa_free(void *ptr) {
  gRadamsa.allocations.erase(ptr);
  free(ptr);
}

// Interposes on `realloc`s performed by Radamsa. The OWL scheme compiler uses
// `realloc` for heap allocation.
void *radamsa_realloc(void *ptr, size_t size) {
  gRadamsa.allocations.erase(ptr);
  ptr = realloc(ptr, size);
  gRadamsa.allocations.insert(ptr);
  return ptr;
}

static char gRadamsaSeed[32] = {'\0'};

extern "C" char * Mutate(char *str) {

  printf("input: %s\n", str);
  std::string input = str;
  gRadamsa.input = &input;
  gRadamsa.input_index = 0;
  gRadamsa.output.clear();
  gRadamsa.output.reserve(input.size());

  sprintf(gRadamsaSeed, "%ld", rand());

  char *args[] = {
    (char *)"radamsa",
    (char *)"--seed",
    gRadamsaSeed,
    nullptr
  };

  // I am not sure if this is even valid, due to the Radamsa heap being a
  // static char array.
  boot(3, args);

  for (auto ptr : gRadamsa.allocations) free(ptr);

  gRadamsa.input = nullptr;
  gRadamsa.allocations.clear();

  std::string output;
  output.swap(gRadamsa.output);
  printf("output %s\n", output.c_str());
  return strdup(output.c_str());
}

int main() {
  char *res = Mutate((char *)"some_unique_string");
  printf("%s", res);
  return 1;
}

