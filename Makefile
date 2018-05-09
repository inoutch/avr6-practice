TARGET	  	?= arduino-practice
BUILD_DIR	?= build
SRC_DIRS	?= src

SRCS 		:= $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
FLS			:= $(notdir $(SRCS))
OBJS 		:= $(SRCS:%=$(BUILD_DIR)/%.o)
INC_DIRS 	:= $(shell find $(SRC_DIRS) -type d)
INC_FLAGS 	:= $(addprefix -I,$(INC_DIRS))

MMCU 		= avr6
CONF 		= $$ARDUINO_PATH/hardware/tools/avr/etc/avrdude.conf
DEV 		= /dev/cu.usbmodem1421

AS 	= avr-as
LD	= avr-ld
CC	= avr-gcc
OBJDUMP	= avr-objdump
OBJCOPY	= avr-objcopy

LFLAGS	= -static -T src/ld.scr -L.

CFLAGS  = -Wall -nostdinc -nostdlib -fno-builtin
CFLAGS  += -mmcu=$(MMCU)
CFLAGS  += -Wl,--defsym=__stack=_bootstack # 先生に教わったオプション
CFLAGS  += -fno-jump-tables
CFLAGS  += -fno-inline
CFLAGS  += -I. -g

ASFLAGS = -mmcu=$(MMCU)

# -Mでメモリマップ表示
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(LD) -o $(BUILD_DIR)/$(TARGET).elf $(LFLAGS) $(OBJS)
	$(OBJDUMP) -d $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).dump
	$(OBJCOPY) -I elf32-avr -O ihex $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

write: $(BUILD_DIR)/$(TARGET)
	avrdude -C $(CONF) -c wiring -P $(DEV) -p m2560 \
	-b 115200 -D -U flash:w:$(BUILD_DIR)/$(TARGET).hex:i

MKDIR_P ?= mkdir -p