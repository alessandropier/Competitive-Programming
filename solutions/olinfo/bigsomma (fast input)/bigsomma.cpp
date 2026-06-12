#include <stdio.h>

const int BUFFER_SIZE = 64 * 1024; // 64 KB: good for L1/L2 cache
char in_buf[BUFFER_SIZE]; // data buffer, size of 64 KB

long long somma(FILE *f)
{
	// 1. LOADING THE FILE INTO RAM
    // Read the first chunk of the file and put it into in_buf
    // size stores the current number of bytes read
    int size = fread(in_buf, 1, BUFFER_SIZE, f); // if size == 0 --> EOF reached
    
    // Pointer to iterate through in_buf
    char *ptr = in_buf;
    // Guard pointer marking the end of valid data in the buffer
    char *end_ptr = in_buf + size;
    
    // 2. N PARSING 
	// Parsing N alone to focus exclusively on numbers later
    int N = 0;
    
    // Convert text characters of N into an integer
    while(*ptr != '\n')
    {
    	N = N * 10 + (*ptr - '0');
    	// Every time a digit is read, we shift the accumulated number
        // to the left by multiplying it by 10. Then we add the new
        // digit, subtracting '0' (ASCII offset) to get its numerical value.
    	
    	ptr++;
	}
	
	// consuming the '\n'
	ptr++;
    
	long long totale = 0;
	int val;
	int segno;
	
	// 3. PARSING CYCLE OF NUMBERS + SUM and SIGN HANDLING
	do
	{	
		// Safety check: verify if there are enough characters left to safely parse a full number
		if(__builtin_expect(ptr > end_ptr - (1 << 5), 0)) // If less than 32 characters -> refill buffer
		{
			// Calculate the number of unread characters left at the right edge of the buffer
			int rimasti_in_buf = end_ptr - ptr;

			// Shift the remaining unread characters to the very beginning of the buffer
			for(int i = 0; i < rimasti_in_buf; i++)
				in_buf[i] = ptr[i];
			
			// Single RAM read to fill ONLY the remaining unused part of the 64 KB buffer
			size = fread(in_buf + rimasti_in_buf, 1, BUFFER_SIZE - rimasti_in_buf, f); // reading the buffer
			size += rimasti_in_buf;
			
			ptr = in_buf;
			end_ptr = in_buf + size; // Update the boundary pointer
			
			if(size == 0) return totale; // EOF reached
		}
		
		// Negative numbers handling
		segno = 1;
		if(*ptr == '-')
		{
			segno = -1;
			
			ptr++;
		}

		// Text-to-number translation
		// (as above)
		val = 0; 
		while(*ptr >= '0')
	    {
	    	val = val * 10 + (*ptr - '0');
	    	
	    	ptr++;
		}
		
		// Apply the sign and accumulate the result into the total
		totale += (long long)val * segno;
		
		// Skip the whitespace to get to the next number
		ptr++;
		
		N--;
	}
	while(N > 0);
	
	return totale;
}
