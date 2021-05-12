leds_c.bin:start.s main.c
	/home/hao/arm-linux/bin/arm-linux-gcc -nostdlib -c -o start.o led.s
	/home/hao/arm-linux/bin/arm-linux-ld start.o -o led_elf
	/home/hao/arm-linux/bin/arm-linux-objcopy -O binary -S led_elf led_c.bin
	
clean:
	rm -rf led_elf led_c.bin *.o

