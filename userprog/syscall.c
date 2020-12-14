#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);
static bool
get_user (const uint8_t *uaddr)
{
int result;
asm ("movl $1f, %0; movzbl %1, %0; 1:"
: "=&a" (result) : "m" (*uaddr));
return result;
}

/* Writes BYTE to user address UDST.
UDST must be below PHYS_BASE.
Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
  : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
 
  switch (*(int *)f->esp)
  {
  case SYS_WRITE:{
      int fsize = *((int*)f->esp + 3);
      char *buff = (char*)*((int*)f->esp + 2);
      int fd = *((int*)f->esp + 1);

      switch (fd)
      {
        case 1: // wirte to console 
          putbuf (buff, fsize); 
          break;
        default:
          break;
      }

    }
    break;
  case SYS_EXIT:{
    
  }
  default:
    break;
  }
 
}
