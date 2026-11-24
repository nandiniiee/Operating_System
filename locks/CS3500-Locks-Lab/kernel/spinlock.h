// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
#ifdef LAB_LOCK
  int nts;
  int n;
#endif
};

#ifdef LAB_LOCK
// Reader-writer lock.
struct rwspinlock {
  // Replace this with your implementation.
  struct spinlock guard; //guard taht protects the reader-writer's lock state
  int readers;//number of readers currently holding the lock
  int waiting_writers;//number of writers waiting to enter
  int writer_active;// 1 if writer holding lock else 0
  //uint64 state;
};
#endif
