#include "simple_bus.h"

char *Version = "0.0.1";

static int sb_match(struct device *dev, struct device_driver *driver)
{
    return !strncmp(dev_name(dev), driver->name, strlen(driver->name));
}

struct bus_type sb_bus_type = {
    .name = "sb",
    .match = sb_match,
};

static ssize_t version_show(struct bus_type *bus, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "%s\n", Version);
}

static BUS_ATTR_RO(version);

static void sb_dev_release(struct device *dev)
{ }

int register_sb_device(struct sb_device *sbdev)
{
    sbdev->dev.bus = &sb_bus_type;
    sbdev->dev.release = sb_dev_release;
    dev_set_name(&sbdev->dev, sbdev->name);
    return device_register(&sbdev->dev);
}
EXPORT_SYMBOL(register_sb_device);

void unregister_sb_device(struct sb_device *sbdev)
{
    device_unregister(&sbdev->dev);
}
EXPORT_SYMBOL(unregister_sb_device);

static int sb_drv_probe(struct device *dev)
{
    printk(KERN_INFO"sb_drv probe %s\n", dev_name(dev));
    return 0;
}

int register_sb_driver(struct sb_driver *sdrv)
{
    sdrv->driver.bus = &sb_bus_type;
    sdrv->driver.probe = &sb_drv_probe;
    return driver_register(&sdrv->driver);
}
EXPORT_SYMBOL(register_sb_driver);

void unregister_sb_driver(struct sb_driver *driver)
{
    driver_unregister(&driver->driver);
}
EXPORT_SYMBOL(unregister_sb_driver);

static int __init sb_bus_init(void)
{
    int ret;
    printk("sb_bus_init");
    ret = bus_register(&sb_bus_type);
    if (ret) {
        printk(KERN_ERR "Unable to register sb bus, failure was %d\n",ret);
        return ret;
    }
    if (bus_create_file(&sb_bus_type, &bus_attr_version))
        printk(KERN_ERR "Unable to create version attribute\n");
    return 0;
}

static void sb_bus_exit(void)
{
    bus_unregister(&sb_bus_type);
}

module_init(sb_bus_init);
module_exit(sb_bus_exit);

MODULE_LICENSE("GPL");
