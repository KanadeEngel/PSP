TARGET = mkbl
OBJS = main.o

BUILD_PRX = 1

CFLAGS = -O2 -Werror

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Blacklist Dumper by 173210 Update by KanadeEngel
PSP_EBOOT_ICON = ICON0.PNG

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
