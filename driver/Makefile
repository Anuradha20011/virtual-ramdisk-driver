obj-m += virtual_ramdisk.o

KDIR := $(HOME)/projects/virtual-ramdisk-driver/wsl-kernel
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
