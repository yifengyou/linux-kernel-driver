#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init zero_page_init(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Loading zero_page Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
    return 0;
}

static void __exit zero_page_exit(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Removing zero_page Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
}

module_init(zero_page_init);
module_exit(zero_page_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("zero_page Module");
MODULE_AUTHOR("nicyou");
