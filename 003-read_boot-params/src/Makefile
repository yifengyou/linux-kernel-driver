ENTRY := boot_params
obj-m := $(ENTRY).o
KERNEL_VER = $(shell uname -r)
default: notfound_build
  
force_build: boot_params.c
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules
	ls -alh *.ko

notfound_build: boot_params.c
	[ -f *.ko ] || make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules

build: force_build

clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean

insmod: info notfound_build
	dmesg --clear
	insmod boot_params.ko || true
	dmesg

rmmod:
	rmmod boot_params && dmesg

lsmod:
	lsmod |grep boot_params

status: lsmod

info: notfound_build boot_params.ko
	modinfo boot_params.ko
	md5sum boot_params.ko

modinfo: info

help:
	@echo " build    - build module(default target)"
	@echo " clean    - clean build dir"
	@echo " insmod   - insmod boot_params ko module"
	@echo " rmmod    - rmmod boot_params ko module"
	@echo " lsmod    - find boot_params ko module whether already insmod"
	@echo " status   - same as lsmod"
	@echo " info     - display boot_params ko info"
	@echo " modinfo  - same as info"
	@echo " help     - display help info"

