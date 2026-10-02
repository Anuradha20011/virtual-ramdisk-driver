# UML Class / Component Diagram

## Main Components

C++ Test Application
    |
    | opens /dev/virtual_ramdisk
    v
Virtual RAM Disk Driver
    |
    +-- Block Request Handler
    |
    +-- AES-XTS Encryption
    |
    +-- RAM Storage

C++ Test Application
- Open virtual block device
- Perform read/write
- Verify data integrity

Virtual RAM Disk Driver
- Process block requests
- Manage 16 MB RAM storage
- Handle read/write operations

AES-XTS Encryption
- Encrypt data before RAM storage
- Decrypt data during read

RAM Storage
- Store encrypted blocks in memory
