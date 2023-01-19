#include "param.h" 
#include "types.h" 
#include "stat.h" 
#include "fcntl.h"
#include "user.h"
#include "memlayout.h"

#ifndef NELEN
#define NELEN(x)(sizeof(x)/sizeof(x[0]))
#endif

#ifndef BSIZE
#define BSIZE 512
#endif

#ifndef PGSIZE 
#define PGSIZE 4096
#endif

#define NTEST 5

void printer(char * buf, int len)
{
	char c;
	for (int i = 0; i < len; i++) 
	{
		if (i % PGSIZE == 0)
			printf(1, "\t");
		else if (i % PGSIZE == PGSIZE-1)
			printf(1, "\n");
		c = buf[i];
		printf(1, "%c", c);
	}

}

int 
main(int argc, char* argv[])
{

	int fd, pid, print; 
	char * alloc[NTEST]; 
	print = 0;
	if (argc > 1)
		print = 1;
	fd = open("test.txt", O_RDWR); 

	int len[NTEST] = {8*PGSIZE, 16*PGSIZE,   2*PGSIZE, 4*PGSIZE, 16*PGSIZE};
	int idx[NTEST] = {0};
	for (int i = 1; i < (int)NELEN(idx); i++)
	{
		idx[i] = idx[i-1] + len[i-1];
	}
	//= {0,        2*PGSIZE, 3*PGSIZE, 8*PGSIZE, 11*PGSIZE};   
	// fd, anon, fd&pop, anon&pop, fd&pop
	int flag[NTEST] = {0, MAP_ANONYMOUS, MAP_POPULATE, MAP_ANONYMOUS|MAP_POPULATE, MAP_POPULATE};
	int prot[NTEST] = {PROT_READ|PROT_WRITE, PROT_READ|PROT_WRITE, PROT_READ|PROT_WRITE, PROT_READ|PROT_WRITE, PROT_READ|PROT_WRITE};
	int fdarr[NTEST] = {fd, -1, fd, -1, fd};
for (int i = 0;i < (int)NELEN(idx); i++){
		printf(1, "\n");
		printf(1, "before mmap: freemem is %d\n", freemem());
		printf(1, "%d: mmap:(M[%d*PGSIZE], %d*PGSIZE, %s, %s)->", i, idx[i]/PGSIZE, len[i]/PGSIZE, flag[i]&MAP_POPULATE ? "pop":"unpop", flag[i]&MAP_ANONYMOUS?"anon":"fd");

		alloc[i] = (char*) mmap(idx[i], len[i], prot[i], flag[i], fdarr[i], 512);
		printf(1, "Mem[%x]\n", alloc[i]);
	 	//mmap(idx[i], len[i], prot[i], flag[i], fdarr[i], 0);

/*
		if (!(flag[i] & MAP_POPULATE)) {
			for (int p=PGSIZE; p <= len[i]; p+=PGSIZE)
				alloc[i][p-1] = '\0'; // null character
			printf(1, "\n\tcontents\n%s\n", alloc[i]); 
		}
		*/

		printf(1, "after mmap: freemem is %d\n", freemem());
		printf(1, "\n");
	}
if (print) {
	printf(1, "\n");
	alloc[0][4*PGSIZE-1] = '\0';
	printf(1, "%s\n", &alloc[0][3*PGSIZE]);
}
alloc[3][0]  = 'A';
alloc[3][1]  = 'B';
alloc[3][2]  = '\0';
	printf(1, "before fork: freemem is %d\n", freemem());
	pid = fork();

	if (pid) {
		wait();
		printf(1, "\nafter child exit, freemem: %d\n\n", freemem());
		printf(1, "Parent (pid:%d) process\n", getpid()); 
		printf(1, "after fork: freemem is %d \t", freemem());
		printf(1, "\n");
		pm();
		printf(1,"\n");
	} else {
		// since the real entry of pgdir of child is made on demand 
		alloc[3][2]  = 'C';
		alloc[3][3]  = '\0';
		printf(1, "%s\n", alloc[3]);
		printf(1, "Child (pid:%d) process\n", getpid()); 
		printf(1, "after fork: freemem is %d \t", freemem());
		printf(1, "\n");
		pm();
		printf(1,"\n");
	}

	for (int i = 0;i < (int)NELEN(idx); i++){
		printf(1, "\n");
		printf(1, "%d: munmap:(M[M2V(%d * PGSIZE)], %d * PGSIZE, %s, %s)\n",\
				i, idx[i]/PGSIZE, len[i]/PGSIZE, flag[i]&MAP_POPULATE ?\
				"pop":"unpop", flag[i]&MAP_ANONYMOUS?"anon":"fd");

		//if (!(flag[i] & MAP_ANONYMOUS) && (flag[i] & MAP_POPULATE)) {
		//}

		printf(1, "before munmap: freemem is %d\n", freemem());
		if (print) {
		  printf(1, "\n%s\n\n", alloc[i]); 
		  printf(1, "\n");
		}
		printf(1, "unmap_result: %d\n", munmap(M2V(idx[i])));
		printf(1, "after munmap: freemem is %d\n", freemem());
		pm();
		printf(1, "\n");
	}
	//alloc[3][10] = 0;

	if (pid)	
		printf(1, "FINAL freemem: %d\n", freemem());

    close(fd);
	exit();
	return 0;
}

