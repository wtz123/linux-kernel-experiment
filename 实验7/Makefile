scull-objs := main.o 
obj-m := scull.o 
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean 
