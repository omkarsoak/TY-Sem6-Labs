#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE1 200
#define BUFFER_SIZE2 6

int writeDataToFile() {
    char buffer[BUFFER_SIZE1];
    int i;

    // Fill buffer with 'A's and newline characters
    for (i = 0; i < BUFFER_SIZE1; i += 2) {
        buffer[i] = 'A';
        buffer[i + 1] = '\n';
    }

    // Open a new file for writing in /tmp/ folder
    int fd = open("/tmp/test_file.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    // Write BUFFER_SIZE bytes of data to it
    ssize_t bytes_written = write(fd, buffer, BUFFER_SIZE1);
    if (bytes_written != BUFFER_SIZE1) {
        perror("Error writing to file");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int openFileAndRead() {
    char buffer[BUFFER_SIZE2 + 1];  // +1 for null terminator
    int bytesRead;

    // Open the existing file for read-only
    int fd = open("/tmp/test_file.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    // Read bytes number 5 to 10
    lseek(fd, 4, SEEK_SET);  // Move to byte 5
    bytesRead = read(fd, buffer, BUFFER_SIZE2);  // Read 6 bytes (5 to 10)
    if (bytesRead != BUFFER_SIZE2) {
        perror("Error reading file");
        close(fd);
        return -1;
    }

    // Null-terminate the buffer
    buffer[BUFFER_SIZE2] = '\0';

    // Print the read bytes (optional)
    // printf("Read 5th to 10th byte: %s\n", buffer);

    // Close the file descriptor
    close(fd);
    return 0;
}

int main() {
    writeDataToFile();
    //openFileAndRead();
    return 0;
}

