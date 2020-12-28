gcc -Wall -m64 -mabi=ms -ffreestanding -mno-red-zone -c example_main_3.c -o example_main_3.o

gcc -Wall -m64 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main -o BOOTX64.EFI example_main_3.o

@echo off
del *.o
@echo on

pause