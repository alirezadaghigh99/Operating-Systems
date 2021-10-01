#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
int main()
{
    FILE *fd = fopen("reader.txt", "r");
    int num;
    fscanf(fd, "%d", &num);
    int file_device = open("/dev/first_phase", O_RDONLY);
    lseek(file_device, num, SEEK_SET);
    char buffer[50000];
    long pointer = read(file_device, buffer, 50000);
    FILE *fa = fopen("listAccess.txt", "w+");
    fprintf(fa, "%s", buffer);
}
