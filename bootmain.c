// Boot loader.
//
// Part of the boot block, along with bootasm.S, which calls bootmain().
// bootasm.S has put the processor into protected 32-bit mode.
// bootmain() loads an ELF kernel image from the disk starting at
// sector 1 and then jumps to the kernel entry routine.

#include "types.h"
#include "elf.h"
#include "x86.h"
#include "memlayout.h"

#define SECTSIZE  512

void readseg(uchar*, uint, uint);

void
bootmain(void)      // load the kernel from the disk into memory and jump to the first instruction 
{
  struct elfhdr *elf;   // because ./kernel file is a ELF and it has to be read, so we a need a pointer.
  struct proghdr *ph, *eph;
  void (*entry)(void);
  uchar* pa;

  elf = (struct elfhdr*)0x10000;    // scratch space  // ELF pointing to 64KB memory address.

  // Read 1st page off disk
  readseg((uchar*)elf, 4096, 0);    // (uchar *)elf is the target address, read from 0 to 4096 bytes (ELF header).

  // Is this an ELF executable?
  if(elf->magic != ELF_MAGIC)
    return;  // let bootasm.S handle error

  // Load each program segment (ignores ph flags).
  ph = (struct proghdr*)((uchar*)elf + elf->phoff);   // ph == program header is obatined by adding the elf->phoff offset
  eph = ph + elf->phnum;
  // Number of program headers
  for(; ph < eph; ph++){
    // Iterate over each program header
    pa = (uchar*)ph->paddr;   
    // The physical address to load program
    /*
      read ph->filesz bytes into 'pa', from ph->off in kernel/disk  
    */
    readseg(pa, ph->filesz, ph->off);
    if(ph->memsz > ph->filesz)
      stosb(pa + ph->filesz, 0, ph->memsz - ph->filesz);    // if in this condition stosb (store bytes) stores 0 in the rest of size
  }

  // until here segments have been setup

  // Call the entry point from the ELF header.
  // Does not return!
  /*
    * elf->entry was set by linker using kernel.ld
    * This is the address 0x80100000 specified in kernel.ld
    * See kernel.asm for kernel assembly code  
  */
  entry = (void(*)(void))(elf->entry);
  entry();    // entry.S
}

void
waitdisk(void)
{
  // Wait for disk ready.
  while((inb(0x1F7) & 0xC0) != 0x40)
    ;
}

// Read a single sector at offset into dst.
void
readsect(void *dst, uint offset)
{
  // Issue command.
  waitdisk();
  outb(0x1F2, 1);   // count = 1
  outb(0x1F3, offset);
  outb(0x1F4, offset >> 8);
  outb(0x1F5, offset >> 16);
  outb(0x1F6, (offset >> 24) | 0xE0);
  outb(0x1F7, 0x20);  // cmd 0x20 - read sectors

  // Read data.
  waitdisk();
  insl(0x1F0, dst, SECTSIZE/4);
}

// Read 'count' bytes at 'offset' from kernel into physical address 'pa'.
// Might copy more than asked.
void
readseg(uchar* pa, uint count, uint offset)   // reads sectors of size 512 // SECTSIZE = 512, all disks are available with 512 only.
{
  uchar* epa;

  epa = pa + count;

  // Round down to sector boundary.
  pa -= offset % SECTSIZE;

  // Translate from bytes to sectors; kernel starts at sector 1.
  offset = (offset / SECTSIZE) + 1;

  // If this is too slow, we could read lots of sectors at a time.
  // We'd write more to memory than asked, but it doesn't matter --
  // we load in increasing order.
  for(; pa < epa; pa += SECTSIZE, offset++)
    readsect(pa, offset);   // Data will be read from the physical harddisk and put into memory.
}
