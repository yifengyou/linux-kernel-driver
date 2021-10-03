#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
 
MODULE_DESCRIPTION("Frocheng: Driver for DEMO!");
MODULE_AUTHOR("Frodo Cheng");
MODULE_LICENSE("GPL");
MODULE_VERSION("V0.0.1");
 
#define HELLO_IRQ   (16U)
 
static char* dev = "hello";
 
static irqreturn_t irq_handle_cb(int irq, void * arg)
{
    printk("===[frocheng]===[%s]===[%s]===[%d]===[irq: %d]===\n",__FILE__, __func__, __LINE__, irq);
    return IRQ_NONE;
}
 
static int __init hello_init(void)
{
    printk("===[frocheng]===[%s]===[%s]===[%d]===[Hello !]===\n",__FILE__, __func__, __LINE__);
    irqreturn_t ret;
    ret = request_irq(HELLO_IRQ, irq_handle_cb, IRQF_SHARED, "hello", (void*)dev);
    return 0;
}
 
static void __exit hello_exit(void)
{
    free_irq(HELLO_IRQ, (void*)dev);
    printk("===[frocheng]===[%s]===[%s]===[%d]===[Bye bye...]===\n",__FILE__, __func__, __LINE__);
}
 
module_init(hello_init);
module_exit(hello_exit);
