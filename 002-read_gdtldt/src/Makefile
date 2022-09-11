ENTRY := read_gdtldt
obj-m := $(ENTRY).o
KERNEL_VER = $(shell uname -r)
default: notfound_build
  
force_build: read_gdtldt.c
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules
	ls -alh *.ko

notfound_build: read_gdtldt.c
	[ -f *.ko ] || make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules

build: force_build

clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean

insmod: info notfound_build
	dmesg --clear
	insmod read_gdtldt.ko || true
	dmesg

rmmod:
	rmmod read_gdtldt && dmesg

lsmod:
	lsmod |grep read_gdtldt

status: lsmod

info: notfound_build read_gdtldt.ko
	modinfo read_gdtldt.ko
	md5sum read_gdtldt.ko

modinfo: info

help:
	@echo " build    - build module(default target)"
	@echo " clean    - clean build dir"
	@echo " insmod   - insmod read_gdtldt ko module"
	@echo " rmmod    - rmmod read_gdtldt ko module"
	@echo " lsmod    - find read_gdtldt ko module whether already insmod"
	@echo " status   - same as lsmod"
	@echo " info     - display read_gdtldt ko info"
	@echo " modinfo  - same as info"
	@echo " help     - display help info"

