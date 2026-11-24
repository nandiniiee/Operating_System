# Operating Systems

This repository contains my implementations and coursework exploring fundamental concepts of Operating Systems, with a focus on understanding kernel internals, process management, memory management, system calls, and file systems through hands-on programming.

The projects involve working with **xv6**, a Unix-like teaching operating system, as well as implementing utilities and exploring core operating system mechanisms.

## Repository Structure

Each directory focuses on a specific Operating Systems concept and contains its own README with implementation details, design decisions, and instructions for execution.

| Directory       | Description                                                                                |
|-----------------|--------------------------------------------------------------------------------------------|
| linux_utilities | Implementation of Linux-style utilities and command-line tools                             |
| system_calls    | Understanding and implementing system calls and kernel-user interactions                   |
| traps           | Exploring traps, interrupts, exceptions, and control transfer between user and kernel mode |
| locks           | Synchronisation, mutual exclusion, and concurrency control                                 |
| paging          | Virtual memory management, address translation, and paging mechanisms                      |
| cow_mmap        | Copy-on-Write (COW) memory management and memory-mapped files (mmap)                       |
| file_system     | File system implementation, inode management, large files, and symbolic links              |

## Concepts Explored

- **Processes and System Calls:** Process management, system call interfaces, and kernel-user mode transitions.
- **Concurrency and Synchronisation:** Locks, mutual exclusion, and safe concurrent execution.
- **Memory Management:** Virtual memory, paging, Copy-on-Write, and memory mapping.
- **File Systems:** Inodes, block allocation, file operations, large files, and symbolic links.
- **Kernel Internals:** Understanding and modifying core operating system components.

## Technologies & Tools

- C
- xv6 (RISC-V)
- RISC-V GNU Toolchain
- QEMU
- Linux / Unix development environment

## Repository Organisation

Each project is maintained in a separate directory to keep implementations modular and organised. Refer to the individual directory READMEs for detailed explanations, implementation notes, build instructions, and testing procedures.

---

This repository serves as a practical exploration of operating system design, connecting theoretical concepts with their implementation inside a working kernel.
