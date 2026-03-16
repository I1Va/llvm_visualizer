#include <iostream>
#include <cstdint>

extern "C" void basic_block_start_logger(uint64_t id) {
  printf("[LOG] start basic block '%ld'\n", id);
}

extern "C" void call_logger(uint64_t caller_id, uint64_t callee_id) {
  printf("[LOG] CALL '%ld' -> '%ld'\n", caller_id, callee_id);
}

extern "C" void res_int_logger(int64_t res, uint64_t val_id) {
  printf("[LOG] Result %ld {%ld}\n", res, val_id);
}

extern "C" void dump_dynamic_logger_info(char *dump_path) {
  printf("[LOG] dump into `%s`\n", dump_path);
}