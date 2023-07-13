/*
 * sd_card.c
 *
 *  Created on: Jul 9, 2021
 *      Author: J.Verkade
 */

#include "sd_card.h"
#include <string.h>
#include <stdio.h>

static void checkCall(FRESULT result)
{
	if(result != FR_OK)
	{
		while(1)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
			HAL_Delay(100);
		}
	}
}

void SD_Init(FATFS* fatfs)
{
	checkCall(f_mount(fatfs, (TCHAR const*)SDPath, 0));
}

void SD_Exists(TCHAR* path)
{
	uint8_t index = 0;
    FRESULT fr;
    FILINFO fno;

    while(1)
    {
    	sprintf(path, "DATA%d.CSV", index);
		fr = f_stat(path, &fno);
		if(fr == FR_NO_FILE) break;
		else index++;
    }
}

void SD_Open(FIL* file, const TCHAR* path, UINT mode)
{
	checkCall(f_open(file, path, mode));
}

UINT SD_Write(FIL* file, char text[])
{
	UINT written = 0;
	checkCall(f_write(file, text, strlen(text), &written));
	return written;
}

void SD_Save(FIL* file)
{
	checkCall(f_sync(file));
}

void SD_Close(FIL* file)
{
	checkCall(f_close(file));
}


