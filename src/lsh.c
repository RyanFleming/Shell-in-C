//
// Author: Ryan Fleming
// Date: 8/30/17
// Description: Simple implementation of  shell in C.
// Demonstrates the basics of how a shell works including:
// read, parse, fork, exec, and wait.
//
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LSH_RL_BUFSIZE 1024
//
// Description: Read a line of input from stdin
// Return: The line from stdin
//
char *LshReadLine(void) {
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int character;
  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }
  while (1) {
    //Read a character
    character = getchar();
    if (character == EOF) {
      exit(EXIT_SUCCESS);
    } else if (character == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = character;
    }
    position++;
    //If we have exceeded the buffer, reallocate
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}
//
// Description: Loop getting input and executing it.
//
void LshLoop(void) {
  char *line;
  char **args;
  int status;
  do {
    printf("> ");
    line = LshReadLine();
//    args = LshSplitLine(line);
//    status = LshExecute(args);
    free(line);
    free(args);
  } while(status);
}
//
// Description: Main entry point.
// Parameter: agrc Argument count
// Parameter: argv Argument vector
// Return: status code
int main(int argc, char **argv) {
  //Load config files, if any.
  //Run command loop.
  LshLoop();
  //Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}
