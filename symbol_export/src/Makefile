ENTRY := symbol_export
obj-m := $(ENTRY).o
KERNEL_VER = $(shell uname -r)
default: notfound_build
  
force_build: symbol_export.c
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules
	ls -alh *.ko

notfound_build: symbol_export.c
	[ -f *.ko ] || make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules

build: force_build

clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean

insmod: info notfound_build
	dmesg --clear
	insmod symbol_export.ko || true
	dmesg

rmmod:
	rmmod symbol_export && dmesg

lsmod:
	lsmod |grep symbol_export

status: lsmod

info: notfound_build symbol_export.ko
	modinfo symbol_export.ko
	md5sum symbol_export.ko

modinfo: info

help:
	@echo " build    - build module(default target)"
	@echo " clean    - clean build dir"
	@echo " insmod   - insmod symbol_export ko module"
	@echo " rmmod    - rmmod symbol_export ko module"
	@echo " lsmod    - find symbol_export ko module whether already insmod"
	@echo " status   - same as lsmod"
	@echo " info     - display symbol_export ko info"
	@echo " modinfo  - same as info"
	@echo " help     - display help info"

