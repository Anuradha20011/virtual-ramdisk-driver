## In-Memory Encrypted Virtual RAM-Disk Block Driver

## 1. Project Overview

It is a small virtual RAM disk based on Linux which has been implemented as a block device driver.
The driver creates a 16 MB storage area in RAM and exposes it as:
/dev/virtual_ramdisk
Upon writing data to the device, the driver encrypts each 512-byte block with AES-XTS and then stores it in RAM; when the data is subsequently read, the driver decrypts it and forwards the original data to the user-space program.
The program written in C++17 is used to test the device and to verify that the data written to the RAM disk can be read back correctly.

## 2. Main Objectives

- Make a Linux virtual block device.
Use the RAM for temporary storage rather than the physical disk.
- Deal with read and write requests at the block level.
- Before data is stored in RAM it should be encrypted.
- When reading the data, decrypt it.
- Use the AES-XTS function of the Linux Kernel Crypto API.
Check the device with a C++17 application.
- Check both single-block and multiple-block operations.

## 3. Explanation of how the project works

The project has two main parts:
User Space
The C++ test program opens /dev/virtual_ramdisk and uses the usual read() and write() system calls.
Kernel Space
The Linux block layer and blk-mq pass on the block requests to the Linux kernel driver; and for a write request it takes the data, encrypts it, and then stores the encrypted block in the RAM-backed storage.
When handling a read request, it takes the encrypted block from RAM, decrypts it, and then returns the decrypted data.
Data Flow
Write:
C++ application → /dev/virtual_ramdisk → blk-mq → driver → AES-XTS encryption → RAM
Read:
RAM → AES-XTS decryption → driver → /dev/virtual_ramdisk → C++ application

## 4. Technology Used

- Linux / WSL2
- Linux Kernel
- Linux block-device driver
- blk-mq
- C
- C++17
- Linux Kernel Crypto API
- AES-XTS
- GNU Make
- Git / GitHub
- x86-64 architecture
The kernel driver is written in C since the code for the Linux kernel and device drivers makes use of the kernel's C interfaces, and the user-space test program is written in C++17.

## 5. Project Structure

virtual-ramdisk-driver/
├── driver/
│   ├── virtual_ramdisk.c
│   └── Makefile
├── userspace/
│   └── ramdisk_test.cpp
├── docs/
│   ├── development_plan.md
│   ├── requirements.md
│   └── test_plan.md
├── diagrams/
│   ├── architecture.md
│   ├── class_diagram.md
│   └── sequence_diagram.md
├── README.md
└── .gitignore

## 6. Requirements

The project needs a Linux environment with:
- Linux / WSL2
- A kernel development environment
- Required block-device and crypto support
- GCC
- G++
- GNU Make
- Git
The development environment makes use of a custom WSL2 kernel.

## 7. Build and Run

Step 1: Build the kernel module
cd ~/projects/virtual-ramdisk-driver/driver
make
This should generate:
virtual_ramdisk.ko
Step 2: Load the driver
sudo insmod ./virtual_ramdisk.ko
Step 3: Check the virtual disk
lsblk | grep virtual_ramdisk
The device must appear as a block device of 16 MB.
You can also check the device file:
ls -l /dev/virtual_ramdisk
Step 4: Build the C++ test program
cd ~/projects/virtual-ramdisk-driver/userspace
g++ -std=c++17 -Wall -Wextra ramdisk_test.cpp -o ramdisk_test
Step 5: Run the test
sudo ./ramdisk_test

## 8. Testing

The current test program checks:
Single-block test
A 512-byte block is written to the virtual disk and then read back; the data returned is compared with the data that was written.
Multiple-block test
The program consisting of the test writes four blocks of data and then reads them back; memcmp() is used to check that the data has not changed after the complete journey.
The current test output is:
The virtual RAM disk has been successfully opened.
Write successful: 512 bytes
Read successful: 512 bytes
Data: C++_RAMDISK_TEST

Running multiple-block test...
Multiple-block test: PASS
The test plan for the project also includes the successful compilation of the module, the loading of the driver, the creation of the virtual device, testing with a single block, testing with multiple blocks, and traversal of the encryption/decryption path.

## 9. Encryption

The driver makes use of the Linux Kernel Crypto API with AES-XTS.
When the module is initialized a 64-byte key is generated and, for each 512-byte sector, the sector-based value is used as the XTS tweak.
The important part of the write path is:
plaintext → AES-XTS encryption → encrypted data → RAM
The read path is the reverse:
encrypted data in RAM → AES-XTS decryption → plaintext
The encryption key is created when the module is initialized and is not saved.

## 10. Limitations

This is an educational prototype, so it has some limitations:
– The storage is volatile since the data is lost whenever the driver is unloaded or the system is restarted.
- The encryption key is not stored.
- AES-XTS ensures confidentiality but does not offer authenticated integrity or the ability to detect tampering.
- The size of the storage is now set at 16 MB.
The current setup handles requests in a simple way. It is not meant to be a storage driver for production use.
- There is some extra time required due to encryption and the processing carried out by the kernel driver.

## 11. Future Improvements

Possible improvements include:
- Configurable RAM-disk size.
- There will be more comprehensive automated tests.
- Performance benchmarking.
Better handling of requests at the same time.
- There is a control interface in user space which uses ioctl.
- Improved key-management options.
- Monitoring of runtime and statistics.

## 12. Development Plan

Over the course of the 20-day training period, the project was developed in a step-by-step manner.
The work covered:
1. Linux and WSL environment setup.
2. A basic introduction to the Linux kernel and device drivers.
3. Virtual RAM-disk implementation.
4. blk-mq request processing.
5. AES-XTS encryption and decryption.
6. C++ user-space testing.
7. Documentation and validation.
8. Final demonstration.
The detailed plan is available in:
docs/development_plan.md

## 13. Documentation

Additional project documentation:
- docs/requirements.md. Project requirements and scope.
- docs/development_plan.md, 20-day development plan.
- docs/test_plan.md. Test cases and results.
— diagrams/architecture.md — the system architecture.
- diagrams/class_diagram.md, component or class view.
- diagrams/sequence_diagram.md, read and write sequence.

## 14. Conclusion

The project shows how a block-device driver for the Linux kernel can be used to provide temporary storage by using RAM rather than a physical disk.
The project also illustrates the way that C++ code running in user space communicates with a driver in kernel space and shows how AES-XTS can be included in the data path before the blocks are stored in RAM.
One project brings together Linux device-driver concepts, block I/O, memory management, C/C++ system programming, and basic data-security concepts.
