obj-m	:= pci_skel.o

KERNELDIR ?= /home/hao/work_my/kernel_source/x86_64/linux-4.19.90/
PWD       := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

