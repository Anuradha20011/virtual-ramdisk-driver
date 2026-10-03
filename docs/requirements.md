# Project Requirements

## 1. Project Title

In-Memory Encrypted Virtual RAM-Disk Block Driver

## 2. Project Overview

The project implements a Linux-based virtual block device that provides
16 MB of RAM-backed storage.

The virtual disk is implemented as a Linux block device driver. Data written
to the virtual disk is encrypted using AES-XTS before being stored in RAM.
During read operations, the encrypted data is decrypted before being returned
to the user-space application.

A C++ user-space application is used to test read and write operations.

## 3. Functional Requirements

### FR1: Virtual Block Device

The system shall create a virtual block device accessible through:

/dev/virtual_ramdisk

### FR2: RAM-Based Storage

The system shall provide 16 MB of storage using system RAM instead of
physical disk storage.

### FR3: Block-Level Write

The driver shall process block-level write requests and store the
corresponding data in RAM after encryption.

### FR4: Block-Level Read

The driver shall process block-level read requests and return decrypted
data to the user-space application.

### FR5: Data Encryption

The driver shall encrypt data using the Linux Kernel Crypto API with
AES-XTS before storing it in RAM.

### FR6: Data Decryption

The driver shall decrypt stored encrypted data during read operations.

### FR7: User-Space Testing

The project shall provide a C++ application for testing the virtual
block device.

### FR8: Multi-Block Testing

The test application shall verify read and write operations across
multiple blocks.

## 4. Non-Functional Requirements

### NFR1: Operating System

The project shall run on a Linux-based environment.

### NFR2: Programming Languages

Kernel/device-driver implementation shall use C.

User-space testing shall use C++.

### NFR3: Reliability

Read data shall match the corresponding data previously written to
the virtual disk.

### NFR4: Security

Data shall be encrypted before being stored in RAM.

### NFR5: Performance

The RAM-backed storage should provide low-latency access compared with
traditional physical storage, subject to encryption and driver overhead.

### NFR6: Maintainability

The project shall use a modular structure separating the kernel driver,
user-space application, and documentation.

## 5. Hardware and Software Requirements

### Hardware

- x86-64 compatible computer
- Minimum 4 GB RAM
- Sufficient free RAM for the 16 MB virtual disk and development tools

### Software

- Linux / WSL2 Linux environment
- Linux kernel development environment
- GCC
- G++
- GNU Make
- Git
- Linux Kernel Crypto API support

## 6. Project Scope

### Included

- Linux virtual block device
- 16 MB RAM-backed storage
- Block-level read and write operations
- AES-XTS encryption
- C++ user-space testing
- Multi-block testing
- GitHub documentation

### Not Included

- Persistent storage after reboot
- User-configurable storage size
- Network storage
- Full disk encryption
- Cryptographic authentication/integrity protection

## 7. Expected Outcome

The completed system shall provide a functional encrypted virtual RAM disk
that can be accessed through a Linux block-device interface and verified
using the C++ user-space test application.
