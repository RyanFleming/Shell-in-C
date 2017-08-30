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
//
// Description Loop getting input and executing it.
//
void LshLoop(void) {
  char *line;
  char **args;
  int status;
  do {
    printf("> ");
    line = LshReadLine();
    args = LshSplitLine(line);
    status = LshExecute(args);
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