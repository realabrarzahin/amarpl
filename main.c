#include "bytecodes.h"
#include "common.h"
#include "debug.h"
#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl() {

  char buffer[4096];

  while (true) {
    printf("~> ");

    if (!fgets(buffer, sizeof(buffer), stdin)) {
      printf("\n");
      break;
    }

    interpret(buffer);
  }
}

static char *read_file(const char *path) {

  FILE *file = fopen(path, "rb");

  if (file == NULL) {
    printf("Couldn't open file at '%s'\n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *buffer = malloc(file_size + 1);

  if (buffer == NULL) {
    printf("Not enough memory to read '%s'\n", path);
    exit(74);
  }

  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

  if (bytes_read < file_size) {
    printf("Couldn't read file at '%s'\n", path);
    exit(74);
  }

  buffer[bytes_read] = '\0';

  fclose(file);
  return buffer;
}

static void run_file(const char *path) {

  char *source = read_file(path);
  InterpretResult result = interpret(source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, const char *argv[]) {

  init_vm();

  if (argc == 1) {
    repl();
  }

  else if (argc == 2) {
    run_file(argv[1]);
  }

  else {
    fprintf(stderr, "Usage: amarpl [path]\n");
    exit(64);
  }

  free_vm();

  return 0;
}
