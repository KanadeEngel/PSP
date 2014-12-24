#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <pspuser.h>
#include <pspdebug.h>
#include <pspnand_driver.h>

PSP_MODULE_INFO("nand_ipl", 0x1006, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

int g_iPageSize, g_iPagesPerBlock, g_iBlockSize;
int g_bFlashLocked = 0;

void LockFlash();
void UnlockFlash();
void die(const char *msg);
int ReadIPLBlockTable(void *pBlockBuf);

void msglog(char *fmt, ...)
{
	va_list list; 
	char msg[256]; 
	va_start(list, fmt); 
	vsprintf(msg, fmt, list); 
	va_end(list); 

	SceUID fd = sceIoOpen("ms0:/Dump_Log.txt", PSP_O_WRONLY | PSP_O_CREAT, 0777);
	if(fd > 0)
	{
		sceIoLseek(fd, 0, PSP_SEEK_END);
		sceIoWrite(fd, msg, strlen(msg));
		sceIoClose(fd);
	}
}

int WriteFile(char *file, char *buf, int size, int seek)
{
    SceUID fd = sceIoOpen(file, PSP_O_WRONLY | PSP_O_CREAT, 0777);

	int write = -1;

    if(fd >= 0)
    {
		sceIoLseek(fd, seek, PSP_SEEK_SET);
        write = sceIoWrite(fd, buf, size);
        sceIoClose(fd);
    }

	return write;
}

void LockFlash()
{
	if(!g_bFlashLocked)
		sceNandLock(0);

	g_bFlashLocked = 1;
}

void UnlockFlash()
{
	if(g_bFlashLocked)
		sceNandUnlock();

	g_bFlashLocked = 0;
}

void die(const char *msg)
{
	UnlockFlash();
	msglog("-- Fatal error: %s.\r\n", msg);
	while(1)
		sceKernelSleepThread();
}

int ReadIPLBlockTable(void *pBlockBuf)
{
	u16 *pBlockTable;
	u32 uiBlockNum = 4;
	u32 uiSearchCount = 8;
	int iBlockCount;

	LockFlash();
	memset(pBlockBuf, 0, g_iBlockSize);

	while(uiSearchCount > 0)
	{
		u32 uiPPN = uiBlockNum * g_iPagesPerBlock;

		if(!sceNandIsBadBlock(uiPPN))
		{
			if(sceNandReadBlockWithRetry(uiPPN, pBlockBuf, NULL) < 0)
				die("-- can't read IPL block table!\r\n");

			msglog("Found IPL block table at block %d.\r\n", uiBlockNum);

			break;
		}

		uiBlockNum++;
		uiSearchCount--;
	}


	pBlockTable = (u16 *) pBlockBuf;
	iBlockCount = 0;

	while(*pBlockTable != 0)
	{
		iBlockCount++;
		pBlockTable++;
	}

	if(iBlockCount)
		msglog("IPL spans a total of %d blocks.\r\n", iBlockCount);

	UnlockFlash();
	return iBlockCount;
}

int start_thread()
{
	void *pBlockBuf;
	u16 *pBlockTable;
	int iBlockCount, iBlock;
	int fd;

	g_iPageSize = sceNandGetPageSize();
	g_iPagesPerBlock = sceNandGetPagesPerBlock();
	g_iBlockSize = g_iPageSize * g_iPagesPerBlock;

	
	pBlockBuf = malloc(g_iBlockSize);
	assert(pBlockBuf);

	iBlockCount = ReadIPLBlockTable(pBlockBuf);
	if(!iBlockCount)
		die("-- IPL block count is 0!.\r\n");

	pBlockTable = (u16 *) malloc(iBlockCount * sizeof(u16));
	memcpy(pBlockTable, pBlockBuf, iBlockCount * sizeof(u16));

	fd = sceIoOpen("ms0:/nand_ipl.bin", PSP_O_WRONLY | PSP_O_CREAT, 0777);
	if (fd < 0)
		die("-- can't open ms0:/nand_ipl.bin for writing!\r\n");

	for(iBlock = 0; iBlock < iBlockCount; iBlock++)
	{
		u32 uiPPN = pBlockTable[iBlock] * g_iPagesPerBlock;

		LockFlash();

		if(sceNandReadBlockWithRetry(uiPPN, pBlockBuf, NULL) < 0)
		{
			sceIoClose(fd);
			die("-- can't read block from IPL!\r\n");
		}

		UnlockFlash();

		if(sceIoWrite(fd, pBlockBuf, g_iBlockSize) < 0)
		{
			sceIoClose(fd);
			die("-- can't write data to file!\r\n");
		}
	}

	sceIoClose(fd);
	free(pBlockTable);
	free(pBlockBuf);

	msglog("Finished writing NAND IPL to ms0:/nand_ipl.bin.\r\n");
	sceKernelSleepThread();

	return 0;
}

int module_start(SceSize args, void *argp)
{
	SceUID thid = sceKernelCreateThread("start_thread", start_thread, 0x10, 0x4000, 0, NULL);
	if(thid < 0)
		return thid;

	sceKernelStartThread(thid, args, argp);

	return 0;
}

int module_stop(void)
{
	return 0;
}
