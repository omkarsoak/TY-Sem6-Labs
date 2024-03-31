#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 6

int main() {
    char buffer[BUFFER_SIZE + 1];  // +1 for null terminator
    int bytesRead;

    // Open the existing file for read-only
    int fd = open("/tmp/test_file2.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Seek to byte 5
    off_t offset = lseek(fd, 4, SEEK_SET);
    if (offset == -1) {
        perror("Error seeking file");
        close(fd);
        return 1;
    }

    // Read bytes number 5 to 10
    bytesRead = read(fd, buffer, BUFFER_SIZE);  // Read 6 bytes (5 to 10)
    if (bytesRead != BUFFER_SIZE) {
        perror("Error reading file");
        close(fd);
        return 1;
    }

    // Null-terminate the buffer
    buffer[BUFFER_SIZE] = '\0';

    // Print the read bytes
    printf("%s", buffer);

    // Close the file descriptor
    close(fd);
    return 0;
}

