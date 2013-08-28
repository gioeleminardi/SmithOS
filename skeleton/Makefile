CC=i586-elf-g++
ASFLAGS=-felf
LD=i586-elf-gcc
CFLAGS=-c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc
SOURCES=kernel.cpp monitor.cpp common.cpp
OBJECTS=$(SOURCES:.cpp=.o) boot.o
EXECUTABLE=SmithOS.bin

all: SmithOS.iso

qemu: SmithOS.iso
	qemu-system-i386 -cdrom ../SmithOS.iso
.s.o:
	nasm $(ASFLAGS) $<
.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@

SmithOS.bin: $(OBJECTS)
	$(LD) -T linker.ld -o $@ $(LDFLAGS) $(OBJECTS)

SmithOS.iso: SmithOS.bin
	cp SmithOS.bin ../iso/boot/SmithOS.bin
	grub-mkrescue -o ../SmithOS.iso ../iso

