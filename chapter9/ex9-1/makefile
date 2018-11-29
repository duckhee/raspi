KDIR = /lib/modules/3.18.7-v7/build

obj-m := ex9_1.o

default:
	$(MAKE) -C $(KDIR) M=$$PWM modules

clean:
	$(MAKE) -C $(KDIR) M=$$PWM clean