/*
 * Copyright (C) 2014 173210 <root.3.173210@live.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <pspkernel.h>
#include <string.h>
#include <psputility.h>

#include "_icon_install.h"
#include <_param_install.h>
#include <_data_install.h>

#define printf pspDebugScreenPrintf

PSP_MODULE_INFO("MKBL", PSP_MODULE_USER, 0, 0);

void Exit(char *fmt, ...)
{
    sceKernelDelayThread(6000*200); 
	sceKernelExitGame(); 
}

int flash_file(char *file, void *file_name,  int file_size) 
{
	sceIoRemove(file);
    SceUID fd = sceIoOpen(file, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC , 511 );
    if(fd < 0)
          Exit("\nCannot open file for writing."); 

    int written = sceIoWrite(fd, file_name, file_size);
    if(written != file_size) 
	{
		sceIoClose(fd);
		Exit("\nCannot write file.");
	}

    sceIoClose(fd);
    return 0;
}

SceUtilitySavedataParam params = {
	.base = {
		.size = sizeof(SceUtilitySavedataParam),
		.graphicsThread = 16,
		.accessThread = 16,
		.fontThread = 16,
		.soundThread = 16
	},
	.mode = PSP_UTILITY_SAVEDATA_AUTOLOAD
};

int main(int argc, char *argv[])
{
	SceUID fd;
	int fail_cnt = 0;
	int len;
	void *p;

	pspDebugScreenInit();
	pspDebugScreenSetXY(0, 0);

	printf("Blacklist Dumper by 173210\n");
	printf("Update by KanadeEngel\n\n");

	sceIoMkdir("ms0:/PSP/SAVEDATA/DUMP01234", 0777);
	flash_file("ms0:/PSP/SAVEDATA/DUMP01234/ICON0.PNG", _icon_install, size__icon_install);
	flash_file("ms0:/PSP/SAVEDATA/DUMP01234/PARAM.SFO", _param_install, size__param_install);
	flash_file("ms0:/PSP/SAVEDATA/DUMP01234/SECURE.BIN", _data_install, size__data_install);
	
	
	printf("Initializing the savedata utility...\n\n");
	sceUtilitySavedataInitStart(&params);
   
	while(sceUtilitySavedataGetStatus() < PSP_UTILITY_DIALOG_VISIBLE)
		sceKernelDelayThread(65536);

	printf("Searching the blacklist...\n");
	for (p = (void *)0x08400000; (int)p < 0x08800000; p += sizeof(int)) {
		if(*(int *)p == 0x5F455445) {
			printf("Found at %p\nOpening ms0:/PSP/SAVEDATA/DUMP01234/BL.TXT\n", p);
			fd = sceIoOpen("ms0:/PSP/SAVEDATA/DUMP01234/BL.TXT", PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);

			printf("Searching IDs...\n");
			while(fail_cnt < 32) {
				len = strlen((char *)p);

				if(*(char *)p == 'N' || *(char *)p == 'U') {
					((char *)p)[len] = '\n';
					printf((char *)p);
					sceIoWrite(fd, p, len + 1);
					fail_cnt = 0;
				} else {
					fail_cnt += 1;

					if(!len) {
						p += 4;
						continue;
					}
				}

				p += len & 3 ? (len & 0x7FFFFFFC) + 4 : len;
			}

			printf("Closing ms0:/PSP/SAVEDATA/DUMP01234/BL.TXT\n");
			sceIoClose(fd);
			break;
		}
	}

	printf("shutdown savedata utility...\n");
	sceUtilitySavedataShutdownStart();

	printf("Exiting...\n");
	sceKernelExitGame();

	return 0;
}
