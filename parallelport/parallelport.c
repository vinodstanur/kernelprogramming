#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/ioport.h>
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <asm/io.h> /* inb, outb */

MODULE_LICENSE("Dual BSD/GPL");
unsigned char led=255;
static char data[100];
static int port;
int pport_open(struct inode *inode, struct file *filp); 
int pport_close(struct inode *inode, struct file *filp); 
ssize_t pport_read(struct file *filp, char *buf, size_t count, loff_t *f_pos); 
ssize_t pport_write(struct file *filp, char *buf, size_t count, loff_t *f_pos); 


static struct file_operations fops = {
	open: pport_open,
	read: pport_read,
	write: pport_write,
	release: pport_close
};



int pport_open(struct inode *inode, struct file *filp)
{
	return 0;
}

int pport_close(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t pport_write(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	copy_from_user(data, buf, 100);
	data[count+1] = 0;
	outb((led = ~led), 0x378);
	printk(KERN_ALERT "string = %s\n", data);
	return count;
}

ssize_t pport_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	copy_to_user(buf, data, 32768);
	return 32768;
}

static int major = 61;

int init_module(void)
{
	int a;
	a = register_chrdev(major, "registered_pport_61", &fops);
	if(a < 0) {
		printk(KERN_ALERT "error: can't register major number %d\n", major);
		return a;
	}
	port = check_region(0x378, 1);
	if(port) printk(KERN_ALERT "pport cannot reserve 0x378\n");
	request_region(0x378, 1, "registered_pport_61");
	return 0;
}

void cleanup_module(void)
{
	printk("pport module is going to terminate\n");
	unregister_chrdev(major, "registered_pport_61");
}

