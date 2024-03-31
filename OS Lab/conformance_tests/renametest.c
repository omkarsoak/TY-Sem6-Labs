#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    const char* old_name = "/tmp/test_file5.txt";
    const char* new_name = "/tmp/renamed_file5.txt";

    if (rename(old_name, new_name) == -1) {
        perror("Error renaming file");
        return 1;
    }

    //printf("File renamed successfully.\n");

    return 0;
}

