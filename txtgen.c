#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BSIZE 512
#define PGSIZE 4096
#define NCHARINLINE 512

int main()
{
	int len, i, j;
	unsigned seed;
	char c, * alnum, *fname, *hex;
	int n;
    FILE* fp; 

	fname = "test.txt";
	alnum = "0123456789abcdefghizklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()";
	hex ="0123456789ABCDEF";

	len = strlen(alnum);
	//n = 4*NCHARINLINE <= 4*BSIZE ? 4*NCHARINLINE : 4*BSIZE;
	fp = fopen(fname,"w");

	seed = (unsigned)time(NULL);
	for (i = 0; i < 16*8; i++) {
		srand((unsigned)(seed + i));
    	for (j = 0; j < BSIZE; j++) {
			if ((i*BSIZE+j) % PGSIZE == 1)
				c = hex[(i*BSIZE+j) / PGSIZE];
			else if ((i*BSIZE+j) % PGSIZE == 0)
				c = '[';
			else if ((i*BSIZE+j) % PGSIZE == 2)
				c = ']';
			else if ((i*BSIZE+j) % PGSIZE == 3)
				c = ' ';
			else if ((i*BSIZE+j) == PGSIZE * 16 - 2)
				c = '\n';
			else if ((i*BSIZE+j) == PGSIZE * 16 - 1)
				c = '\0';
			else if ((i*BSIZE+j) % PGSIZE == 4)
				c = ' ';
			else if ((i*BSIZE+j) % PGSIZE == PGSIZE-1)
				c = '\n';
			else if ((i*BSIZE+j) % PGSIZE == PGSIZE-2)
				c = '\n';
			else
			    c = alnum[rand() % len];
        	fprintf(fp, "%c", c);
		}
	}
    fclose(fp);

	exit(EXIT_SUCCESS);
}
