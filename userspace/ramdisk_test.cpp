#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

const char* DEVICE = "/dev/virtual_ramdisk";
const int BLOCK_SIZE = 512;

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

    close(fd);

    return 0;
}
