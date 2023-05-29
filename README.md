# XV6 Virtual Memory

This project involves modifications to the XV6 operating system to implement virtual memory management capabilities. The focus is on implementing a memory mapped files feature, including system calls such as mmap(), munmap(), and a page fault handler. The freemem() syscall is also implemented to provide information about the current number of free memory pages. 

## Build and Run

To build and run the project, execute the following command in your terminal:
```bash
$ sh build.sh
```

## QEMU Usage 

Interrupt functionality has been implemented for QEMU. You can trigger an interrupt with C-c and shut down the system with C-a + x.

```bash
SeaBIOS (version 1.15.0-1)

Booting from Hard Disk..xv6...
```

After booting the xv6 system, you can run the following commands:

```bash
$ vmtest # test without file
```
```bash
$ vmtest file # test with file
```
```bash
$ vmtestiter # test without file and iteration
```
```bash
$ vmtestiter file # test with file and iteration
```
```bash
$ freemem # test to call freemem system call 
```
## Syscalls

### 1. mmap()

This syscall is used to create a new mapping in the virtual address space of the calling process. 

#### 1.1 addr

The addr parameter is always page-aligned. MMAPBASE + addr provides the start address of mapping. Each process’s virtual address MMAPBASE is 0x40000000.

#### 1.2 length 

The length is also a multiple of the page size. MMAPBASE + addr + length is the end address of the mapping.

#### 1.3 prot

The prot can be PROT_READ or PROT_READ|PROT_WRITE. It should match the file’s open flag.

#### 1.4 flags

The flags parameter can be given with combinations of certain values:

1.4.1 If MAP_ANONYMOUS is given, it implies anonymous mapping.

1.4.2 If MAP_ANONYMOUS is not given, it signifies file mapping.

1.4.3 If MAP_POPULATE is given, it allocates physical page & make page table for the whole mapping area.

1.4.4 If MAP_POPULATE is not given, it only records its mapping area. If a page fault occurs in the according area (access to mapping area’s virtual address), it allocates a physical page & make a page table for the according page.

1.4.5 Other flags are not used.

#### 1.5 fd

The fd is given for file mappings, if not, it should be -1.

#### 1.6 offset

The offset is given for file mappings, if not, it should be 0.

The mmap syscall returns the start address of the mapping area on success, and 0 on failure. Failures could be due to several reasons, such as when the fd is -1 but the mapping is not anonymous, the protection of the file and the prot of the parameter are different, the mapping area is overlapped, or other additional errors occur.

### 2. Page Fault Handler (trap)

The Page Fault Handler is designed to handle accesses on the mapping region where a physical page & page table has not been allocated. 

#### 2.1 Page Fault Handler

The handler aims to allocate physical pages and page table entries when a page fault occurs, allowing the process to work without any problems. If this fails, the process is terminated.

#### 2.2 Page Fault Occurrence

A page fault (interrupt 14, T_PGFLT) is caught when an access (read/write) occurs.

#### 2.3 Determining Fault Address and Access Type

In the page fault handler, the fault address is determined by reading the CR2 register (using rcr2()). The type of access

 (read or write) is also determined:

2.3.1 read: tf->err&2 == 0

2.3.2 write: tf->err&2 == 1

#### 2.4 Mapping Region

The faulted address's corresponding mapping region is found in mmap_area. If there's no corresponding mmap_area for the faulted address, it returns -1.

#### 2.5 Write Protection

If a write fault occurs while mmap_area is write-prohibited, it returns -1.

#### 2.6 Page Allocation

For only one page according to the faulted address, a new physical page is allocated, filled with 0. If it's a file mapping, the file is read into the physical page with offset. If it's an anonymous mapping, the page which is filled with 0s is left as is. Finally, a page table is made & filled properly (if it was PROT_WRITE, PTE_W should be 1 in PTE value).

### 3. munmap() syscall

The munmap syscall is used to delete the mappings for the specified address range.

#### 3.1 munmap(addr)

Unmaps the corresponding mapping area. Returns 1 on success and -1 on failure.

#### 3.2 addr

The addr will always be given with the start address of the mapping region, which is page aligned.

#### 3.3 Removing mmap_area Structure

munmap() should remove the corresponding mmap_area structure. If there is no mmap_area of the process starting with the address, it returns -1.

#### 3.4 Freeing Physical Pages and Page Table

If a physical page is allocated & a page table is constructed, they should be freed. When freeing the physical page, it should be filled with 1 and returned to the free list.

#### 3.5 Removing mmap_area Structure

If a physical page is not allocated (page fault has not occurred on that address), just remove the mmap_area structure.

### 4. freemem() syscall

The freemem syscall is used to return the current number of free memory pages.

When the kernel frees (puts a page into the free list), freemem should increase, and when the kernel allocates (takes a page from the free list and gives it to a process), freemem should decrease.

## Screenshots

The screenshot shows the output after running the xv6 operating system and executing the vmtest, vmtest file, vmtestiter, vmtestiter file, and freemem commands.

![Screenshot](https://user-images.githubusercontent.com/71680670/213357441-0930e4ba-bb61-43fb-86d3-ad3abb1edd7e.png)

## Contributing 

This project was developed as part of the SWE3004 Operating System course in 2022 by Yosep Kim. The source code is available on [GitHub](https://github.com/ypskm/xv6-virtual-memory).

For any queries or issues, please raise them on the repository or reach out to the developer. Your contributions to improving this implementation are most welcome.
