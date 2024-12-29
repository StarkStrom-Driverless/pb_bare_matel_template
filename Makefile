CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. -Isys -Isys/definitions -Iusr -Iusr_configs\
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

# Include the sources.mk file
include sys/sys_sources.mk
include usr/usr_sources.mk

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -f
endif

build: firmware.bin

firmware.elf: $(SYS_SOURCES) $(USR_SOURCES)
	arm-none-eabi-gcc main.c $(SYS_SOURCES) $(USR_SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	$(RM) firmware.*