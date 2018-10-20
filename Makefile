# MCU type and frequency
MCU=atmega328p
F_CPU=16000000L

# Programmer options
PROGRAMMER=arduino
PORT=/dev/ttyS8
BAUD=57600

CC=avr-gcc
OBJCOPY=avr-objcopy
AVRDUDE=avrdude

BIN=pia
OBJS=	pia.o \
		uart.o \
		spi.o \
		iox.o

CFLAGS=-std=c99 -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -I.

$(BIN).hex: $(BIN).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BIN).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

install: $(BIN).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<

clean:
	rm -f $(BIN).elf $(BIN).hex $(OBJS)
