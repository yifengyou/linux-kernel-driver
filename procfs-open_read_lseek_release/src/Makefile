obj-m := procfs4.o

KERNEL_VER = $(shell uname -r)
EXTRA_CFLAGS = -g -O1
  
default: kernel_modules
  
kernel_modules:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(CURDIR) modules
      
clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(CURDIR) clean
