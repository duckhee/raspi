KDIR = /lib/modules/$(shell uname -r)/build

obj-m := gpio_module.o


default:
	$(MAKE) -C $(KDIR) M=$$PWM modules


clean:
	$(MAKE) -C $(KDIR) M=$$PWM clean
