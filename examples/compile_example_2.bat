gcc -Wall -m64 -mabi=ms -ffreestanding -c example_main_2.c -o example_main_2.o

gcc -Wall -m64 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main -o BOOTX64.EFI example_main_2.o

@echo off
del *.o
@echo on

pause