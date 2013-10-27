ARDUINO_DIR   = /usr/share/arduino
ARDMK_DIR     = /usr
AVR_TOOLS_DIR = /usr

BOARD        = NANO
BOARD_TAG    = atmega328
MCU          = atmega328
ARDUINO_PORT = /dev/ttyUSB*

# HACK - avrdude's options don't match the BOARD tag on Saucy Salamander
AVRDUDE_ARD_EXTRAOPTS = -p m328p

# LIBRARIES    = Wire Core
# ARDUINO_LIBS = Ethernet Ethernet/utility SPI

include /usr/share/arduino/Arduino.mk
