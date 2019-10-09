
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : sklos.hex sklos.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

sklmain.o : sklmain.c
	$(ARMGNU)-gcc $(COPS) -c sklmain.c -o sklmain.o

uart.o : uart.c uart.h
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

sklos.elf : memmap vectors.o uart.o sklmain.o 
	$(ARMGNU)-ld vectors.o uart.o sklmain.o -T memmap -o sklos.elf
	$(ARMGNU)-objdump -D sklos.elf > sklos.list

sklos.bin : sklos.elf
	$(ARMGNU)-objcopy sklos.elf -O binary sklos.bin

sklos.hex : sklos.elf
	$(ARMGNU)-objcopy sklos.elf -O ihex sklos.hex

