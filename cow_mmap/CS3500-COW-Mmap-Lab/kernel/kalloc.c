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

struct {
  struct spinlock lock; // protects llocator's state against concurrent access
  struct run *freelist; // points to linked list of free physical pages
  //adding my code here
  int refcnt[PHYSTOP/PGSIZE]; //stores active page table mappings for every physical page in memory 
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE){
    //updating the free change loop here
    acquire(&kmem.lock);
    kmem.refcnt[(uint64)p/PGSIZE]=1;
    release(&kmem.lock);
    kfree(p);
  }
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)

void
kfree(void *pa)
{
  struct run *r;
  uint64 index = (uint64)pa / PGSIZE;
  if(((uint64)pa % PGSIZE) != 0 ||
     (char*)pa < end ||
     (uint64)pa >= PHYSTOP)
    panic("kfree");
  acquire(&kmem.lock);
  if(kmem.refcnt[index] < 1){
    release(&kmem.lock);
    panic("kfree ref");
  }
  kmem.refcnt[index]--;

  // Other processes still reference this page.
  if(kmem.refcnt[index] > 0){
    release(&kmem.lock);
    return;
  }
  // No references remain; the page can be freed.
  release(&kmem.lock);
  memset(pa, 1, PGSIZE);
  r = (struct run*)pa;
  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  /*updating our kalloc fucntion here too*/
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r){
    kmem.freelist = r->next;
    kmem.refcnt[(uint64)r/PGSIZE]=1;
  }
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
//adding my helper function for incrementin the reference
void krefinc(void *pa){
  uint64 index=(uint64)pa /PGSIZE;
  if(((uint64)pa % PGSIZE)!=0 || (char *)pa<end || (uint64)pa>=PHYSTOP){
    panic("krefinc");
  }
  acquire(&kmem.lock);
  if(kmem.refcnt[index]<1){
    release(&kmem.lock);
    panic("krefinc ref");
  }
  kmem.refcnt[index]++;
  release(&kmem.lock);
}
