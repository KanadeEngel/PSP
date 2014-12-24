#include <pspsdk.h> 
#include <pspkernel.h> 
#include <pspdebug.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <stdarg.h> 

PSP_MODULE_INFO("NANDIPLDumpLoader", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(0);



#define printf pspDebugScreenPrintf 



void ErrorExit(int milisecs, char *fmt, ...) 
{ 
	va_list list; 
	char msg[256]; 

	va_start(list, fmt); 
	vsprintf(msg, fmt, list); 
	va_end(list); 

	printf(msg);

	sceKernelDelayThread(milisecs*1000); 
	sceKernelExitGame(); 
} 

int LoadStartModule(char *module, int partition)
{
	SceUID mod = sceKernelLoadModule(module, 0, NULL);

	if (mod < 0)
		return mod;

	return sceKernelStartModule(mod, 0, NULL, NULL, NULL);
}


int main() 
	
{
	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x66FF33);
	printf("This App dumps the nand_ipl.bin to your Memory Stick\n");
	printf("\nDeveloped by KanadeEngel\n");
    printf("\nThanks to: \n\nNeur0n for his ME/LME CFW\nTotal_Noob for his TN Hen\nTeam Pro for the Pro HEN");
	printf("\nVisit www.KINGx.de and http://twitter.com/KanadeEngel\n");
	sceIoRemove("ms0:/Dump_Log.txt");
	sceKernelDelayThread(1000*1000);

	int mod = LoadStartModule("nand_ipl.prx", PSP_MEMORY_PARTITION_KERNEL);
	if(mod < 0)
		ErrorExit(5000, "Error 0x%08X loading nand_ipl.prx.\n", mod);
    
	ErrorExit(6000, "\n\nDONE :)\n\n\n\nExiting in 6 Seconds...");
	

	return 0;
}
