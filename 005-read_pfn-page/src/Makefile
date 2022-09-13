ENTRY := pfn_page
obj-m := $(ENTRY).o
KERNEL_VER = $(shell uname -r)
default: notfound_build
  
force_build: pfn_page.c
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules
	ls -alh *.ko

notfound_build: pfn_page.c
	[ -f *.ko ] || make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules

build: force_build

clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean

insmod: info notfound_build
	dmesg --clear
	insmod pfn_page.ko || true
	dmesg

rmmod:
	rmmod pfn_page && dmesg

lsmod:
	lsmod |grep pfn_page

status: lsmod

info: notfound_build pfn_page.ko
	modinfo pfn_page.ko
	md5sum pfn_page.ko

modinfo: info

help:
	@echo " build    - build module(default target)"
	@echo " clean    - clean build dir"
	@echo " insmod   - insmod pfn_page ko module"
	@echo " rmmod    - rmmod pfn_page ko module"
	@echo " lsmod    - find pfn_page ko module whether already insmod"
	@echo " status   - same as lsmod"
	@echo " info     - display pfn_page ko info"
	@echo " modinfo  - same as info"
	@echo " help     - display help info"

