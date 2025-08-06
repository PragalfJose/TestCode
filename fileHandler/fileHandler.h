//****************************** fileHandler **********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : fileHandler.h
// Summary  : Includes necessary file operations
// Note     : None
// Author   : Pragalf T Jose
// Date     : 05/08/2025
//
//*****************************************************************************
#ifndef _FILE_HANDLER_H
#define _FILE_HANDLER_H

//******************************* Include Files *******************************
#include "Common.h"

//******************************* Global Types ********************************

//***************************** Global Constants ******************************
#define OPEN_WR                 "w"
#define OPEN_RD                 "r"
#define OPEN_APND               "a"
#define OPEN_W_PLUS             "w+"
#define OPEN_R_PLUS             "r+"
#define OPEN_A_PLUS             "a+"



//***************************** Global Variables ******************************

//**************************** Forward Declarations ***************************
bool fileHandlerFileOpen(FILE **pFileHandle, 
                         uint8 *pucFileName, 
                         uint8 *pucFileOpenMode);
bool fileHandlerFileClose(FILE **pFileHandle);
bool fileHandlerFileWrite(uint8 *pucFileName, 
                          uint8 *pucWriteData, 
                          uint32 ulWriteSize, 
                          uint8 *pucWriteMode);
bool fileHandlerFileWriteInPosition(uint8 *pucFileName, 
                                    uint8 *pucWriteData, 
                                    uint32 ulWriteSize, 
                                    uint32 ulPosition);
bool fileHandlerFileRead(uint8 *pucFileName, 
                          uint8 *pucWriteData, 
                          uint32 ulWriteSize, 
                          uint8 *pucWriteMode);
bool fileHandlerFileReadFromPosition(uint8 *pucFileName, 
                                     uint8 *pucWriteData, 
                                     uint32 ulWriteSize, 
                                     uint32 ulPosition);
bool fileHandlerFileRemove(uint8 *pucFileName);
bool fileHandlerFileCreate(uint8 *pucFileName);
bool fileHandlerFileCheckExist(uint8 *pucFileName);
bool fileHandlerFilePointerPosition(FILE ** pFileHandle, int32 *plPosition);
// bool fileHandlerFileSeek();

#endif // _FILE_HANDLER_H
// EOF
