ENTRY := zero_page
obj-m := $(ENTRY).o
KERNEL_VER = $(shell uname -r)
default: notfound_build
  
force_build: zero_page.c
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules
	ls -alh *.ko

notfound_build: zero_page.c
	[ -f *.ko ] || make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules

build: force_build

clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean

insmod: info notfound_build
	dmesg --clear
	insmod zero_page.ko || true
	dmesg

rmmod:
	rmmod zero_page && dmesg

lsmod:
	lsmod |grep zero_page

status: lsmod

info: notfound_build zero_page.ko
	modinfo zero_page.ko
	md5sum zero_page.ko

modinfo: info

help:
	@echo " build    - build module(default target)"
	@echo " clean    - clean build dir"
	@echo " insmod   - insmod zero_page ko module"
	@echo " rmmod    - rmmod zero_page ko module"
	@echo " lsmod    - find zero_page ko module whether already insmod"
	@echo " status   - same as lsmod"
	@echo " info     - display zero_page ko info"
	@echo " modinfo  - same as info"
	@echo " help     - display help info"

