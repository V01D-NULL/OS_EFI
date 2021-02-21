gcc -Wall -m64 -Ttext 0x00100000 -mabi=ms -Wl,--oformat=binary -c kernel_test_1.c -o kernel.bin

pause