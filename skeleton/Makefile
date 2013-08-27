CC=i586-elf-g++
AS=nasm -felf
LD=i586-elf-gcc
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

all: SmithOS.iso

qemu: SmithOS.iso
	qemu-system-i386 -cdrom ../SmithOS.iso
boot.o: boot.s
	$(AS) boot.s
kernel.o: kernel.cpp
	$(CC) -c kernel.cpp -o kernel.o $(CFLAGS)
SmithOS.bin: boot.o kernel.o
	$(LD) -T linker.ld -o SmithOS.bin $(LDFLAGS) boot.o kernel.o
SmithOS.iso: SmithOS.bin
	cp SmithOS.bin ../iso/boot/SmithOS.bin
	grub-mkrescue -o ../SmithOS.iso ../iso
clean:
	rm -R -f *.o
	rm -R -f *.bin
	rm -R -f *.iso
	rm -R -f ../*.iso
	rm -R -f ../iso/boot/*.bin
	rm -R -f *~
