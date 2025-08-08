//**************************** fileHandler ************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : fileHandler.c
// Summary  : Filr handling functions
// Note     : Necessary functions to control file operations
// Author   : Pragalf T Jose
// Date     : 05/08/2025
//
//*****************************************************************************

//******************************* Include Files *******************************
#include "fileHandler.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************

//***************************** Local Variables *******************************

//****************************** Local Functions ******************************

//****************************.fileHandlerFileOpen.****************************
//Purpose   : Opens a file in given name
//Inputs    : puFileName - File name
//Inputs    : puFileOpenMode - Opening mode
//Outputs   : pstFileHandle - Pointer to opened file
//Return    : true - File opened, false - File open failed
//Notes     : None
//*****************************************************************************
bool fileHandlerFileOpen(FILE **pstFileHandle, 
                         int8 *pcFileName, 
                         int8 *pcFileOpenMode)
{
    bool blReturn = false;

    if(*pstFileHandle == NULL)
    {
        if((pcFileName != NULL) && (pcFileOpenMode != NULL))
        {
            *pstFileHandle = fopen(pcFileName, pcFileOpenMode);
            if(*pstFileHandle != NULL)
            {
                blReturn = true;
            }
            else
            {
                printf("File Open failed\r\n");
            }
        }
        else
        {
            printf("Invalid input Parameters\r\n");
        }
    }
    else
    {
        printf("File Handle not Empty durinf FileOpen\r\n");
    }

    return blReturn;
}

//****************************.fileHandlerFileClose.***************************
//Purpose   : Close a file in given Handler
//Inputs    : pstFileHandle - File handler address
//Outputs   : None
//Return    : true - File closed, false - File close failed
//Notes     : Close a given file and reset the pointer value
//*****************************************************************************
bool fileHandlerFileClose(FILE **pstFileHandle)
{
    bool blReturn = false;

    if(*pstFileHandle != NULL)
    {
        if(fclose(*pstFileHandle) == RETURN_OK)
        {
            blReturn = true;
            pstFileHandle = NULL;
        }
        else
        {
            printf("File CLose failed\r\n");
        }
    }
    else
    {
        printf("File Handle is empty During FileClose\r\n");
    }

    return blReturn;
}

//**********************.fileHandlerFilePointerPosition.***********************
//Purpose   : Find current position of pointer in file
//Inputs    : pstFileHandle - File pointer to an opened file
//Outputs   : pulPosition - current position of pointer
//Return    : true - Success, false - failed
//Notes     : None
//*****************************************************************************
bool fileHandlerFilePointerPosition(FILE ** pstFileHandle, int32 *plPosition)
{
    bool blReturn = false;

    if((*pstFileHandle != NULL) && (plPosition != NULL))
    {
        *plPosition = ftell(*pstFileHandle);
        if(*plPosition != ERROR_VALUE)
        {
            blReturn = true;
        }
        else
        {
            printf("Failed to find pointer position\r\n");
        }
    }
    else
    {
        printf("Invalid parameters in File Pointer position\r\n");
    }

    return blReturn;
}

//***************************.fileHandlerFileSeek.*****************************
//Purpose   : Find current position of pointer in file
//Inputs    : pstFileHandle - File pointer to an opened file
//Inputs    : lOffset - Offset value we need to move pointer
//Inputs    : lPosition - Position from we need to move pointer
//Outputs   : None
//Return    : true - Pointer moved to given position, false - failed to move
//Notes     : None
//*****************************************************************************
bool fileHandlerFileSeek(FILE **pstFileHandle, int32 lOffset, int32 lPosition)
{
    bool blReturn = false;

    if(*pstFileHandle != NULL)
    {
        if(fseek(*pstFileHandle, lOffset, lPosition) == RETURN_OK)
        {
            blReturn = true;
        }
        else
        {
            printf("Fseek failed \r\n");
        }
    }

    return blReturn;
}

//****************************.fileHandlerFileWrite.***************************
//Purpose   : Write data to a file
//Inputs    : pcFileName - Name of file
//Inputs    : pcWriteData - Data to be written
//Inputs    : ulWriteSize - Size of data
//Inputs    : pcWriteMode - File open mode (write or append)
//Outputs   : None
//Return    : true - File write success, false - File write failed
//Notes     : Write a data to a given file
//*****************************************************************************
bool fileHandlerFileWrite(int8 *pcFileName, 
                          int8 *pcWriteData, 
                          uint32 ulWriteSize, 
                          int8 *pcWriteMode)
{
    bool blReturn = false;

    if((pcFileName != NULL) && 
       (pcWriteData != NULL) && 
       (pcWriteMode != NULL) && 
       (ulWriteSize != 0) && 
       (strcmp(OPEN_RD, pcWriteMode)) &&
       (strcmp(OPEN_R_PLUS, pcFileName)))
    {
        FILE *pstFileHandle = NULL;

        if(fileHandlerFileOpen(&pstFileHandle, 
                               pcFileName, 
                               pcWriteMode) == true)
        {
            if(fwrite(pcWriteData, 
                      sizeof(uint8), 
                      ulWriteSize, 
                      pstFileHandle) == ulWriteSize)
            {
                blReturn = true;
            }
            else
            {
                printf("File write failed\r\n");
            }

            fileHandlerFileClose(&pstFileHandle);
        }
        else
        {
            printf("File open failed for Writing\r\n");
        }
    }
    else
    {
        printf("Invalid parameters in File write\r\n");
    }

    return blReturn;
}

//***********************.fileHandlerFileWriteInPosition.**********************
//Purpose   : Write data to in given position of file
//Inputs    : pcFileName - Name of file
//Inputs    : pcWriteData - Data to be written
//Inputs    : ulWriteSize - Size of data
//Inputs    : ulPosition - Position of data
//Outputs   : None
//Return    : true - File write success, false - File write failed
//Notes     : Designed to write data of same type and same size into file
//          : Actual position is calculated by Position * sizeof(single data)
//*****************************************************************************
bool fileHandlerFileWriteInPosition(int8 *pcFileName, 
                                    int8 *pcWriteData, 
                                    uint32 ulWriteSize, 
                                    uint32 ulPosition)
{
    bool blReturn = false;

    if((pcFileName != NULL) && 
       (pcWriteData != NULL) && 
       (ulWriteSize != 0))
    {
        FILE *pstFileHandle = NULL;

        if(fileHandlerFileOpen(&pstFileHandle, 
                               pcFileName, 
                               OPEN_APND) == true)
        {
            fileHandlerFileSeek(&pstFileHandle, 
                                (ulPosition * ulWriteSize), 
                                SEEK_SET);
            if(fwrite(pcWriteData, 
                      sizeof(uint8), 
                      ulWriteSize, 
                      pstFileHandle) == ulWriteSize)
            {
                blReturn = true;
            }
            else
            {
                printf("File write failed in Write in Position\r\n");
            }

            fileHandlerFileClose(&pstFileHandle);
        }
        else
        {
            printf("File open failed for Writing in position\r\n");
        }
    }
    else
    {
        printf("Invalid parameters in File write in Position\r\n");
    }

    return blReturn;
}
//****************************.fileHandlerFileRead.****************************
//Purpose   : Read data from file
//Inputs    : pcFileName - Name of file
//Inputs    : ulReadSize - Size of data
//Inputs    : pcReadMode - File open mode (read or plus modes)
//Outputs   : Data read from file
//Return    : true - File read success, false - File read failed
//Notes     : Read a data from given file
//*****************************************************************************
bool fileHandlerFileRead(int8 *pcFileName, 
                        int8 *pcReadData, 
                        uint32 ulReadSize, 
                        int8 *pcReadMode)
{
    bool blReturn = false;

    if((pcFileName != NULL) && 
       (pcReadData != NULL) && 
       (pcReadMode != NULL) && 
       (ulReadSize != 0) && 
       (strcmp(OPEN_WR, pcReadMode)) && 
       (strcmp(OPEN_W_PLUS, pcReadMode)))
    {
        FILE *pstFileHandle = NULL;

        if(fileHandlerFileOpen(&pstFileHandle, pcFileName, pcReadMode) == true)
        {
            if(fread(pcReadData, 
                     sizeof(uint8), 
                     ulReadSize, 
                     pstFileHandle) == ulReadSize)
            {
                blReturn = true;
            }
            else
            {
                printf("File write failed\r\n");
            }

            fileHandlerFileClose(&pstFileHandle);
        }
        else
        {
            printf("File open failed for Writing\r\n");
        }
    }
    else
    {
        printf("Invalid parameters in File write\r\n");
    }

    return blReturn;
}

//***********************.fileHandlerFileReadFromPosition.*********************
//Purpose   : Read data from given position file
//Inputs    : puFileName - Name of file
//Inputs    : ulWriteSize - Size of data
//Inputs    : ulPosition - Position of data need to be read
//Outputs   : Data read from file
//Return    : true - File read success, false - File read failed
//Notes     : Designed to read data of same type and same size from file
//          : Actual position is calculated by Position * sizeof(single data)
//*****************************************************************************
bool fileHandlerFileReadFromPosition(int8 *pcFileName, 
                                     int8 *pcReadData, 
                                     uint32 ulWriteSize, 
                                     uint32 ulPosition)
{
    bool blReturn = false;

    if((pcFileName != NULL) && 
       (pcReadData != NULL) && 
       (ulWriteSize != 0))
    {
        FILE *pstFileHandle = NULL;

        if(fileHandlerFileOpen(&pstFileHandle, 
                               pcFileName, 
                               OPEN_RD) == true)
        {
            fileHandlerFileSeek(&pstFileHandle, 
                                (ulPosition * ulWriteSize), 
                                SEEK_SET);
            if(fread(pcReadData, 
                     sizeof(uint8), 
                     ulWriteSize, 
                     pstFileHandle) == ulWriteSize)
            {
                blReturn = true;
            }
            else
            {
                printf("File read failed in read from Position\r\n");
            }

            fileHandlerFileClose(&pstFileHandle);
        }
        else
        {
            printf("File open failed for read in position\r\n");
        }
    }
    else
    {
        printf("Invalid parameters in File read in Position\r\n");
    }

    return blReturn;
}

//****************************.fileHandlerFileClear.***************************
//Purpose   : Clear data from file
//Inputs    : puFileName - Name of file
//Outputs   : None
//Return    : true - File clear success, false - File clear failed
//Notes     : Clear all data from given file
//*****************************************************************************
bool fileHandlerFileRemove(int8 *pcFileName)
{
    bool blReturn = false;

    if(pcFileName != NULL)
    {
       if(remove(pcFileName) == RETURN_OK)
        {
            blReturn = true;
            printf("File %s removed\r\n", pcFileName);
        }
        else
        {
            printf("Failed to remove file for clearing\r\n");
        }
    }
    else
    {
        printf("Invalid parameters in File remove\r\n");
    }

    return blReturn;
}

//***************************.fileHandlerFileCreate.***************************
//Purpose   : Create a new file
//Inputs    : puFileName - Name of file
//Outputs   : None
//Return    : true - File Create success, false - File create failed
//Notes     : Create a new file
//*****************************************************************************
bool fileHandlerFileCreate(int8 *pcFileName)
{
    bool blReturn = false;

    if(pcFileName != NULL)
    {
        FILE *pstFileHandle = NULL;

        if(fileHandlerFileOpen(&pstFileHandle, pcFileName, OPEN_WR) == true)
        {
            blReturn = true;
            fileHandlerFileClose(&pstFileHandle);
        }
        else
        {
            printf("Failed to Create file '%s'\r\n", pcFileName);
        }
    }

    return blReturn;
}

//*************************.fileHandlerFileCheckExist.*************************
//Purpose   : Check a file exist or not
//Inputs    : puFileName - Name of file
//Outputs   : None
//Return    : true - File already exist, false - File does no exist
//Notes     : None
//*****************************************************************************
bool fileHandlerFileCheckExist(int8 *pcFileName)
{
    bool blReturn = false;

    if(pcFileName != NULL)
    {
        FILE *pstFileHandle = NULL;

        if(fileHandlerFileOpen(&pstFileHandle, pcFileName, OPEN_RD) == true)
        {
            fileHandlerFileClose(&pstFileHandle);
            blReturn = true;
        }
    }

    return blReturn;

}

//**************************.fileHandlerInitialCheck.**************************
//Purpose   : Check a file exist or not and clear or create it
//Inputs    : None
//Outputs   : None
//Return    : true - Success, false - Failed
//Notes     : Check for files exixtance. Create if file doesn't exist
//            Clear if file exist
//*****************************************************************************
bool fileHandlerInitialCheck()
{
    bool blReturn = false;

    if(fileHandlerFileCheckExist(FILE_NAME) == true)
    {
        if(fileHandlerFileRemove(FILE_NAME) != true)
        {
            printf("Failed to remove %s file\r\n",FILE_NAME);
        }
    }
    if(fileHandlerFileCreate(FILE_NAME) != true)
    {
        printf("File %s create failed\r\n",FILE_NAME);
        exit(-1);
    }
    else
    {
        blReturn = true;
    }

    return blReturn;
    
}