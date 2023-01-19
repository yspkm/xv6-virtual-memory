#pragma once

// pa3
struct mmap_area{
  struct file * f;
  uint addr;
  int length;
  int offset;
  int prot;
  int flags;
  int fd;
  int fork;
  struct proc *p; // the process with this mmap_area
};

void print_mtable(char*);
