#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/pid.h>
#include <linux/proc_fs.h>
#include <linux/pid_namespace.h>
#include <linux/kallsyms.h>
#include <linux/uidgid.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <asm/paravirt.h>
#include <linux/time.h>

#define MODULE
#define LINUX
#define __KERNEL__
#define GFP_KERNEL (__GFP_RECLAIM | __GFP_IO | __GFP_FS)
#define CR0_PROT 0x00010000

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Alireza Nima Sina");
MODULE_DESCRIPTION("phase1");

static dev_t task_device = 0;
static struct cdev *task_cdev;
static struct class *task_class;
static int starter = 0;
struct timeval time;
unsigned long local_time;
struct tm result;
struct timespec ts;
char saveAccesses[50000];
int numOfUsers = 0;
int numOfFiles = 0;

int saveAccessSize = 0;
unsigned long *sysCallTable;
asmlinkage int (*prevOpen)(const char *, int, int);
char *stringConverter(int num)
{

    int i = 0;
    int temp = num;
    while (num > 0)
    {
        num /= 10;
        i++;
    }
    int j = 0;
    int n;
    char returnValue[i];
    for (j = i - 1; j >= 0; j--)
    {
        n = temp % 10;
        returnValue[j] = n + '0';
        temp = temp / 10;
    }
    return returnValue;
}
struct User
{
    int id;
    int userPrivacy;
} users[100];
struct File
{
    char path[50];
    int filePrivacy;
} files[100];
struct
{
    char array[1000];
} char_arr;

asmlinkage int newOpen(const char *file, int flags, int mode)
{
    uid_t userId = get_current_user()->uid.val;
    int userAccessTemp = 1;
    int fileAccessTemp = 0;

    for (int i = 0; i < numOfUsers; i++)
    {
        if (users[i].id == userId)
        {
            userAccessTemp = users[i].userPrivacy;
            break;
        }
    }
    for (int i = 0; i < numOfFiles; i++)
    {
        if (strcmp(files[i].path, file) == 0)
        {

            fileAccessTemp = files[i].filePrivacy;
            if (userId == 0)
            {
                saveAccesses[saveAccessSize] = '0';
                saveAccessSize++;
            }
            if (userId != 0)
            {

                int p = saveAccessSize;
                int l = 0;
                int num = userId;
                int temp = num;
                while (num > 0)
                {
                    num /= 10;
                    l++;
                }
                int counter = 0;
                int z;
                char uidString[l];
                for (counter = l - 1; counter >= 0; counter--)
                {
                    z = temp % 10;
                    uidString[counter] = z + '0';
                    temp = temp / 10;
                }
                for (; saveAccessSize < l + p; saveAccessSize++)
                {
                    saveAccesses[saveAccessSize] = uidString[saveAccessSize - p];
                }
            }
            saveAccesses[saveAccessSize] = ' ';
            saveAccessSize++;
            saveAccesses[saveAccessSize] = userAccessTemp + '0';
            saveAccessSize++;
            saveAccesses[saveAccessSize] = ' ';
            saveAccessSize++;
            int pf = saveAccessSize;
            for (; saveAccessSize < strlen(file) + pf; saveAccessSize++)
            {
                saveAccesses[saveAccessSize] = file[saveAccessSize - pf];
            }
            saveAccesses[saveAccessSize] = ' ';
            saveAccessSize++;
            saveAccesses[saveAccessSize] = fileAccessTemp + '0';
            saveAccessSize++;
            saveAccesses[saveAccessSize] = ' ';
            saveAccessSize++;
            char *modeString;
            if (mode == 0)
            {
                saveAccesses[saveAccessSize] = '0';
                saveAccessSize++;
            }
            else
            {

                int pm = saveAccessSize;
                int l1 = 0;
                int num1 = mode;
                int temp1 = num1;
                while (num1 > 0)
                {
                    num1 /= 10;
                    l1++;
                }
                int counter1 = 0;
                int z1;
                char modeString[l1];
                for (counter1 = l1 - 1; counter1 >= 0; counter1--)
                {
                    z1 = temp1 % 10;
                    modeString[counter1] = z1 + '0';
                    temp1 = temp1 / 10;
                }
                for (; saveAccessSize < l1 + pm; saveAccessSize++)
                {
                    saveAccesses[saveAccessSize] = modeString[saveAccessSize - pm];
                }
            }
            saveAccesses[saveAccessSize] = ' ';
            saveAccessSize++;
            getnstimeofday(&ts);
            int hour = (int)(ts.tv_sec / 3600) % 24;
            int minute = (int)(ts.tv_sec / 60) % 60;
            int seconds = (int)(ts.tv_sec % 60);
            int pt = saveAccessSize;

            int counter2 = 0;
            int tempSecond, tempHour, tempMinute;
            char hourString[2];
            char minuteString[2];
            char secondString[2];
            for (counter2 = 2 - 1; counter2 >= 0; counter2--)
            {
                tempHour = hour % 10;
                hourString[counter2] = tempHour + '0';
                hour = hour / 10;
                tempMinute = minute % 10;
                minuteString[counter2] = tempMinute + '0';
                minute = minute / 10;
                tempSecond = seconds % 10;
                secondString[counter2] = tempSecond + '0';
                seconds = seconds / 10;
            }
            for (; saveAccessSize < 2 + pt; saveAccessSize++)
            {
                saveAccesses[saveAccessSize] = hourString[saveAccessSize - pt];
            }
            saveAccesses[saveAccessSize] = ':';
            saveAccessSize++;
            pt = saveAccessSize;
            for (; saveAccessSize < 2 + pt; saveAccessSize++)
            {
                saveAccesses[saveAccessSize] = minuteString[saveAccessSize - pt];
            }
            saveAccesses[saveAccessSize] = ':';
            saveAccessSize++;
            pt = saveAccessSize;
            for (; saveAccessSize < 2 + pt; saveAccessSize++)
            {
                saveAccesses[saveAccessSize] = secondString[saveAccessSize - pt];
            }
            saveAccesses[saveAccessSize] = '\n';
            saveAccessSize++;
            break;
        }
    }

    //printk(KERN_ALERT "access of file %.2lu: %.2lu: %.2lu: %.6lu: \n", (ts.tv_sec/3600)%24,(ts.tv_sec/60)%60,(ts.tv_sec%60),ts.tv_nsec/1000);

    printk(KERN_INFO "user access is: %d and file access is: %d \n ", userAccessTemp, fileAccessTemp);

    if (fileAccessTemp == 0)
    {
        printk(KERN_INFO "user %d opens file %s as its ususal\n", userId, file);
        return prevOpen(file, flags, mode);
    }
    if (fileAccessTemp == userAccessTemp)
    {
        printk(KERN_INFO "user %d can read/write on file %s\n", userId, file);
        return prevOpen(file, 0x0002, mode);
    }
    if (fileAccessTemp < userAccessTemp)
    {
        printk(KERN_INFO "user %d can only read from file %s\n", userId, file);
        return prevOpen(file, 0x0000, mode);
    }
    printk(KERN_INFO "user %d can only write on file %s\n", userId, file);
    return prevOpen(file, 0x0001, mode);
}

static ssize_t device_read(struct file *file, char *buf, size_t size, loff_t *offset)
{
    copy_to_user(buf, saveAccesses, saveAccessSize);
    return 1;
}

static loff_t device_llseek(struct file *file, loff_t position, int whence)
{
    if (whence == 0)
    {
        if (position > 92)
            position = 92;
        if (position < 0)
            position = 0;
        file->f_pos = position;
    }
    return position;
}

static ssize_t device_write(struct file *filp, const char *buf, size_t count, loff_t *offset)
{
    unsigned long ret;
    if (count > sizeof(char_arr.array) - 1)
        return -EINVAL;
    ret = copy_from_user(char_arr.array, buf, count);
    if (ret)
        return -EFAULT;

    int i = 0;
    int j = 0;
    int x;
    while (1)
    {
        x = 0;
        users[j].id = 0;
        while (char_arr.array[i] != '%')
        {

            int c = char_arr.array[i] - '0';
            users[j].id *= 10;
            users[j].id += c;
            i++;
            x++;
        }
        i++;
        users[j].userPrivacy = char_arr.array[i] - '0';
        i += 2;
        j++;
        if (char_arr.array[i] == '?')
        {
            i++;
            break;
        }
    }
    numOfUsers = j;
    j = 0;
    while (1)
    {
        x = 0;
        while (char_arr.array[i] != '%')
        {

            files[j].path[x] = char_arr.array[i];
            i++;
            x++;
        }
        i++;
        files[j].filePrivacy = char_arr.array[i] - '0';
        i += 2;
        j++;
        if (char_arr.array[i] == '\0')
        {
            break;
        }
    }
    numOfFiles = j;
    for (int i = 0; i < numOfFiles; i++)
    {
        printk(KERN_ALERT "access of file %d \n", files[i].filePrivacy);
        printk(KERN_ALERT "file name %s", files[i].path);
    }
    sysCallTable = kallsyms_lookup_name("sys_call_table");
    prevOpen = (void *)sysCallTable[__NR_open];
    unsigned long cr0 = read_cr0();
    write_cr0(cr0 & ~CR0_PROT);
    sysCallTable[__NR_open] = (unsigned long)(&newOpen);
    write_cr0(cr0);
    return count;
}

const struct file_operations task_file_operation = {
    .owner = THIS_MODULE,
    .read = device_read,
    .llseek = device_llseek,
    .write = device_write,
};

static int task_init(void)
{
    int rc = alloc_chrdev_region(&task_device, 0, 1, "first_phase");

    if (rc < 0)
    {
        printk(KERN_ALERT "registeration failed\n");
        return rc;
    }

    task_cdev = cdev_alloc();

    if (task_cdev == NULL)
    {
        printk(KERN_ALERT "cdev allocation failed\n");
        unregister_chrdev_region(task_device, 1);
        return -1;
    }

    cdev_init(task_cdev, &task_file_operation);
    rc = cdev_add(task_cdev, task_device, 1);

    if (rc < 0)
    {
        printk(KERN_ALERT "cdev addition failed\n");
        unregister_chrdev_region(task_device, 1);
        return -1;
    }

    task_class = class_create(THIS_MODULE, "first_phase");

    if (!task_class)
    {
        printk(KERN_ALERT "class creation failed\n");
        cdev_del(task_cdev);
        unregister_chrdev_region(task_device, 1);
        return -1;
    }

    if (!device_create(task_class, NULL, task_device, NULL, "first_phase"))
    {
        printk(KERN_ALERT "Failed to create device");
        class_destroy(task_class);
        cdev_del(task_cdev);
        unregister_chrdev_region(task_device, 1);
        return -1;
    }

    printk(KERN_ALERT "module loaded!\n");
    return rc;
}

static void task_exit(void)
{
    unsigned long cr0;
    cr0 = read_cr0();
    write_cr0(cr0 & ~CR0_PROT);
    sysCallTable[__NR_open] = (unsigned long)(prevOpen);
    write_cr0(cr0);
    device_destroy(task_class, task_device);
    class_destroy(task_class);
    cdev_del(task_cdev);
    unregister_chrdev_region(task_device, 1);
    printk(KERN_ALERT "module unloaded\n");
}

module_init(task_init);
module_exit(task_exit);
