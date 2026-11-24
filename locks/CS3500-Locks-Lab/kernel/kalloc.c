// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

//made my changes in this struct
struct {
  struct spinlock lock;
  struct run *freelist;
} kmem[NCPU];

static char kmem_lock_names[NCPU][16];
static uint64 kmem_epoch; //changes whenever a page is added to or removed from any free list

//made changes in kinit
void
kinit()
{
  //initlock(&kmem.lock, "kmem");
  for(int i=0;i<NCPU; i++){
    snprintf(kmem_lock_names[i], sizeof(kmem_lock_names[i]), "kmem%d", i);
    initlock(&kmem[i].lock, kmem_lock_names[i]);
    kmem[i].freelist=0;
  }
  kmem_epoch=0;
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  //keeping the cpu number stable while using its lock
  push_off();
  int id=cpuid();
  
  acquire(&kmem[id].lock);
  r->next = kmem[id].freelist;
  kmem[id].freelist = r;
  __atomic_fetch_add(&kmem_epoch, 1, __ATOMIC_SEQ_CST);
  release(&kmem[id].lock);
  pop_off();
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.

  

void *
kalloc(void)
{
  struct run *r;

  for(;;){
    //Record the epoch BEFORE checking any free list.
    uint64 before=__atomic_load_n(&kmem_epoch, __ATOMIC_SEQ_CST);

    //First try the current CPU's free list.
    push_off();
    int id=cpuid();

    acquire(&kmem[id].lock);
    r=kmem[id].freelist;
    if(r){
      kmem[id].freelist=r->next;
      __atomic_fetch_add(&kmem_epoch, 1, __ATOMIC_SEQ_CST);
    }
    release(&kmem[id].lock);
    pop_off();
    if(r){
      memset((char*)r, 5, PGSIZE);
      return (void*)r;
    }

    //Local list was empty. Try stealing from other CPUs.
    for(int i=0; i<NCPU; i++){
      acquire(&kmem[i].lock);
      r=kmem[i].freelist;
      if(r){
        kmem[i].freelist=r->next;
        __atomic_fetch_add(&kmem_epoch, 1, __ATOMIC_SEQ_CST);
      }
      release(&kmem[i].lock);
      if(r){
        break;
      }
    }
    if(r){
      memset((char*)r, 5, PGSIZE);
      return (void*)r;
    }
    // No page was found. Retry if any free-list change
    // occurred since we started this attempt.
    uint64 after=__atomic_load_n(&kmem_epoch, __ATOMIC_SEQ_CST);
    if(before == after){
      return 0;
    }
  }
}

