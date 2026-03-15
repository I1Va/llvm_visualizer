#include <iostream>
#include <cstdint>

extern "C" void basic_block_start_logger(long int id) {
  printf("[LOG] start basic block '%ld'\n", id);
}

void callLogger(char *callerName, char *calleeName, long int valID) {
  printf("[LOG] CALL '%s' -> '%s' {%ld}\n", callerName, calleeName, valID);
}

void resIntLogger(long int res, long int valID) {
  printf("[LOG] Result %ld {%ld}\n", res, valID);
}

void funcEndLogger(char *funcName, long int valID) {
  printf("[LOG] End function '%s' {%ld}\n", funcName, valID);
}

void binOptLogger(int val, int arg0, int arg1, char *opName, char *funcName,
                  long int valID) {
  printf("[LOG] In function '%s': %d = %d %s %d {%ld}\n", funcName, val, arg0,
         opName, arg1, valID);
}
