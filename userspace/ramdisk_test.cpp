#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

const char* DEVICE = "/dev/virtual_ramdisk";
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 4;

char multi_write[BLOCK_SIZE * NUM_BLOCKS] = {};
char multi_read[BLOCK_SIZE * NUM_BLOCKS] = {};

int main() {
    int fd = open(DEVICE, O_RDWR);

    if (fd < 0) {
        perror("Failed to open virtual RAM disk");
        return 1;
    }

    cout << "Virtual RAM Disk opened successfully." << endl;

    const char* message = "C++_RAMDISK_TEST";
    char buffer[BLOCK_SIZE] = {};

    strncpy(buffer, message, strlen(message));

    ssize_t written = write(fd, buffer, BLOCK_SIZE);

    if (written != BLOCK_SIZE) {
        perror("Write failed");
        close(fd);
        return 1;
    }

    cout << "Write successful: " << written << " bytes" << endl;

    lseek(fd, 0, SEEK_SET);

    ssize_t bytes_read = read(fd, buffer, BLOCK_SIZE);

    if (bytes_read != BLOCK_SIZE) {
        perror("Read failed");
        close(fd);
        return 1;
    }

    cout << "Read successful: " << bytes_read << " bytes" << endl;
    cout << "Data: " << buffer << endl;

    cout << "\nRunning multiple-block test..." << endl;

    for (int i = 0; i < BLOCK_SIZE * NUM_BLOCKS; i++) {
        multi_write[i] = 'A' + (i % 26);
    }

    lseek(fd, 0, SEEK_SET);

    ssize_t multi_written = write(fd, multi_write, BLOCK_SIZE * NUM_BLOCKS);

    if (multi_written != BLOCK_SIZE * NUM_BLOCKS) {
        perror("Multiple-block write failed");
        close(fd);
        return 1;
    }

    lseek(fd, 0, SEEK_SET);

    ssize_t multi_bytes_read = read(fd, multi_read, BLOCK_SIZE * NUM_BLOCKS);

    if (multi_bytes_read != BLOCK_SIZE * NUM_BLOCKS) {
        perror("Multiple-block read failed");
        close(fd);
        return 1;
    }

    if (memcmp(multi_write, multi_read,
               BLOCK_SIZE * NUM_BLOCKS) == 0) {
        cout << "Multiple-block test: PASS" << endl;
    } else {
        cout << "Multiple-block test: FAIL" << endl;
    }
    close(fd);

    return 0;
}
