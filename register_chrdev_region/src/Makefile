ENTRY := register_chrdev_region
obj-m := $(ENTRY).o
KERNEL_VER = $(shell uname -r)
default: build_module
  
build_module:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules
	ls -alh *.ko
      
clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean

insmod:
	dmesg --clear
	insmod $(ENTRY).ko  &> /dev/null && dmesg

rmmod:
	rmmod $(ENTRY) && dmesg
