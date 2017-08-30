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
// Function Delcarations for builtin shell commands:
//
int LshCd(char **args);
int LshHelp(char **args);
int LshExit(char **args);
//
// List of builtin commands, followed by their corresponding functions.
//
char *builtin_str[] = {"cd", "help", "exit"};
int (*builtin_func[]) (char **) = {&LshCd, &LshHelp, &LshExit};
int LshNumBuiltins() {
  return sizeof(builtin_str) / sizeof(char *);
}
//
//Builtin function implementations.
//
//
// Description: Builtin command: change directory.
// Parameter: args List of args. args[0] is "cd". args [1] is the directory.
// Return: Always returns 1, to continue executing
//
int LshCd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}
//
// Description: Builtin command: print help
// Parameter: args List of args. Not examined.
// Return: Always returns to 1, to continue executing.
//
int LshHelp(char **args) {
  int i;
  printf("Ryan Fleming's LSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");
  for (i = 0; i < LshNumBuiltins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
  printf("Use the man command for information on other programs.\n");
  return 1;
}
//
// Description: Builtin command: exit.
// Parameter: args List of args. Not examined.
// Return: Always returns 0, to terminate execution
//
int LshExit(char **args) {
  return 0;
}
//
// Description: Launch a program and wait for it to terminate.
// Paramter: args Null terminated list of arguments (including program).
// Return: Always returns to 1, to continue execution.
//
int LshLaunch(char **args) {
  pid_t pid;
  int status;
  pid = fork();
  if (pid == 0) {
    //Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    //Error forking
    perror("lsh");
  } else {
    //Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}
//
// Description: Execute shell built-in or launch program.
// Parameter: args Null terminated list of arguments.
// Return: 1 if the shell should continue running, 0 if it should terminate
//
int LshExecute(char **args) {
  int i;
  if (args[0] == NULL) {
    //An empty command was entered.
    return 1;
  }
  for (i = 0; i < LshNumBuiltins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return LshLaunch(args);
}
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
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
//
// Description: Split a line into tokens.
// Parameter: line The line.
// Return: Null-terminated array of tokens.
//
char **LshSplitLine(char *line) {
  int bufsize = LSH_TOK_BUFSIZE;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *single_token;
  char **tokens_backup; 
  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }
  single_token = strtok(line, LSH_TOK_DELIM);
  while (single_token != NULL) {
    tokens[position] = single_token;
    position++;
    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    single_token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
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
//
int main(int argc, char **argv) {
  //Load config files, if any.
  //Run command loop.
  LshLoop();
  //Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}
