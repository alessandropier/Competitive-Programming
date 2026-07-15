// https://training.olinfo.it/task/ois_natoabc

// 1. extreme optimization for GCC compiler
#pragma GCC optimize("O3,unroll-loops")           // max speed for cycles
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")  // uses modern hardware instructions

#include <iostream>
#include <cstdio>   // freopen, fread, fwrite (Fast I/O)

using namespace std;

// 2. static global buffers
const int BUFFER_SIZE = 1 << 25; // moving bits: 2^25 = ~33MB (huge)
char in_buf[BUFFER_SIZE];        // Array of chars for input
char out_buf[BUFFER_SIZE];       // Array of chars for output

// 3. dictionaries for alph and lengths of the words
const char* alph[26] = {
    "alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel", 
    "india", "juliett", "kilo", "lima", "mike", "november", "oscar", "papa", 
    "quebec", "romeo", "sierra", "tango", "uniform", "victor", "whiskey", "xray", 
    "yankee", "zulu"
};
// array w lengths of words in "alph"
const int lengths[26] = {5, 5, 7, 5, 4, 7, 4, 5, 5, 7, 4, 4, 4, 8, 5, 4, 6, 5, 6, 5, 7, 6, 7, 4, 6, 4};

int main() {  
    // 4. I/O
    //we use (void) to avoid compiling errors
    (void)freopen("input.txt", "r", stdin);
    (void)freopen("output.txt", "w", stdout);
    
    // 5. reading as a block (pure fast I/O)
    // fread puts the whole file inside "in_buf" and returns the numbers of chars read
    size_t bytes_read = fread(in_buf, 1, BUFFER_SIZE - 1, stdin);
    in_buf[bytes_read] = '\0'; // places the '\0' at the end

    // 6. pointers
    char* ptr = in_buf;       // reading: points to the beginning of the input
    char* out_ptr = out_buf;  // writing: points to the beginning of the output (walks as we write)

    // 7. parsing
    while (*ptr >= 'a' && *ptr <= 'z') {
        *out_ptr++ = *ptr;
        ptr += lengths[*ptr - 'a'];
    }

    // 8. print
    // The subtraction (out_ptr - out_buf) calculates the distance between the two pointers, indicating the total number of bytes to write
    size_t bytes_written = fwrite(out_buf, 1, out_ptr - out_buf, stdout);
    (void)bytes_written; // Consume the variable to prevent the compiler from generating an unused code warning

    return 0;
}
