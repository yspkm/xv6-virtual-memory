# XV6 Virtual Memory

## Build and Run

```bash
$ sh build.sh
```

## QEMU  

​	Interrupt C-c functionality implemented, shutdown with C-a + x

```bash
SeaBIOS (version 1.15.0-1)


iPXE (https://ipxe.org) 00:03.0 CA00 PCI2.10 PnP PMM+1FF8B4A0+1FECB4A0 CA00
                                                                               


Booting from Hard Disk..xv6...
cpu0: starting 0
sb: size 16384 nblocks 16321 ninodes 200 nlog 30 logstart 2 inodestart 32 bmap start 58
init: starting sh

2022 SWE3004 Operating System PA3 
2017312605 Yosep Kim.
https://github.com/ypskm/xv6-virtual-memory

$ vmtest 
$ vmtest file
$ vmtestiter
$ vmtestiter file
$ freemem
```

![스크린샷 2023-01-19 13-44-26](https://user-images.githubusercontent.com/71680670/213357441-0930e4ba-bb61-43fb-86d3-ad3abb1edd7e.png)

## 1. mmap() syscal

###   1.1 addr is always page-aligned

​	MMAPBASE + addr is the start address of mapping
​	MMAPBASE of each process’s virtual address is 0x40000000

###   1.2 length is also a multiple of page size

​	MMAPBASE + addr + length is the end address of mapping

###   1.3 prot can be PROT_READ or PROT_READ|PROT_WRITE

​	prot should be match with file’s open flag

###   1.4 flags can be given with the combinations

​	1.1.4.1 If MAP_ANONYMOUS is given, it is anonymous mapping
​	1.1.4.2 If MAP_ANONYMOUS is not given, it is file mapping
​	1.1.4.3 If MAP_POPULATE is given, allocate physical page & make page table for whole mapping area.
​	1.1.4.4 If MAP_POPULATE is not given, just record its mapping area. (If page fault occurs to according area (access to mapping area’s virtual address), allocate physical page & make page table to according page)
​    1.1.4.5 Other flags will not be used

    1.1.5 fd is given for file mappings, if not, it should be -1 
    1.1.6 offset is given for file mappings, if not, it should be 0
###   Return

​    Succeed: return the start address of mapping area
​    Failed: return 0

     - It's not anonymous, but when the fd is -1
          - The protection of the file and the prot of the parameter are different
          - The situation in which the mapping area is overlapped is not considered
               - If additional errors occur, we will let you know by writing notification

## 2. Page Fault Handler (trap)
###   2.1 Page Fault Handler

​    2.1.1 Page fault handler is for dealing with access on mapping region with physical page & page table is not allocated
​    2.1.2 Succeed: Physical pages and page table entries are created normally, and the process works without any problems
​    2.1.3 Failed: The process is terminated

###   2.2 When an access occurs (read/write), catch according page fault (interrupt 14, T_PGFLT) in

###   2.3 In page fault handler, determine fault address by reading CR2 register(using rcr2()) & access was read or write

​    2.3.1 read: tf->err&2 == 0
​    2.3.2 write: tf->err&2 == 1

###   2.4 Find according mapping region in mmap_area

​    If faulted address has no corresponding mmap_area, return -1

###   2.5.If fault was write while mmap_area is write prohibited, then return -1

###   2.6For only one page according to faulted address

​    2.5.1 Allocate new physical page
​    2.5.2 Fill new page with 0
​    2.5.3 If it is file mapping, read file into the physical page with offset
​    2.5.4 If it is anonymous mapping, just left the page which is filled with 0s
​    2.5.5 Make page table & fill it properly (if it was PROT_WRITE, PTE_W should be 1 in PTE value)

## 3. munmap() syscall
###   3.1 munmap(addr)

​    3.1.1 Unmaps corresponding mapping area
​    3.1.2 Return value: 1(succeed), -1(failed)

###   3.2 addr will be always given with the start address of mapping region, which is page aligned

###   3.3 munmap() should remove corresponding mmap_area structure 

​    If there is no mmap_area of process starting with the address, return -1

###   3.4 If physical page is allocated & page table is constructed, should free physical page & page table 

​    When freeing the physical page should fill with 1 and put it back to freelist

###   3.5 If physical page is not allocated (page fault has not been occurred on that address), just remove mmap_area structure.

###   3.6 Notice) In one mmap_area, situation of some of pages are allocated and some are not can happen.

## 4. freemem() syscall
###   4.1 freemem()  

​    syscall to return current number of free memory pages

###   4.2 When kernel frees (put page into free list), freemem should be increase

###   4.3 When kernel allocates (takes page from free list and give it to process), freemem should decrease