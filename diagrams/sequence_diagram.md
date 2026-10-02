# UML Sequence Diagram

## Write Operation

```text
C++ Test Application
        |
        | write()
        v
Linux Virtual Block Device
        |
        | Block Request
        v
blk-mq Request Handler
        |
        | Process Sector
        v
AES-XTS Encryption
        |
        | Encrypted Block
        v
RAM-Backed Storage
        |
        | Store encrypted data
        v
       RAM


## Read Operation

       RAM
        |
        | Encrypted Block
        v
RAM-Backed Storage
        |
        v
AES-XTS Decryption
        |
        | Decrypted Block
        v
blk-mq Request Handler
        |
        v
Linux Virtual Block Device
        |
        | read()
        v
C++ Test Application

## Complete Data Flow

WRITE:
Application → Block Device → blk-mq → AES-XTS Encrypt → RAM

READ:
RAM → AES-XTS Decrypt → blk-mq → Block Device → Application
