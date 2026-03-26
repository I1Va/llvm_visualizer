#include <stdio.h>

// Small function calls force a BB break in the caller if not inlined
void log_step(int logic) {
    if (logic) printf("Step A\n");
}

int main(int argc, char** argv) {
    int x = argc;
    int result = 0;

    // 1. Ternary operators often create 3 small BBs (cond, true, false)
    result = (x > 1) ? (x * 2) : (x + 5);

    // 2. Explicit labels and gotos create mandatory BB boundaries
    if (result > 10) 
    
    result -= 1;

    // 3. Sequential function calls create "Call" edges and split blocks
    log_step(1);
    result += 100;

    // 4. Multiple exit points or forced returns
    return (result % 2 == 0) ? 0 : 1;
}