# natoabc

_Link to the Training Olinfo problem [here](https://training.olinfo.it/task/ois_natoabc)._\
_Source code [here](https://github.com/alessandropier/Competitive-Programming/blob/main/solutions/olinfo/natoabc/natoabc.cpp)._

## Overview
Given a string of NATO codes (without spaces), the output should contain only the initials of each word. 

<div align="center">

|  |  |  |  |  |  |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **a** – alpha | **b** – bravo | **c** – charlie | **d** – delta | **e** – echo | **f** – foxtrot |
| **g** – golf | **h** – hotel | **i** – india | **j** – juliett | **k** – kilo | **l** – lima |
| **m** – mike | **n** – november | **o** – oscar | **p** – papa | **q** – quebec | **r** – romeo |
| **s** – sierra | **t** – tango | **u** – uniform | **v** – victor | **w** – whiskey | **x** – xray |
| **y** – yankee | **z** – zulu | | | | |
</div>

Here are some examples of how the code should work:

<div align="center">

| input | output |
| :---: | :---: |
| kilooscardeltakilouniformpapaalpha | kodkupa |
| indiaindiaoscartango | iiot |
</div>

## Solution

Having a good solution is alright but having a fast solution is **better**. How do we achieve the fastest solution? *(0.02 seconds)*

1. **Input Buffer**: reading **all at once** is faster than using the standard input, fscan or others. By doing this, we get **everything** at once and then we execute the code on the block just read.
2. **Alphabet Dictionary**: pre-loaded dictionary of **pointers strings** to nato codes. Using pointers allows us to be quicker and use less memory.
3. **Array of Lengths**: instead of calculating the nato code's length every time, a pre-loaded array of lengths allows us to do it quicker.

## Implementation of Each Point

```cpp
// 1: reading as a block (fast I/O)
(void)freopen("input.txt", "r", stdin);
(void)freopen("output.txt", "w", stdout);

// fread puts everything in "in_buf" and returns the numbers of chars read
size_t bytes_read = fread(in_buf, 1, BUFFER_SIZE - 1, stdin);
in_buf[bytes_read] = '\0'; // places the '\0' at the end

// 2: dictionaries for alph and lengths of the words
const char* alph[26] = {
"alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel", "india", "juliett", "kilo", "lima", "mike", "november", "oscar", "papa",  "quebec", "romeo", "sierra", "tango", "uniform", "victor", "whiskey", "xray", "yankee", "zulu"
};

// 3: array w lengths of words in "alph"
const int lengths[26] = {5, 5, 7, 5, 4, 7, 4, 5, 5, 7, 4, 4, 4, 8, 5, 4, 6, 5, 6, 5, 7, 6, 7, 4, 6, 4};
```
## Parsing

**2 pointers** are used:

1. **ptr**: used for _reading_; it points to the beginning of the input
2. **out_ptr**: used for _writing_; it points to the beginning of the output (walks as we write)

As each character is read, this while cycle runs:
```cpp
while (*ptr >= 'a' && *ptr <= 'z') {
        *out_ptr++ = *ptr;
        ptr += lengths[*ptr - 'a'];
    }
```

As soon as new characters are read, the **first character** is saved inside **out_ptr** _(pointars that contains the output characters)_ and then it's incremented:
```cpp
        *out_ptr++ = *ptr;
        
        // is equals to:

        *out_ptr = *ptr; // copy of the value
        out_ptr++;       // increment
```

Then, we increment **ptr** _(used for reading from the buffer)_ by the length of the NATO code just read:
```cpp
ptr += lengths[*ptr - 'a'];
```

> **note**: ```*ptr - 'a'``` allows us to retrieve the index of the ```lengths``` array by using ASCII codes.

<div align="center">

| Char | Dec | Char | Dec | Char | Dec | Char | Dec |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **a** | 97 | **h** | 104 | **o** | 111 | **v** | 118 |
| **b** | 98 | **i** | 105 | **p** | 112 | **w** | 119 |
| **c** | 99 | **j** | 106 | **q** | 113 | **x** | 120 |
| **d** | 100 | **k** | 107 | **r** | 114 | **y** | 121 |
| **e** | 101 | **l** | 108 | **s** | 115 | **z** | 122 |
| **f** | 102 | **m** | 109 | **t** | 116 |  |  |
| **g** | 103 | **n** | 110 | **u** | 117 |  |  |

</div>

### Example: 
If the NATO code is **hotel** we consider the letter **h**. 

We perform ```'h' - 'a'``` and get: ```104 - 97 = 7```.

```alph[7]``` and ```lengths[7]``` both corrispond to the NATO code **hotel**.

> By doing this, the remaining letters of the NATO code just read are completely **skipped**, going straight to the **next NATO code**.

## Output

This is used to write the output:
```cpp
size_t bytes_written = fwrite(out_buf, 1, out_ptr - out_buf, stdout);
```

This line writes the processed data from the temporary buffer directly to the standard output (`stdout`). 

#### Parameters Breakdown:
* **`out_buf`**: Array of chars for the output(output buffer).
* **`1`**: Size of each element to write (1 byte, since it is a character buffer).
* **`out_ptr - out_buf`**: Total number of elements to write. Used to calculate the exact number of bytes currently stored in the buffer by subtracting the start pointer (`out_buf`) from the current position pointer (`out_ptr`).
* **`stdout`**: The output stream destination.

#### Return Value:
* **`bytes_written`**: Stores the actual number of bytes successfully written. This is useful for error checking to ensure the entire buffer was flushed correctly. _(not useful here)_
