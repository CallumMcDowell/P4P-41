C_SRC	:= $(wildcard *.c)
A_SRC	:= $(wildcard *.S)
LD_SCRIPT := link.ld
DIRS := out .obj

# Filter out any source files to be not included in the build
C_FILTER :=

C_SRC := $(filter-out $(C_FILTER), $(C_SRC))
C_OUT := $(addprefix .obj/, $(C_SRC:.c=.o))
A_OUT := $(addprefix .obj/, $(A_SRC:.S=.S.o))

CROSS_COMPILE = /opt/riscv/bin/riscv64-unknown-elf-

RM := rm -rf
CC := $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
BIN2MIF := python3 bin2mif.py

ARCH := rv32im
ABI := ilp32

BUILD_VERSION := "$(shell git describe --abbrev=8 --dirty --always --tags)"
BUILD_DATE := "$(shell date '+%Y-%m-%d %H:%M:%S %:::z')"

# Compiler Flags
CFLAGS := -fno-delete-null-pointer-checks -march=$(ARCH) -mabi=$(ABI) -Wall -std=gnu99 -I. -ffunction-sections -fdata-sections -flto -fno-strict-aliasing
CFLAGS += -DDBUILD_VERSION=\"$(BUILD_VERSION)\"
CFLAGS += -DDBUILD_DATE=\"$(BUILD_DATE)\"

# In order: generate debug info, include debug symbols, no optimisation, optimising for size
CFLAGS += -Os #-ggdb3 -O0 # -Os -g3 
# Logs configuration (flag) settings
CFLAGS += -frecord-gcc-switches
# Other Warnings options not covered by -Wall or -Wextra
CFLAGS += -Wshadow -Wdouble-promotion -Wundef


# Linker Flags
LDFLAGS := -march=$(ARCH) -mabi=$(ABI) -static -nostartfiles -nostdlib -lgcc -lc -Wl,-print-memory-usage -Wl,-gc-sections

APP = out/bootrom

DUMP := $(APP).dump
ELF  := $(APP).elf
BIN  := $(APP).bin
HEX  := $(APP).hex
SREC := $(APP).s37
MIF  := $(APP).mif

all: $(MIF)

make_dirs:
	$(shell mkdir -p $(DIRS))

.obj/%.o: %.c make_dirs
	$(CC) $(CFLAGS) -c $< -o $@

.obj/%.S.o: %.S make_dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(C_OUT) $(A_OUT) $(LD_SCRIPT)
	$(CC) -T $(LD_SCRIPT) $(A_OUT) $(C_OUT) -o $@ $(LDFLAGS)
	$(OBJDUMP) -D $@ > $(DUMP)

# If needed to inspect raw instructions: -Mnumeric,no-aliases

$(SREC): $(ELF)
	$(OBJCOPY) -I elf32-littleriscv -O srec --srec-forceS3 $< $@

$(MIF): $(BIN)
	$(BIN2MIF) $< 0x0 > $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

.PHONY: all clean distclean print debug

clean:
	$(RM) $(DIRS)

print:
	@echo C_SRC $(C_SRC)
	@echo A_SRC $(A_SRC)
	@echo DIRS $(DIRS)
