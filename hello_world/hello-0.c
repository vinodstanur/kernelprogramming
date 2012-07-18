#include <linux/module.h>
#include <linux/kernel.h>
int init_module(void)
{
	printk(KERN_ALERT, "Module Initializing...\n");
	return 0;
}
void cleanup_module(void)
{
	printk("Cleaning up...\n");
}
