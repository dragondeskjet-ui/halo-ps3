TARGET     := halo_data_installer
TITLE      := Halo CE Data Installer
TITLE_ID   := HALOCEDAT
CONTENT_ID := UP0001-$(TITLE_ID)_00-0000000000000000
BUILD      ?= build

PREFIX     := $(PS3DEV)/ppu/bin/ppu-
CC         := $(PREFIX)gcc
SPRXLINK   := $(PS3DEV)/bin/sprxlinker
MAKE_SELF_NPDRM := $(PS3DEV)/bin/make_self_npdrm

CFLAGS     := -O2 -Wall -mcpu=cell -mhard-float -D__PS3__ -I$(PSL1GHT)/ppu/include
LDFLAGS    := -L$(PSL1GHT)/ppu/lib -lio -lsysutil -lsysmodule -lrt -llv2

all: $(BUILD)/EBOOT.BIN

$(BUILD)/$(TARGET).elf: installer.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(BUILD)/EBOOT.BIN: $(BUILD)/$(TARGET).elf
	$(SPRXLINK) $<
	$(MAKE_SELF_NPDRM) $< $@ $(CONTENT_ID)
