
BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/main
MCU = cortex-m7

LD_SCRIPT = ./ld/stm32f7508-dk.ld

DEFINES = -DMCU_STM32F750 -DBOARD_STM32F7508_DK

GDB_SERVER_PORT = 4242

CXX = arm-none-eabi-g++
OC = arm-none-eabi-objcopy
OS = arm-none-eabi-size
GDB = arm-none-eabi-gdb

WFLAGS = -Wall -Wpedantic -Wextra -Wno-unused-parameter

CXXFLAGS = -c -O0 -mcpu=$(MCU) -mthumb -g3 -mhard-float -mfloat-abi=hard \
	-mfpu=fpv5-sp-d16 -ffunction-sections -fdata-sections \
	-fno-exceptions -fno-rtti --specs=nosys.specs $(WFLAGS)
LFLAGS = -T $(LD_SCRIPT) -mcpu=$(MCU) -mthumb -lgcc -mhard-float \
	-mfloat-abi=hard -mfpu=fpv5-sp-d16 -Wl,--gc-sections -Wl,-L./ld \
	--specs=nosys.specs $(WFLAGS)

INCLUDES = -I./include -I./src/*

SRC_DIR = ./src
CXX_EXT = cpp

CXX_SRC = $(shell find $(SRC_DIR) -type f -name *.$(CXX_EXT))
OBJS = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(CXX_SRC:.$(CXX_EXT)=.o))
DEPENDS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(CXX_SRC:.$(CXX_EXT)=.d))


-include $(DEPENDS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(CXX_EXT) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -MMD -MP -o $@ $<

$(TARGET).elf: $(OBJS)
	$(CXX) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


.PHONY: all flash-n-debug flash-n-debug-no-tui clean

all: $(TARGET).bin

flash-n-debug: all
	$(GDB) $(TARGET).elf \
		-ex 'target extended-remote :$(GDB_SERVER_PORT)' -ex load \
		-ex 'tui e' -ex 'layout regs' -ex 'break reset_handler' -ex continue -ex 'focus cmd'

flash-n-debug-no-tui: all
	$(GDB) $(TARGET).elf \
		-ex 'target extended-remote :$(GDB_SERVER_PORT)' -ex load -ex 'break main' -ex continue

clean:
	rm -rf $(BUILD_DIR)/*
