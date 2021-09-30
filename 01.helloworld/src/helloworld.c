#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

//模块加载时运行此函数
int simple_init(void)                           //模块入口点没有传入参数（void）
{
    printk(KERN_INFO "Loading Module\n");
    return 0;                                   //模块入口点必须返回int，0：成功  !0：失败
}

//模块删除时运行此函数
void simple_exit(void)                          ////模块离开点没有传入参数（void）
{
    printk(KERN_INFO "Removing Module\n");
}

//用于注册模块入口点和出口点的宏
module_init(simple_init);                       //模块入口点
module_exit(simple_exit);                       //模块离开点
MODULE_LICENSE("GPL");                          //模块证书
MODULE_DESCRIPTION("Simple Module");            //模块描述
MODULE_AUTHOR("SGG");                           //模块作者
