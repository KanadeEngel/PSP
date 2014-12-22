TARGET = XMBMOD
OBJS = main.o

LIBS = -lpspkubridge

BUILD_PRX = 1
PSP_FW_VERSION = 660

CFLAGS = -O2 -G0 -Wall -fshort-wchar
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

PSP_EBOOT_ICON = ICON0.PNG
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = XMB Mod Flasher

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
