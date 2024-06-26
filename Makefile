#  SPDX-License-Identifier: 0BSD
#
#  Copyright (C) 2023 Michael T. Kloos <michael@michaelkloos.com>
#

CROSS_COMPILE :=
CC            := gcc
OBJCPY        := objcopy
STRIP         := strip
LDFLAGS       := -lusb-1.0
CFLAGS        :=
CFLAGS        := $(CFLAGS) -Wall -Wextra -Wno-unused-parameter -g # Set build warnings and debugging
CFLAGS        := $(CFLAGS) -std=c99 # The standards to build to.
CFLAGS        := $(CFLAGS) -fno-stack-check -fno-stack-protector -fomit-frame-pointer -ffunction-sections -flto
CFLAGS        := $(CFLAGS) -O3 -MMD
TARGET        := daptool

GFILES        :=
GFILES        := $(GFILES) main.o
GFILES        := $(GFILES) dapctl.o
GFILES        := $(GFILES) errors.o
GFILES        := $(GFILES) chips.o
GFILES        := $(GFILES) probes.o
GFILES        := $(GFILES) dapctl.o
GFILES        := $(GFILES) dapctl/dap_link.o
GFILES        := $(GFILES) dapctl/dap_cmds.o
GFILES        := $(GFILES) dapctl/dap_oper.o
GFILES        := $(GFILES) modes/flash.o

GFILES        := $(GFILES) chips/max32690.o

GFILES        := $(GFILES) probes/max32625pico.o

# What list of base filenames are we to build?
FILES_BASE    := $(basename $(GFILES))

.PHONY: all rebuild clean install

all: $(TARGET).dynamic $(TARGET).dynamic.strip # $(TARGET).static $(TARGET).static.strip

rebuild: clean
	$(MAKE) all

clean:
	rm -f $(TARGET).dynamic $(TARGET).dynamic.strip $(TARGET).static $(TARGET).static.strip
	rm -f $(addsuffix .o,$(FILES_BASE))
	rm -f $(addsuffix .d,$(FILES_BASE))

install: $(TARGET).dynamic.strip
	cp $(TARGET).dynamic.strip /usr/local/bin/$(TARGET)

%.o: %.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).dynamic: $(GFILES)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TARGET).static: $(GFILES)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -static $^ -o $@ $(LDFLAGS)

%.strip: %
	$(CROSS_COMPILE)$(STRIP) -s -x -R .comment $^ -o $@

# Header dependency tracking
-include $(addsuffix .d,$(FILES_BASE))
