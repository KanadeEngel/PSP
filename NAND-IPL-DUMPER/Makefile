TARGET = ipl_dumper_loader
OBJS = main.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR = 
LDFLAGS = -L.
LIBS =

PSP_FW_VERSION = 271

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = NAND IPL Dumper by KanadeEngel

BUILD_PRX = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
