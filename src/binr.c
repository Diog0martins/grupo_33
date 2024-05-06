#include "../include/defs.h"

int main() {
    int fd;
    int bytes_read;
    int offset = 0;
    Registo reg;

    // Open the binary file
    fd = open("../register", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read and print the contents of the binary file
    while ((bytes_read = read(fd, &reg, sizeof(Registo))) > 0) {
        printf("ID: %d\n", reg.id);
        printf("Args: %s\n", reg.args);
        printf("Duration: %Lf\n\n", reg.duration);

        // Move the offset to the next struct
        offset += bytes_read;
        lseek(fd, offset, SEEK_SET);
    }

    // Close the file
    close(fd);

    return 0;
}