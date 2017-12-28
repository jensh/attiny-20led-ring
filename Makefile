# (c) 2017-12-28 Jens Hauke <jens@4k2.de>			-*- makefile -*-

all:

MCU_TARGET=attiny45

PROGS += led_circle.elf
led_circle.elf: led_circle.o power_down.o led_map.o


include ./make.rules

#PROGRAMMER=usbasp
#PROGRAMMER=stk500hvsp
#PROGRAMMER_OPTIONS=-b57600 -P/dev/ttyUSB0
