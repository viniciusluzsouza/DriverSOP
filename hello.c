#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/errno.h>

#define DEVICE_NAME "RW_MODULE"

//ssize_t (*read) (struct file *, char *, size_t, loff_t *);
//ssize_t (*write) (struct file *, const char *, size_t, loff_t *);
//int (*open) (struct inode *, struct file *);

int init_module(void);
void cleanup_module(void);

static int device_open(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int Major;
static int DeviceOpen = 0;

static char msg[80];
static char *msg_Ptr;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open
};


int init_module(void) {
	printk(KERN_INFO "Hello world\n");

	Major = register_chrdev(163, DEVICE_NAME, &fops);

	printk("Modulo inicializado com major %d", Major);

	return 0;
}

void cleanup_module(void) {
	int ret = unregister_chrdev(Major, DEVICE_NAME);
	if (ret < 0) printk("Erro %d no unregister!", ret);
	else printk(KERN_INFO "Bye world\n");
}

//=========================================================//


static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {

	int bytes_read = 0;

	if (*msg_Ptr == 0) return 0;

	while (length && *msg_Ptr) {
		
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}

	return bytes_read;

}

static int device_open(struct inode *inode, struct file *file) {

	if DeviceOpen return -1;

	DeviceOpen = 1;
	MOD_INC_USE_COUNT;

	return 0;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {

	


}
