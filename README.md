# In-Memory Encrypted Virtual RAM-Disk Block Driver

## 1. Project Overview

This project implements a Linux-based virtual block device that provides
16 MB of RAM-backed storage.

The virtual disk is implemented as a Linux block device driver. Data written
to the virtual disk is encrypted using AES-XTS before being stored in RAM.
When data is read, it is decrypted by the driver and returned to the user.

A C++ user-space application is used to interact with and test the virtual
RAM disk.

## 2. Objectives

- Implement a Linux virtual block device.
- Store disk data in RAM instead of physical storage.
- Implement block-level read and write operations.
- Encrypt data before storing it in RAM.
- Decrypt data during read operations.
- Provide a C++ application for testing the device.
- Demonstrate interaction between user-space software and a Linux kernel driver.
- Test data integrity using single-block and multiple-block operations.

## 3. System Architecture

The project follows a user-space to kernel-space architecture.

```text
+-------------------------------+
| C++ User-Space Test App       |
| ramdisk_test.cpp              |
+---------------+---------------+
                |
                | read() / write()
                v
+-------------------------------+
| Linux Virtual Block Device    |
| /dev/virtual_ramdisk          |
+---------------+---------------+
                |
                v
+-------------------------------+
| Linux Kernel Block Driver     |
| virtual_ramdisk.c             |
+---------------+---------------+
                |
                v
+-------------------------------+
| AES-XTS Encryption Layer      |
+---------------+---------------+
                |
                v
+-------------------------------+
| In-Memory Storage             |
| 16 MB RAM                     |
+-------------------------------+

Write path:
User data → Block Driver → AES-XTS Encryption → RAM

Read path:
RAM → AES-XTS Decryption → Block Driver → User application

### Main Components

1. **Linux Kernel Driver**
   - Creates the virtual block device.
   - Handles block read/write requests.
   - Manages the RAM-backed storage.

2. **Encryption Layer**
   - Uses the Linux Kernel Crypto API.
   - Uses AES-XTS for block-level encryption.
   - Encrypts data before it is stored in RAM.
   - Decrypts data before returning it to the user.

3. **C++ User-Space Application**
   - Opens `/dev/virtual_ramdisk`.
   - Performs read/write operations.
   - Tests single-block and multiple-block data integrity.

4. **RAM Storage**
   - Provides 16 MB temporary storage.
   - Data exists only while the driver is loaded.

## 4. Requirements

### Software Requirements

- Linux environment
- WSL2 with Ubuntu
- Custom WSL2 kernel with required block-device and crypto support
- GCC
- G++
- GNU Make
- Git

### Hardware/Architecture Concepts

- RAM-backed storage
- Block device architecture
- Kernel-space and user-space interaction
- System calls
- Linux block I/O
- Memory management
- Data encryption

## 5. Build and Run

### Build the Kernel Module

```bash
cd ~/projects/virtual-ramdisk-driver/driver
make

### Verify the Virtual Disk

```bash
lsblk | grep virtual_ramdisk

### Build the C++ Test Application

```bash
cd ~/projects/virtual-ramdisk-driver/userspace
g++ -std=c++17 -Wall -Wextra ramdisk_test.cpp -o ramdisk_test

## 6. Testing and Results

### Test 1: Virtual Disk Detection

The Linux system successfully detects the 16 MB virtual RAM disk.

### Test 2: Single-Block Read/Write

- Block size: 512 bytes
- Write operation: PASS
- Read operation: PASS
- Data integrity: PASS

### Test 3: Multiple-Block Read/Write

- Multiple-block test: PASS
- Data was successfully written and read back.

### Test 4: Encryption and Decryption

- AES-XTS encryption: PASS
- Encrypted data is stored in RAM.
- Data is decrypted during read operations.
- Original plaintext was successfully recovered after read-back.

### Test Result

All implemented functional tests completed successfully.

## 7. Project Structure

```text
virtual-ramdisk-driver/
├── driver/
│   ├── virtual_ramdisk.c
│   └── Makefile
├── userspace/
│   └── ramdisk_test.cpp
├── docs/
├── diagrams/
├── tests/
├── scripts/
├── README.md
└── .gitignore

## 8. Limitations

- The virtual disk provides temporary storage in RAM.
- Data is lost when the driver is unloaded or the system is restarted.
- The encryption key is generated when the driver is initialized and is not persisted.
- AES-XTS provides confidentiality but does not provide authentication or tamper detection.
- The current implementation is intended as an educational prototype

## 9. Future Improvements

- Add a user-space control interface using ioctl.
- Add configurable RAM disk size.
- Improve concurrent request handling.
- Add more extensive automated testing.
- Add performance benchmarking.
- Improve key-management options.
- Add detailed monitoring and statistics.

## 10. Conclusion

The project demonstrates the implementation of a Linux RAM-backed virtual block device with
AES-XTS encryption and a C++ user-space test application.

It combines Linux device-driver concepts, block I/O, memory management, system programming,
computer architecture concepts, and data security in a single working prototype.
