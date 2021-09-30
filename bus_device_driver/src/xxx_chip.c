#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/slab.h>
#include "simple_bus.h"

extern int register_sb_device(struct sb_device *sbdev);
extern void unregister_sb_device(struct sb_device *sbdev);

struct xxx_chip {
	char devname[20];
	struct sb_device sdev;
};

int chipdev_num = 4;
struct xxx_chip *chipdev;

static void chip_register_dev(struct xxx_chip *dev, int index)
{
	snprintf(dev->devname, sizeof(dev->devname), "chip%d", index);
	dev->sdev.name = dev->devname;
	dev_set_drvdata(&dev->sdev.dev, dev);
	register_sb_device(&dev->sdev);
}

int chip_init(void)
{
	int i;

	chipdev = kmalloc(chipdev_num*sizeof (struct xxx_chip), GFP_KERNEL);

	memset(chipdev, 0, chipdev_num*sizeof (struct xxx_chip));
	for (i = 0; i < chipdev_num; i++) {
		chip_register_dev(chipdev + i, i);
	}

	return 0;
}

void chip_cleanup(void)
{
	int i;
	for (i = 0; i < chipdev_num; i++) {
		unregister_sb_device(&chipdev[i].sdev);
	}
	kfree(chipdev);
}

module_init(chip_init);
module_exit(chip_cleanup);

MODULE_LICENSE("GPL");
