int test1() {
    // Assembly steps:
    // 1. Allocate stack space for y: sub stack pointer
    // 2. Store 10 in y: mov w8, #10 (store 10 in register)
    // 3. Store y in memory: str w8, [sp, #12] (store register in memory)
    int y = 10;

    // 4. Clean up: Move stack pointer up
    // 5. ret
}

// // TEST 1: VARIABLES & DATA TYPES
// int main() {

//     // Test 1: Declaration
//     int x;
//     int y = 10;

//     // Test 2: Assignment
//     x = 5;

//     // Test 3: Postfix operations
//     x++;
//     x--;
//     x = y++;

//     // Test 4: Unary operations
//     x = -5;
//     x = +5;
//     x = ++y;
//     x = --y;

//     // // Test 4: Operations
//     // x = 1 + 2;
//     // x = 1 - 2;
//     // x = 1 * 2;
//     // x = 4 / 2;
//     // x = 5 % 2;

//     // // Test 5: Precedence
//     // x = 1 + 2 - 3;
//     // x = 1 + 2 * 3;
//     // x = 1 % 2;
//     // x = (1 + 2) * 3;


//     return x;
// }
