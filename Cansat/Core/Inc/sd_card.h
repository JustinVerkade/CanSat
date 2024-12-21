/*
 * sd_card.h
 *
 *  Created on: Jul 9, 2021
 *      Author: J.Verkade
 */

#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include "fatfs.h"

void SD_Init(FATFS* fatfs);
void SD_Exists(TCHAR* path);
void SD_Open(FIL* file, const TCHAR* path, UINT mode);
UINT SD_Write(FIL* file, char* text);
void SD_Save(FIL* file);
void SD_Close(FIL* file);

#endif /* INC_SD_CARD_H_ */
