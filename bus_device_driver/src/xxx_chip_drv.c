#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/device.h>
#include "simple_bus.h"


extern int register_sb_driver(struct sb_driver *sdrv);
extern void unregister_sb_driver(struct sb_driver *sdrv);

static struct sb_driver sculld_driver = {
	.driver = {
		.name = "chip",
	},
};

int xxx_chipdrv_init(void)
{
	return register_sb_driver(&sculld_driver);
}

void xxx_chipdrv_cleanup(void)
{
	unregister_sb_driver(&sculld_driver);
}

module_init(xxx_chipdrv_init);
module_exit(xxx_chipdrv_cleanup);
MODULE_LICENSE("GPL");
