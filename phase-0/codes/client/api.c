#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    long pointer;
    char buffer[1];
    int offset;
    printf("which index do you want: ");
    scanf("%d", &offset);
    int file_device = open("/dev/fibonacci_device" , O_RDWR);
    if (file_device<0){
        exit(1);
    }
    int i = 0;
    printf("Fibonacci series for n = %d :\n", offset);
    while (i<offset){
        lseek(file_device,i,SEEK_SET);
        pointer = read(file_device,buffer,1);
        printf("%ld\n",pointer);
        i++;
    }
    close(file_device);
    return 0;


}
