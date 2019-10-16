
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

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

.c.o :
	$(ARMGNU)-gcc $(COPS) -c $*.c -o $@

sklos.elf : memmap vectors.o uart.o sklmain.o util.o bootldr.o
	$(ARMGNU)-ld $^ -T memmap -o sklos.elf
	$(ARMGNU)-objdump -D sklos.elf > sklos.list

sklos.bin : sklos.elf
	$(ARMGNU)-objcopy sklos.elf -O binary sklos.bin

sklos.hex : sklos.elf
	$(ARMGNU)-objcopy sklos.elf -O ihex sklos.hex

