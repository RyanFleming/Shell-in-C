//
// Author: Ryan Fleming
// Date: 8/30/17
// Description: Simple implementation of  shell in C.
// Demonstrates the basics of how a shell works including:
// read, parse, fork, exec, and wait.
//
int main(int argc, char **argv) {
  //Load config files, if any.
  //Run command loop.
  LshLoop();
  //Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}