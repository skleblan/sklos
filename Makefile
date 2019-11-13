
ARMGNU ?= arm-none-eabi
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
OBJDIR = obj
SRCDIR = src

SOURCES := $(wildcard $(SRCDIR)/*.c)
ASM_SRCS := $(wildcard $(SRCDIR)/*.s)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ASM_OBJS := $(ASM_SRCS:$(SRCDIR)/%.s=$(OBJDIR)/%.o)

all : kernel.img sklos.hex

clean :
	rm -f -R $(OBJDIR)
	rm -f *.hex *.elf *.list *.img
	rm -f prograspi

directories :
	mkdir -p $(OBJDIR)

$(ASM_OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.s directories
	$(ARMGNU)-as $< -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c directories
	$(ARMGNU)-gcc $(COPS) -c $< -o $@

sklos.elf : memmap $(OBJECTS) $(ASM_OBJS)
	$(ARMGNU)-ld $(OBJECTS) $(ASM_OBJS) -T memmap -o sklos.elf
	$(ARMGNU)-objdump -D sklos.elf > sklos.list

kernel.img : sklos.elf
	$(ARMGNU)-objcopy sklos.elf -O binary $@

sklos.hex : sklos.elf
	$(ARMGNU)-objcopy sklos.elf -O ihex $@

prograspi : prograspi.c ser.c ser.h
	gcc -O2 -Wall prograspi.c ser.c -o prograspi
