#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    const char* source_file = "/tmp/test_file4.txt";
    const char* destination_file = "/tmp/copied_file4.txt";

    int source_fd = open(source_file, O_RDONLY);
    if (source_fd == -1) {
        perror("Error opening source file");
        return 1;
    }

    int destination_fd = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (destination_fd == -1) {
        perror("Error opening destination file");
        close(source_fd);
        return 1;
    }

    // Copy the contents of the source file to the destination file
    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_written = write(destination_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to destination file");
            close(source_fd);
            close(destination_fd);
            return 1;
        }
    }

    if (bytes_read == -1) {
        perror("Error reading from source file");
        close(source_fd);
        close(destination_fd);
        return 1;
    }

    // Close file descriptors
    close(source_fd);
    close(destination_fd);

    //printf("File copied successfully.\n");

    return 0;
}

