obj-m += simple_bus.o xxx_chip.o xxx_chip_drv.o

KERNEL_VER = $(shell uname -r)
#EXTRA_CFLAGS = -g -O0
  
default: kernel_modules
  
kernel_modules:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(CURDIR) modules
      
clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(CURDIR) clean
