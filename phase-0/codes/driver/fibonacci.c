#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>

#define MODULE
#define LINUX
#define __KERNEL__

MODULE_LICENSE("GPL");

static dev_t fibo_device = 0;
static struct cdev *fibo_cdev;
static struct class *fibo_class;

static ssize_t fibo_read(struct file *file, char *user_buffer, size_t size, loff_t *offset){
    long long n = *offset;
    int result[n + 1];
	
    result[0] = 1;
    result[1] = 1;

    for(int i = 2; i <= n; i++){
	result[i] = result[i - 1] + result[i - 2];
    }
    return result[n];
}

static loff_t fibo_llseek(struct file *file, loff_t position, int whence)
{
    if (whence == 0) {
	if (position > 92)
            position = 92;
    	if (position < 0)
            position = 0;        
    	file->f_pos = position;
    }  
    return position;
}

const struct file_operations fibo_file_operation = {
    .owner = THIS_MODULE,
    .read = fibo_read,
    .llseek = fibo_llseek,
};

static int fibo_init(void){
    int rc = alloc_chrdev_region(&fibo_device, 0, 1, "fibonacci_device");

    if (rc < 0) {
        printk(KERN_ALERT "registeration failed\n");
        return rc;
    }

    fibo_cdev = cdev_alloc();

    if (fibo_cdev == NULL) {
        printk(KERN_ALERT "cdev allocation failed\n");
        unregister_chrdev_region(fibo_device, 1);
	return -1;
    }

    cdev_init(fibo_cdev, &fibo_file_operation);
    rc = cdev_add(fibo_cdev, fibo_device, 1);

    if (rc < 0) {
        printk(KERN_ALERT "cdev addition failed\n");
        unregister_chrdev_region(fibo_device, 1);
	return -1;
    }

   fibo_class = class_create(THIS_MODULE, "fibonacci_device");

    if (!fibo_class) {
        printk(KERN_ALERT "class creation failed\n");
    	cdev_del(fibo_cdev);
        unregister_chrdev_region(fibo_device, 1);
	return -1;
    }

    if (!device_create(fibo_class, NULL, fibo_device, NULL, "fibonacci_device")) {
        printk(KERN_ALERT "Failed to create device");
    	class_destroy(fibo_class);
        cdev_del(fibo_cdev);
        unregister_chrdev_region(fibo_device, 1);
	return -1;
    }

    printk(KERN_ALERT "module loaded!\n");
    return rc;
}

static void fibo_exit(void) {
    device_destroy(fibo_class, fibo_device);
    class_destroy(fibo_class);
    cdev_del(fibo_cdev);
    unregister_chrdev_region(fibo_device, 1);
    printk(KERN_ALERT "module unloaded\n");
}

module_init(fibo_init);
module_exit(fibo_exit);
