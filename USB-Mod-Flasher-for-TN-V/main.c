/* TN-V USB Mod Installer - 2014, KanadeEngel & The Z */

#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <kubridge.h>

#include <_660_install_X_icon.h>
#include <_660_original_X_icon.h>

PSP_MODULE_INFO("USB ICON Mod", 0x0800, 9, 9); 
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VSH);

#define printf pspDebugScreenPrintf
#define setc pspDebugScreenSetXY
#define setcolor pspDebugScreenSetTextColor
#define setbcolor pspDebugScreenSetBackColor

#define RGB(r, g, b) (0xFF000000 | ((b)<<16) | ((g)<<8) | (r))
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define BLUE RGB(0, 0, 255)
#define BLACK RGB(0, 0, 0)
#define ORANGE RGB(255, 127, 0)
#define WHITE RGB(255, 255, 255)
#define YELLOW RGB(255, 255, 0)
#define GRAY RGB(103, 120, 137)

void Exit(char *fmt, ...)
{
	char msg[256];
	va_list list;
	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);
	/*printf("%s\nPress X to exit.", msg);

	sceKernelDelayThread(1000 * 100);
	while(1)
	{
		SceCtrlData pad;
		sceCtrlReadBufferPositive(&pad, 1);

		if(pad.Buttons & PSP_CTRL_CROSS)
			break;
	} */
    sceKernelDelayThread(6000*200); // Auto-Exit
	sceKernelExitGame(); 
}

int flash_file(char *file, void *file_name,  int file_size) 
{
	sceIoRemove(file);
    printf("Writing File to %s...", file);
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
    printf(" OK\n");

    return 0;
}

int assign_flash_zero()
{
	int ret = sceIoUnassign("flash0:");
	ret += sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", IOASSIGN_RDWR, NULL, 0);

	return ret;
}

void printfc(int x, int y, char *fmt, ...)
{	
	char msg[256];

	va_list list;
	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	setc(x, y);
	printf(msg);
	setcolor(WHITE);
}

//from ospbt by cory1492
void wait_release(unsigned int buttons)
{
	SceCtrlData pad;

	sceCtrlReadBufferPositive(&pad, 1);
	while(pad.Buttons & buttons)
		sceCtrlReadBufferPositive(&pad, 1);
}

//from ospbt by cory1492
unsigned int wait_press(unsigned int buttons)
{
	SceCtrlData pad;

	sceCtrlReadBufferPositive(&pad, 1);
	while(1)
	{
		if(pad.Buttons & buttons)
			return pad.Buttons & buttons;

		sceCtrlReadBufferPositive(&pad, 1);
	}

	return 0;
}


int main() 
{

   	pspDebugScreenInit();
	int devkit = sceKernelDevkitVersion(),
	cursor = 0;
	
		if(devkit != 0x06060010)
		Exit("This program supports only 6.60!");

    printf("USB Mod Flasher\n\n");    
	setcolor(RED);
	printf("This Mod is only for the TN-V8 eCFW for the PS Vita!");
	setcolor(WHITE);
	printf("\n\n\n\n\n\n\n\n\n\n\n\nCredits:\nXMB Mod by The Z\nInstaller Port by KanadeEngel\n\nSpecial Thanks to:\nfrostegater");

	while(1)
	{
		if(cursor > 2)
			cursor = 0;
		else if(cursor < 0)
			cursor = 2;

		if(cursor == 0) setbcolor(GRAY);
		printfc(3, 4, " Install XMB Mod.                       ");
		setbcolor(BLACK);
		if(cursor == 1) setbcolor(GRAY);
		printfc(3, 5, " Restore Original Files.                ");
		setbcolor(BLACK);
		if(cursor == 2) setbcolor(GRAY);
		printfc(3, 6, " Exit.                                  ");
		setbcolor(BLACK);

		int i;
		for(i = 0; i < 4; i++)
			printfc(1, 4 + i, " ");

		setcolor(BLUE);
		printfc(1, 4 + cursor, ">");

		unsigned int Buttons = wait_press(PSP_CTRL_CROSS | PSP_CTRL_UP | PSP_CTRL_DOWN);
		wait_release(PSP_CTRL_CROSS | PSP_CTRL_UP | PSP_CTRL_DOWN);

		SceCtrlData pad;
		sceCtrlReadBufferPositive(&pad, 1);

		if(Buttons & PSP_CTRL_CROSS)
		{
			if(cursor != 2)
			{
				setc(0, 9);
				if(assign_flash_zero() < 0)
					Exit("\n Error in 'flash0:/' assign.");
	

					{
						if(cursor == 0)//Install Mod Module
						{
							flash_file("flash0:/vsh/resource/topmenu_icon.rco", _660_install_X_icon, size__660_install_X_icon);
						}
	                    else if(cursor == 1)//Restore Original Module
							flash_file("flash0:/vsh/resource/topmenu_icon.rco", _660_original_X_icon, size__660_original_X_icon);
						}
					}
			else
				Exit("");

			break;
		}
		else if(Buttons & PSP_CTRL_UP)
			cursor--;
		else if(Buttons & PSP_CTRL_DOWN)
			cursor++;
	}

	Exit("Done.");
   sceKernelExitGame();
	return 0;
}
