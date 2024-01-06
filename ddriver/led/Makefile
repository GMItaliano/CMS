obj-m := led.o
led-objs:=ledmodule.o utils.o
KDIR := /home/goncalo/buildroots/buildroot-2021.08-Fato/output/build/linux-custom
ARCH ?= arm
CROSS_COMPILE ?= /home/goncalo/buildroots/buildroot-2021.08-Fato/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-

export ARCH CROSS_COMPILE

	
KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	#$(MAKE) -C $(KDIR) M=$(shell pwd) $@ EXTRA_CFLAGS="$(MY_CFLAGS)"
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@