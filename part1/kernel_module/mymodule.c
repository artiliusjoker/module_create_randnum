#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MINH QUAN-1712695");
MODULE_DESCRIPTION("This module creates a random number and a character device");
MODULE_VERSION("1.0.0");

// Constant
#define DEVICE_NAME "my_random"
#define MY_MINOR 0

// Important global variables
static int result = 0; // our random number
static const unsigned long int_size = sizeof(int); // size of int;

// Define functions
static ssize_t custom_write(struct file *, const char *, size_t, loff_t *);
static ssize_t custom_read(struct file *, char *, size_t, loff_t *);
static int custom_open(struct inode *, struct file *);
static int custom_release(struct inode *, struct file *);
int make_random_number(void);

// struct file operations
static const struct file_operations custom_fops =
{
	.open = custom_open,
	.read = custom_read,
	.write = custom_write,
	.release = custom_release,
};

// Dung miscdevice de co permission
struct miscdevice random_misc =
{
	.minor = MY_MINOR,
	.name = DEVICE_NAME,
	.fops = &custom_fops,
	.mode = 0666,
};

static int __init random_init(void)
{
	printk(KERN_INFO "Random number module started\n");
	result = make_random_number();
	printk(KERN_INFO "Random number created : %d\n", result);

	// Make character device file COF
	int check = 1;
	check = misc_register(&random_misc);
	if(check){
		printk(KERN_ALERT "ERROR, TRY AGAIN !\n");
		return 1;	
	}
	printk("Everything works perfectly !\n");
	return result;
}

static void __exit random_exit(void)
{
	printk("Random number module exited\n");
	misc_deregister(&random_misc);
	return;
}

static int custom_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "CLOSED");
	return 0;
}

static ssize_t custom_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	return 0;
}

static int custom_open(struct inode *inodep, struct file *filep){
	return 0;
}

static ssize_t custom_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	if(copy_to_user(buffer, &result, int_size) != 0)
	{
		printk(KERN_ALERT "Error copying !");					
	}	
	return int_size;
}

int make_random_number(void)
{
	int random = 0;
	// generate random integer
	get_random_bytes(&random, int_size-1);
	// make the number smaller	
	random = random % 150;
	return random;
}

module_init(random_init);
module_exit(random_exit);



















