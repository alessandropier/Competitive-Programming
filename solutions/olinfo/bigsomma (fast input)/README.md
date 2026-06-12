# Bigsomma (fast input)

_Link to the Training Olinfo problem [here](https://training.olinfo.it/task/bigsomma)._\
_Source code [here](https://github.com/alessandropier/Competitive-Programming/blob/main/solutions/olinfo/bigsomma%20(fast%20input)/bigsomma.cpp)._

As shown in the attached PDF file in the repository and on the official website, the solution requires a simple sum of N numbers.

## Overview

This problem is very interesting because the logic is simple, but the real challenge is implementing the solution under the time constraint.



The limits are:

    Time limit: 1s      |       Memory limit: 256 MB

As you might imagine, the real challenge is making the code run in under 1 second, especially since the numbers are quite large.

## What does **not** work
The easiest idea to come to mind would be a simple for
```cpp
int sum = 0;
int val = 0;

for(int i = 0; i < N; i++){
    cin >> val;
    sum += val;
}
```

but unfortunately this does not work _(otherwise I would not be talking about it)_ for multiple reasons: 
1. **Integer Overflow:** Storing the accumulator in a standard 32-bit `int` will cause an overflow. Given the constraints ($50,000,000 \times 67,108,863$), the maximum potential sum requires up to 52 bits. Using a 64-bit `long long` is mandatory.
2. **Excessive System Calls:** Standard functions like `std::cin` or `scanf` read data in small fragments. Processing 50 million numbers triggers thousands of expensive System Calls to request more data from the Operating System, wasting a massive amount of CPU cycles.
3. **Cache Inefficiency (Cache Misses):** Because the data is read in fragmented chunks from the RAM, the CPU constantly goes back and forth to the main RAM to fetch single numbers from the input file. The CPU suffers from multiple **cache misses**, forcing it to wait for the slow main memory instead of fetching data instantly from the L1/L2 cache.
4. **The `sync_with_stdio` Illusion:** Even adding the classic one-liner optimization:
```cpp
    ios_base::sync_with_stdio(false);
```
will **not** save this solution because it doesn't bypass the heavy parsing logic of `std::cin`.

Since the logic of the problem is trivial, the only way to pass the time limit is to address the core issue: **the input bottleneck**. This requires a custom *Fast I/O* implementation.

## Goals

Eliminate the I/O bottleneck by:

* **Drastically reduce RAM access cycles:** Instead of querying the slow main RAM for every single value, the goal is to fetch data in massive, structural chunks using a custom memory buffer.
* **Maximize CPU Cache efficiency:** Keep the active dataset strictly within the boundaries of the L1/L2 CPU cache. This allows the processor to fetch subsequent bytes in 1-5 clock cycles rather than waiting for the 150-200 cycles required by standard RAM reads.
* **Bypass heavyweight parsing:** Strip away all unnecessary features of standard library parsers (_like timezone, floating-point checks, and region locales_). The goal is to perform a direct, lightweight math conversion (`val * 10 + (...)`) executing minimal assembly operations per digit.
* **Achieve $\mathcal{O}(1)$ Space Complexity:** Avoid loading the full array of 50 million integers into memory. By accumulating the sum **on-the-fly** while processing the cache buffer, the application maintains a near-zero memory footprint.

## Solution

The solution overcomes the RAM bottleneck by loading the input file in 64 KB chunks into a static buffer (`in_buf`) using a single `fread` operation.

At a high level, the program does not process the input as individual numerical types, but rather as a continuous text stream:
1. **Buffer Loading:** The application pulls a massive block of text from the file straight into our 64 KB cache-aligned buffer.
2. **Safety Check:** Before a new number is processed, the code checks if there is enough contiguous string space left in the buffer to guarantee the number won't be cut in half. If space is running low, it refills the buffer ahead of time.
3. **Sign & Digits:** Moving sequentially via text pointers, the algorithm instantly isolates the negative sign (if present) and extracts the numeric characters.
4. **Accumulation:** The raw characters are mathematically converted into a 64-bit integer and added to the total sum immediately, maintaining an optimal $O(1)$ memory footprint.

> **Buffer Tuning:** Through rigorous testing, a `BUFFER_SIZE` of **64 KB** was found to be the sweet spot. Lowering the buffer to **32 KB** triggers too many sequential `fread` calls and saturates the L1 Data boundaries of some CPUs. Conversely, raising it to **128/512 KB** overflows the L1 cache entirely, spilling into broader memory structures and increasing I/O latency. At 64 KB, the buffer fits perfectly inside the fast L1/L2 core boundary while keeping Operating System calls to an absolute minimum. (_64 KB is the only buffer size that works_)

Below is a detailed breakdown of the architectural choices, memory layouts, and micro-optimizations that allow this step-by-step process to run with near-zero latency:

### 1. Memory Management & File Traversal
To achieve maximum efficiency, the entire file processing strategy relies on strict memory control:
* **Pointers:** Instead of reading line-by-line, the code populates the 64 KB cache buffer using `fread`. Then it initializes a pointer (`ptr`) to iterate through the text characters sequentially.
* **Boundary Control:** To prevent reading garbage data, a secondary pointer (`end_ptr`) is locked at the exact end of the valid data currently inside the buffer (`in_buf + size`). This guarantees absolute control over our position at any given cycle.

### 2. Custom Lightweight Parsing
Standard string conversion functions are bypassed in favor of a specialized arithmetic parser:
* **Sign and Digits:** The parser performs a simple sign check (`*ptr == '-'`) and shifts the binary value of the current character by subtracting the ASCII character `'0'`.
* **Text-to-integer translation:** The text-to-integer translation executes directly on the character stream through raw base-10 arithmetic (`val = val * 10 + (*ptr - '0');`). This avoids the high abstraction overhead of `std::stoi` or `atoi`, consuming only 2 to 3 CPU clock cycles per processed digit.

### 3. The Safety Mechanism & Ahead-of-Time Buffer Refill
The most advanced part of the algorithm is the preventive buffer reload loop, which acts as a safety measure before a number is cut in half at the edge of the buffer:
* **Why 32 Characters?** A single integer under the specified problem constraints can be up to 9–10 characters long (including the minus sign). By checking if we have fewer than 32 characters left (`ptr > end_ptr - 32`), we ensure there is *always* enough contiguous string space to fully parse the next number without facing an abrupt End-of-Buffer error. _(note: here 32 is not the only number that works. every number bigger than the maximum digits of numbers would work. For example: 11 would work just fine because 11 > 10)_
* **Branch Prediction via `__builtin_expect`:** Refilling the buffer occurs very rarely (only once every 64 KB of processed text). The compiler macro `__builtin_expect(..., 0)` forces the CPU to optimize for the _most common path_ (the parsing loop), treating the reload branch as highly unlikely. This prevents the CPU pipeline from stalling and guarantees steady throughput.
* **The Refill Logic:** When the threshold is triggered, the few remaining characters at the end of the buffer are shifted to the very beginning (`in_buf[i] = ptr[i]`). Then, a single `fread` requests only the missing bytes from the Operating System to fill the 64 KB buffer, resetting `ptr` and `end_ptr`.


## Key Optimizations:
* **Cache-Aligned Buffer:** By restricting the buffer to 64 KB, we prevent the data from spilling into the slower main RAM during processing.
* **Locale-Free Arithmetic Parsing:** Instead of using heavy standard libraries, text characters are converted directly into integers using basic math operations (`val * 10 + (...)`). This takes only 2-3 CPU cycles per digit.
* **Branch Prediction Optimization:** The `__builtin_expect` macro informs the compiler that refilling the buffer is a rare event. The CPU pre-fetches the main processing path, eliminating pipeline stalls.
* **On-the-Fly Accumulation:** Numbers are summed instantly as they are parsed, achieving an optimal $\mathcal{O}(1)$ Space Complexity.