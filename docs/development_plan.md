# Development Plan

## 1. Development Approach

The project was developed incrementally during the 20-day training period.
The implementation was divided into multiple stages covering Linux,
computer architecture, system programming, block-device drivers, encryption,
C++ testing, documentation, and final validation.

## 2. 20-Day Development Timeline

### Phase 1: Days 1–3 — Environment and Fundamentals

- Set up the Linux/WSL development environment.
- Study Linux system architecture and kernel space vs user space.
- Review hardware and software interaction.
- Study memory, CPU, storage, and I/O concepts.
- Prepare the kernel development environment.

### Phase 2: Days 4–6 — Linux Kernel and Driver Basics

- Study Linux kernel modules.
- Understand module loading and unloading.
- Study Linux device-driver concepts.
- Understand block devices and block-level I/O.
- Study Linux block layer and request processing.

### Phase 3: Days 7–10 — Virtual RAM-Disk Implementation

- Design the virtual RAM-disk architecture.
- Allocate memory for the 16 MB storage area.
- Implement the virtual block-device interface.
- Implement block-level read operations.
- Implement block-level write operations.
- Register the virtual disk with the Linux block layer.

### Phase 4: Days 11–13 — Block Request Processing

- Integrate blk-mq request processing.
- Process read and write requests.
- Map block requests to RAM storage.
- Validate sector and block handling.
- Test basic read/write functionality.

### Phase 5: Days 14–16 — Data Encryption

- Study Linux Kernel Crypto API.
- Integrate AES-XTS encryption.
- Generate an encryption key during module initialization.
- Encrypt data before storing it in RAM.
- Decrypt data during read operations.
- Validate encrypted read/write functionality.

### Phase 6: Days 17–18 — C++ User-Space Testing

- Develop the C++ user-space test application.
- Open the virtual block device.
- Perform single-block read/write tests.
- Perform multiple-block tests.
- Verify that read data matches written data.

### Phase 7: Day 19 — Documentation and Validation

- Prepare project README.
- Prepare system architecture documentation.
- Prepare UML/component diagrams.
- Prepare project requirements documentation.
- Review source-code organization.
- Perform final functional validation.

### Phase 8: Day 20 — Final Demonstration

- Build the kernel module.
- Load the virtual RAM-disk driver.
- Verify the `/dev/virtual_ramdisk` device.
- Execute the C++ test application.
- Demonstrate encrypted storage workflow.
- Verify test results.
- Prepare the final GitHub repository and project presentation.

## 3. Development Milestones

### Milestone 1 — Environment Setup

Linux/WSL environment and kernel development environment successfully
configured.

### Milestone 2 — Virtual RAM Disk

A functional 16 MB Linux virtual block device was implemented.

### Milestone 3 — Encryption

AES-XTS encryption and decryption were integrated into the block-device
data path.

### Milestone 4 — Testing

A C++ user-space application was implemented to verify single-block and
multi-block operations.

### Final Milestone — Capstone Completion

The complete project was documented, tested, organized on GitHub, and
prepared for final demonstration.

## 4. Development Deliverables

- Linux kernel block-device driver
- 16 MB RAM-backed virtual storage
- AES-XTS encryption support
- C++ user-space test application
- Multi-block testing
- System architecture documentation
- UML/component diagrams
- Project requirements document
- README documentation
- GitHub repository

## 5. Final Validation

The final system is validated through:

1. Successful kernel-module compilation.
2. Successful driver loading.
3. Creation of `/dev/virtual_ramdisk`.
4. Successful single-block read/write test.
5. Successful multi-block test.
6. Successful encryption/decryption path.
7. Successful C++ user-space verification.
8. GitHub repository documentation review.
