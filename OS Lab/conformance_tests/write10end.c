#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 10

int main() {
    char buffer[BUFFER_SIZE];  // Buffer to hold the data to be written
    char read_buffer[BUFFER_SIZE];  // Buffer to hold the data read from the file

    // Fill the buffer with some data (for example, '1' to '9')
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = '0' + i + 1;
    }

    // Open the existing file for read-write, or create it if it doesn't exist
    int fd = open("/tmp/test_file3.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Move the file pointer to the end of the file
    if (lseek(fd, 0, SEEK_END) == -1) {
        perror("Error seeking file");
        close(fd);
        return 1;
    }

    // Write the buffer contents to the end of the file
    ssize_t bytes_written = write(fd, buffer, BUFFER_SIZE);
    if (bytes_written != BUFFER_SIZE) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }

    // Move the file pointer back to the beginning of the written data
    if (lseek(fd, -BUFFER_SIZE, SEEK_END) == -1) {
        perror("Error seeking file");
        close(fd);
        return 1;
    }

    // Read the data written to the file
    ssize_t bytes_read = read(fd, read_buffer, BUFFER_SIZE);
    if (bytes_read != BUFFER_SIZE) {
        perror("Error reading file");
        close(fd);
        return 1;
    }

    // Close the file
    close(fd);

    // Print the read data
    printf("%s\n", read_buffer);

    return 0;
}

