KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)
MODULENAME := enccom

obj-m += $(MODULENAME).o
$(MODULENAME)-objs := main.o options.o device.o packet_handle.o crypto.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
