# Improved PIA Communicator for RC6502

This is an improved version of the [RC6502](https://github.com/tebl/RC6502-Apple-1-Replica) PIA communicator firmware for an Arduino Nano. It is based on the original [Arduino Sketch](https://github.com/tebl/RC6502-Apple-1-Replica/blob/master/RC6502%20Serial%20IO/pia_communicator/pia_communicator.ino) provided with the RC6502. I have replaced the Arduino serial library with buffered interrupt-based serial communications routines and removed all the unnecessary communication delays.  

This firmware is usually able to accept 40-column hex input at full speed, but it still drops characters with wider hex dumps because the Arduino doesn't have hardware flow control.  However, using TeraTerm with a 20 ms delay between lines, it can reliably load hex data up to 97 columns wide. With the 20ms per-line delay, the entire 20KB of hex data for [Fig Forth](http://www.callapple.org/soft/ap1/system/fig110.txt), which is in 97-column hex format, loads reliably in under 10 seconds.

This code is compiled with the provided Makefile using avr-libc and avr-gcc and flashed using avrdude. The Arduino IDE is not used.

# Credits

pia.c is based on the original [RC6502](https://github.com/tebl/RC6502-Apple-1-Replica) pia_communicator.ino sketch by tebl, and released under the GPLv3 license.

uart.h and uart.c come from the [FatFs](http://elm-chan.org/fsw/ff/00index_e.html) AVR example project. Copyright (C) 2016 ChaN.

The iox.h, iox.c, spi.h, and spi.c come from my own [z80ctrl](https://github.com/jblang/z80ctrl) project.
