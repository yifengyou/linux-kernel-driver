#!/bin/bash

if [ $# -ne 1 ]
then
	echo "gen.sh need one parameter, please specific kernel module name."
	echo " begin with {a-z、A-z、_} , may contain 0-9 , make it sense."
	exit 1
fi

[ ! -d src ] && mkdir src

if [ -f src/Makefile ]
then
	echo "src/Makefile already exists! clean or backup it at first!"
	exit 1
fi

cat > src/Makefile << EOF
ENTRY := vaddr_to_paddr
obj-m := \$(ENTRY).o
KERNEL_VER = \$(shell uname -r)
default: notfound_build
  
force_build: vaddr_to_paddr.c
	make -C /lib/modules/\$(KERNEL_VER)/build M=\$(PWD) modules
	ls -alh *.ko

notfound_build: vaddr_to_paddr.c
	[ -f *.ko ] || make -C /lib/modules/\$(KERNEL_VER)/build M=\$(PWD) modules

build: force_build

clean:
	make -C /lib/modules/\$(KERNEL_VER)/build M=\$(PWD) clean

insmod: info notfound_build
	dmesg --clear
	insmod vaddr_to_paddr.ko || true
	dmesg

rmmod:
	rmmod vaddr_to_paddr && dmesg

lsmod:
	lsmod |grep vaddr_to_paddr

status: lsmod

info: notfound_build vaddr_to_paddr.ko
	modinfo vaddr_to_paddr.ko
	md5sum vaddr_to_paddr.ko

modinfo: info

help:
	@echo " build    - build module(default target)"
	@echo " clean    - clean build dir"
	@echo " insmod   - insmod vaddr_to_paddr ko module"
	@echo " rmmod    - rmmod vaddr_to_paddr ko module"
	@echo " lsmod    - find vaddr_to_paddr ko module whether already insmod"
	@echo " status   - same as lsmod"
	@echo " info     - display vaddr_to_paddr ko info"
	@echo " modinfo  - same as info"
	@echo " help     - display help info"

EOF

if [ -f "src/$1" ]
then
	echo "src/$1 already exists! clean or backup it at first!"
	exit 1
fi

cat > "src/$1.c" << EOF
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init vaddr_to_paddr_init(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Loading vaddr_to_paddr Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
    return 0;
}

static void __exit vaddr_to_paddr_exit(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Removing vaddr_to_paddr Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
}

module_init(vaddr_to_paddr_init);
module_exit(vaddr_to_paddr_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("vaddr_to_paddr Module");
MODULE_AUTHOR("nicyou");
EOF


if [ -f README.md ]
then
	echo "README.md already exists! clean or backup it at first!"
	exit 1
fi

cat > README.md << EOF
# vaddr_to_paddr kernel module

* 功能： 待录入
* 创建时间： TIMESTAMP

---




---

EOF

TIMESTAMP=`date +"%Y-%m-%d %H:%M:%S"`

sed -i "s/vaddr_to_paddr/$1/g" src/Makefile
sed -i "s/vaddr_to_paddr/$1/g" "src/$1.c"
sed -i "s/vaddr_to_paddr/$1/g" README.md
sed -i "s/TIMESTAMP/${TIMESTAMP}/g" README.md

[ -f gen.sh ] && rm -f gen.sh

echo "All done!"
