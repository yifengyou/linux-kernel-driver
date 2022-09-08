#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

// 模块加载时运行此函数
// __init 声明表示函数地址放到统一的init段中
//  __init函数在区段.initcall.init中还保存了一份函数指针，
// 在初始化时内核会通过这些函数指针调用这些__init函数指针，
// 并在整个初始化完成后，释放整个init区段，意味着初始化后丢弃内容，节省空间
// #define __init  __attribute__ ((__section__ (".init.text")))  __cold
static int __init helloworld_init(void) //模块入口点没有传入参数（void）
{
    printk(KERN_INFO "Loading Module\n");
    printk(KERN_INFO "%s  %s  %d\n", __FILE__, __func__, __LINE__);
    return 0; //模块入口点必须返回int，0：成功  !0：失败
}

//模块删除时运行此函数
// __exit 声明表示函数地址放到统一的exit段中
// #define __exit  __attribute__ (( __section__ (".exit.text"))) __cold
static void __exit helloworld_exit(void) ////模块离开点没有传入参数（void）
{
    printk(KERN_INFO "Removing Module\n");
    printk(KERN_INFO "%s  %s  %d\n", __FILE__, __func__, __LINE__);
    // 返回值void，不需要return
}

//用于注册模块入口点和出口点的宏
module_init(helloworld_init); //模块入口点
module_exit(helloworld_exit); //模块离开点

MODULE_LICENSE("GPL"); //模块证书
MODULE_DESCRIPTION("Simple Module Example"); //模块描述
MODULE_AUTHOR("nicyou"); //模块作者
