#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

int hello_init(void){
	printk(KERN_INFO "Loading module\n");
	return 0;
}

void hello_exit(void){
	printk(KERN_INFO "Removing module\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zahra Rahimi");
MODULE_DESCRIPTION("To say hello ...");

