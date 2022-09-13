ENTRY := vaddr_to_paddr
obj-m := $(ENTRY).o
KERNEL_VER = $(shell uname -r)
default: notfound_build
  
force_build: vaddr_to_paddr.c
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules
	ls -alh *.ko

notfound_build: vaddr_to_paddr.c
	[ -f *.ko ] || make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules

build: force_build

clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean

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

