#include <stdio.h>

const int BUFFER_SIZE = 64 * 1024; // 64 KB: coincide con la dimensione delle cache L1/L2
char in_buf[BUFFER_SIZE]; //buffer che contiene tutti i dati

long long somma(FILE *f)
{
	// 1. CARICAMENTO DEL FILE IN RAM
    // Legge tutto il file e lo inserisce in in_buf
    // dimensione del buffer attuale
    int size = fread(in_buf, 1, BUFFER_SIZE, f); // se size == 0 --> EOF raggiunto
    
    // Puntatore per scorrere in_buf
    char *ptr = in_buf;
    // Puntatore che segna la fine dei dati validi nel buffer
    char *end_ptr = in_buf + size;
    
    // 2. PARSING DI N 
	// solo N cosi dopo posso scorrere i numeri
    int N = 0;
    
    // Converte i caratteri di N in numero
    while(*ptr != '\n')
    {
    	N = N * 10 + (*ptr - '0');
    	// ogni volta che leggiamo una cifra spostiamo il numero letto
		// finora a sinistra e per farlo moltiplichiamo per 10.
    	// Dopo sommiamo la cifra letta ora da cui sottraggo '0' per
    	// ottenerla
    	
    	ptr++;
	}
	
	// consumiamo il '\n'
	ptr++;
    
	long long totale = 0;
	int val;
	int segno;
	
	// 3. CICLO PARSING DEI NUMERI + SOMMA e GESTIONE NEGATIVI
	do
	{	
		// Unico controllo: vedo se ci sono abbastanza caratteri per terminare 1 ciclo
		if(__builtin_expect(ptr > end_ptr - (1 << 5), 0)) // se i caratteri mancanti sono meno di 32 rileggiamo
		{
			// caratteri rimasti nel buffer ancora da leggere
			int rimasti_in_buf = end_ptr - ptr;

			// salvare i caratteri rimasti nel buffer
			for(int i = 0; i < rimasti_in_buf; i++)
				in_buf[i] = ptr[i];
			
			// rileggere il buffer SOLO del restante per completarlo
			size = fread(in_buf + rimasti_in_buf, 1, BUFFER_SIZE - rimasti_in_buf, f); // rileggo il buffer
			size += rimasti_in_buf;
			
			ptr = in_buf;
			end_ptr = in_buf + size; // aggiorniamo la fine
			
			if(size == 0) return totale; // file finito
		}
		
		// Gestione del segno meno per i numeri negativi
		segno = 1;
		if(*ptr == '-')
		{
			segno = -1;
			
			ptr++;
		}

		// Conversione rapida da testo a numero long long
		// (come sopra)
		val = 0; 
		while(*ptr >= '0')
	    {
	    	val = val * 10 + (*ptr - '0');
	    	
	    	ptr++;
		}
		
		// Applica il segno meno se necessario e accumula il risultato nel totale
		totale += (long long)val * segno;
		
		// Passaggio al numero successivo
		while(*ptr <= ' ')
			ptr++;
		
		N--;
	}
	while(N > 0);
	
	return totale;
}
