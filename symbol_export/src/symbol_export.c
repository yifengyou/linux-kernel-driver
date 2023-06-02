// symbol_export.c: a linux kernel module to demonstrate different symbol types
#include <linux/module.h>
#include <linux/init.h>

// A function that can be called by other code
void func_T(void)
{
    printk(KERN_INFO "This is func_T\n");
}
EXPORT_SYMBOL(func_T);

// A local function that can only be used in this file
static void func_t(void)
{
    printk(KERN_INFO "This is func_t\n");
}

// A global variable that can be accessed and modified by other code
int var_D = 42;
EXPORT_SYMBOL(var_D);

// A local variable that can only be used in this file
static int var_d = 24;

// A read-only variable that cannot be modified
const int var_R = 100;
EXPORT_SYMBOL(var_R);

// A read-only local variable that can only be used in this file
static const int var_r = 200;

// An absolute variable that can be accessed and modified by other code
int var_A __attribute__((section(".data..absolute"))) = 0x1234;
EXPORT_SYMBOL(var_A);

// An absolute local variable that can only be used in this file
static int var_a __attribute__((section(".data..absolute"))) = 0x4321;

// An uninitialized global variable that is initialized to zero at startup
int var_B __attribute__((section(".bss")));
EXPORT_SYMBOL(var_B);

// An uninitialized local variable that is initialized to zero at startup
static int var_b __attribute__((section(".bss")));

// A global variable that may be modified at runtime
int var_G __attribute__((section(".data..global_data"))) = 10;
EXPORT_SYMBOL(var_G);

// A constant that cannot be modified at runtime
const int var_C __attribute__((section(".rodata..constant"))) = 20;
EXPORT_SYMBOL(var_C);

// A weak symbol that can be overridden or selected at link time
int var_W __attribute__((weak)) = 30;
EXPORT_SYMBOL(var_W);

// A module initialization function
static int __init symbol_export_init(void)
{
    printk(KERN_INFO "symbol_export module loaded\n");
    // Call the local function
    func_t();
    // Print the values of the variables
    printk(KERN_INFO "var_D = %d\n", var_D);
    printk(KERN_INFO "var_d = %d\n", var_d);
    printk(KERN_INFO "var_R = %d\n", var_R);
    printk(KERN_INFO "var_r = %d\n", var_r);
    printk(KERN_INFO "var_A = %x\n", var_A);
    printk(KERN_INFO "var_a = %x\n", var_a);
    printk(KERN_INFO "var_B = %d\n", var_B);
    printk(KERN_INFO "var_b = %d\n", var_b);
    printk(KERN_INFO "var_G = %d\n", var_G);
    printk(KERN_INFO "var_C = %d\n", var_C);
    printk(KERN_INFO "var_W = %d\n", var_W);
    return 0;
}

// A module exit function
static void __exit symbol_export_exit(void)
{
    printk(KERN_INFO "symbol_export module unloaded\n");
}

module_init(symbol_export_init);
module_exit(symbol_export_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A linux kernel module to demonstrate different symbol types");
MODULE_AUTHOR("nicyou");
