#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/device.h>

struct sb_device {
    char* name;
    struct device dev;
};

struct sb_driver {
    struct device_driver driver;
};
