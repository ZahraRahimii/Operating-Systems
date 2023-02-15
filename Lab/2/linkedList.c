#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday{
	int day;
	int month;
	int year;
	struct list_head list;
};

struct list_head birthday_list;

struct birthday* create_birthday(int day, int month, int year){
	struct birthday *person;
	person = kmalloc(sizeof(person), GFP_KERNEL);
	person -> day = day;
	person -> month = month;
	person -> year = year;
	return person;
}

int birthday_init(void){
	struct birthday *p1, *p2, *p3, *p4, *p5, *p6;
	p1 = create_birthday(2, 8, 1995);
	p2 = create_birthday(12, 3, 1996);
	p3 = create_birthday(15, 10, 1997);
	p4 = create_birthday(27, 12, 2000);
	p5 = create_birthday(22, 9, 2001);
	p6 = create_birthday(23, 9, 2001);

	INIT_LIST_HEAD(&birthday_list);

	list_add_tail(&p1 -> list, &birthday_list);
	list_add_tail(&p2 -> list, &birthday_list);
	list_add_tail(&p3 -> list, &birthday_list);
	list_add_tail(&p4 -> list, &birthday_list);
	list_add_tail(&p5 -> list, &birthday_list);


	struct birthday *ptr;
	int i = 1;
	list_for_each_entry(ptr, &birthday_list, list){
		printk(KERN_INFO "Birthday %d : %d/%d/%d", i, ptr->year, ptr->month, ptr->day);
		i++;
	}
	return 0;
}	

void birthday_exit(void){
	struct birthday *ptr;
	struct list_head *pos, *n;

	if (list_empty(&birthday_list)){
		printk(KERN_INFO "List is empty\n");
	} else {
		int i = 1;
		list_for_each_safe(pos, n, &birthday_list) {
			ptr = list_entry(pos, struct birthday, list);
			printk(KERN_INFO "Removing Birthday %d : %d/%d/%d", i, ptr->year, ptr->month, ptr->day);
			list_del(pos);
			kfree(ptr);
			i++;
		}
		printk(KERN_INFO "Removing is done\n");
	}
}

module_init(birthday_init);
module_exit(birthday_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zahra Rahimi");
MODULE_DESCRIPTION("Register and Delete Birthdays");

	
