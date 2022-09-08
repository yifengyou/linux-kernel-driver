#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init pfn_page_init(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Loading pfn_page Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
    return 0;
}

static void __exit pfn_page_exit(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Removing pfn_page Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
}

module_init(pfn_page_init);
module_exit(pfn_page_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("pfn_page Module");
MODULE_AUTHOR("nicyou");
