# NOTE: This file will need to be edited to work with the new attacks. 
#  	Check out branches "makefile_test" & "eric_branch" for template code.

ARCH=arm
COMPILER=arm-rpi-linux-gnueabihf 

CC_BIN = 
PROJECT = attacks

OBJECTS = ./main.o ./lib.o

AS      = $(GCC_BIN)arm-rpi-linux-gnueabihf-as
CC      = $(GCC_BIN)arm-rpi-linux-gnueabihf-gcc
CPP     = $(GCC_BIN)arm-rpi-linux-gnueabihf-g++
LD      = $(GCC_BIN)arm-rpi-linux-gnueabihf-gcc
OBJCOPY = $(GCC_BIN)arm-rpi-linux-gnueabihf-objcopy


CC_FLAGS = $(CPU) -c -g -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -std=c++11
CC_SYMBOLS = -DTOOLCHAIN_GCC_ARM -DTOOLCHAIN_GCC -D__CORTEX_M3 -DARM_MATH_CM3 


all: $(PROJECT).bin

clean:
	rm -f $(PROJECT).bin $(PROJECT).elf $(OBJECTS) $(DEPS)

.s.o:
	$(AS) $(CPU) -o $@ $<

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99 -o $@ $<

.cpp.o:
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -o $@ $<


$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS)
	
$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)

