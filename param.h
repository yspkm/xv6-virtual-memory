#define NPROC        64  // maximum number of processes
#define KSTACKSIZE 4096  // size of per-process kernel stack
#define NCPU          8  // maximum number of CPUs
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       16384  // size of file system in blocks

// PA3
#define PROT_READ	    0x1		/* Page can be read.  */
#define PROT_WRITE	    0x2		/* Page can be written.  */
#define MAP_ANONYMOUS	0x1		/* Don't use a file.  */
#define MAP_POPULATE	0x2		/* Populate (prefault) pagetables.  */
#define NMMAP           NPROC      /* Maximum number of mmap_area array */
#define MMAPBASE        0x40000000 /* addr = MMAPBASE + addr*/

#ifndef NULL 
    #define NULL            0x0
#endif
