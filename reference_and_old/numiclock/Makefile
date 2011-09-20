UISP = avrdude -c avrispmkII -P usb -p m168 -F

COMPILE = avr-g++ -Wall -Os -Iusbdrv -I. -mmcu=atmega168  #-DDEBUG_LEVEL=1

OBJECTS = clock.o 
# OBJECTS = main.o

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

fuses:
	$(UISP) -U lfuse:w:0xe6:m -U hfuse:w:0xde:m -U efuse:w:0xf9:m -v

flash:	all
	$(UISP) -U flash:w:main.hex -v

upload:	fuses flash


clean:
	rm -f main.bin main.hex main.eep.hex $(OBJECTS)

# file targets:
main.bin:	$(OBJECTS)
	$(COMPILE) -o main.bin $(OBJECTS)

main.hex:	main.bin
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex

# do the checksize script as our last action to allow successful compilation
# on Windows with WinAVR where the Unix commands will fail.

disasm:	main.bin
	avr-objdump -d main.bin

cpp:
	$(COMPILE) -E main.c
# DO NOT DELETE
