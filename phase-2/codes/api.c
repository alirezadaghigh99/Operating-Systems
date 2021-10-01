#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <asm/unistd.h>
struct User
{
    char id[10];
    char userPrivacy;
};
struct File
{
    char path[50];
    char filePrivacy;
};

int main()
{
    long pointer;
    char buffer[10000];
    int offset;
    int numberOfUsers;
    int numberOfFiles;
    int i = 0;
    int id, state;
    char path[50];

    int k = 0;
    char temp[10000];
    int j = 0;

    printf("enter number of users: ");
    scanf("%d", &numberOfUsers);
    printf("enter number of files: ");
    scanf("%d", &numberOfFiles);
    struct User users[numberOfUsers];
    struct File files[numberOfFiles];
    for (i = 0; i < numberOfUsers; i++)
    {
        printf("enter id and state of %dth user: ", i + 1);
        scanf("%s %c", users[i].id, &users[i].userPrivacy);
    }
    for (i = 0; i < numberOfFiles; i++)
    {
        printf("enter path and state of %dth file: ", i + 1);
        scanf("%s %c", files[i].path, &files[i].filePrivacy);
    }

    int file_device = open("/dev/first_phase", O_RDWR);

    i = 0;
    for (j = 0; j < numberOfUsers; j++)
    {
        int x = 0;
        while (1)
        {
            while (users[j].id[x] != '\0')
            {
                temp[i] = users[j].id[x];
                i++;
                x++;
            }
            temp[i] = '%';
            i++;
            break;
        }
        temp[i] = users[j].userPrivacy;
        i++;
        temp[i] = '%';
        i++;
    }

    temp[i] = '?';
    i++;

    for (j = 0; j < numberOfFiles; j++)
    {
        int x = 0;
        while (1)
        {
            while (files[j].path[x] != '\0')
            {
                temp[i] = files[j].path[x];
                i++;
                x++;
            }
            temp[i] = '%';
            i++;
            break;
        }
        temp[i] = files[j].filePrivacy;
        i++;
        temp[i] = '%';
        i++;
    }

    temp[i] = '\0';
    FILE *fd = fopen("reader.txt", "w+");
    fprintf(fd, "%d", i);
    write(file_device, temp, i);
    for (k = 0; k < i; k++)
    {
        printf("%c", temp[k]);
    }
    printf("\n");

    syscall(__NR_open, "/home/alireza/Desktop/f/reader.txt");
}
