#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/desc.h>

#define store_gdt(ptr) asm volatile("sgdt %0":"=m" (*ptr))
#define store_ldt(ptr) asm volatile("sldt %0":"=m" (*ptr));


static int __init simple_init(void)                           //模块入口点没有传入参数（void）
{

	struct desc_ptr gdtptr; // gdt base addr and size
	struct desc_struct *gdt; // gdt entry object
	struct desc_ptr ldtptr; // gdt base addr and size
	struct desc_struct *ldt; // gdt entry object
	int i;

	store_gdt(&gdtptr); // get gdt base and size
	store_ldt(&ldtptr); // get gdt base and size

	printk("\ngdt size %d\n", gdtptr.size);
	printk("\ngdt address %lx\n", gdtptr.address);

	gdt = (struct desc_struct *)gdtptr.address;

	for (i = 0; i < gdtptr.size; i++) {
		/*
		 * * Access segment descriptor fields
		 * */
		gdt++;
	}
	//unsigned long *j;
	//asm("sgdt %0" : "=r"(j));
	//asm ("sgdt %0":"=m" (*j));
	//printk("%ld\n", j);
    printk(KERN_INFO "Loading Module\n");
	printk("%s  %s  %d\n",__FILE__,__func__,__LINE__);
    return 0;                                   //模块入口点必须返回int，0：成功  !0：失败
}

static void __exit simple_exit(void)                          ////模块离开点没有传入参数（void）
{
    printk(KERN_INFO "Removing Module\n");
	printk("%s  %s  %d\n",__FILE__,__func__,__LINE__);
	// 返回值void，不需要return
}

module_init(simple_init);                       //模块入口点
module_exit(simple_exit);                       //模块离开点

MODULE_LICENSE("GPL");                          //模块证书
MODULE_DESCRIPTION("Simple Module Example");            //模块描述
MODULE_AUTHOR("nicyou");                           //模块作者
