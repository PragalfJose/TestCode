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
//Outputs   : pFileHandle - Pointer to opened file
//Return    : true - File opened, false - File open failed
//Notes     : None
//*****************************************************************************
bool fileHandlerFileOpen(FILE **pFileHandle, 
                         uint8 *pucFileName, 
                         uint8 *pucFileOpenMode)
{
    bool blReturn = false;

    if(*pFileHandle == NULL)
    {
        if((pucFileName != NULL) && (pucFileOpenMode != NULL))
        {
            *pFileHandle = fopen(pucFileName, pucFileOpenMode);
            if(*pFileHandle != NULL)
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
//Inputs    : pFileHandle - File handler address
//Outputs   : None
//Return    : true - File closed, false - File close failed
//Notes     : Close a given file and reset the pointer value
//*****************************************************************************
bool fileHandlerFileClose(FILE **pFileHandle)
{
    bool blReturn = false;

    if(*pFileHandle != NULL)
    {
        if(fclose(*pFileHandle) == RETURN_OK)
        {
            blReturn = true;
            pFileHandle = NULL;
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
//Inputs    : pFileHandle - File pointer to an opened file
//Outputs   : pulPosition - current position of pointer
//Return    : true - Success, false - failed
//Notes     : None
//*****************************************************************************
bool fileHandlerFilePointerPosition(FILE ** pFileHandle, int32 *plPosition)
{
    bool blReturn = false;

    if((*pFileHandle != NULL) && (plPosition != NULL))
    {
        *plPosition = ftell(*pFileHandle);
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

//**********************.fileHandlerFilePointerPosition.***********************
//Purpose   : Find current position of pointer in file
//Inputs    : pFileHandle - File pointer to an opened file
//Inputs    : pFileHandle - File pointer to an opened file
//Inputs    : pFileHandle - File pointer to an opened file
//Outputs   : None
//Return    : true - Pointer moved to given position, false - failed to move
//Notes     : None
//*****************************************************************************
bool fileHandlerFileSeek(FILE **pFileHandle, int32 lOffset, int32 lPosition)
{
    bool blReturn = false;

    if(*pFileHandle != NULL)
    {
        if(fseek(*pFileHandle, lOffset, lPosition) == RETURN_OK)
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
//Inputs    : puFileName - Name of file
//Inputs    : puWriteData - Data to be written
//Inputs    : ulWriteSize - Size of data
//Inputs    : puWriteMode - File open mode (write or append)
//Outputs   : None
//Return    : true - File write success, false - File write failed
//Notes     : Write a data to a given file
//*****************************************************************************
bool fileHandlerFileWrite(uint8 *pucFileName, 
                          uint8 *pucWriteData, 
                          uint32 ulWriteSize, 
                          uint8 *pucWriteMode)
{
    bool blReturn = false;

    if((pucFileName != NULL) && 
       (pucWriteData != NULL) && 
       (pucWriteMode != NULL) && 
       (ulWriteSize != 0) && 
       (strcmp(OPEN_RD, pucWriteMode)) &&
       (strcmp(OPEN_R_PLUS, pucFileName)))
    {
        FILE *pFileHandle = NULL;

        if(fileHandlerFileOpen(&pFileHandle, 
                               pucFileName, 
                               pucWriteMode) == true)
        {
            if(fwrite(pucWriteData, 
                      sizeof(uint8), 
                      ulWriteSize, 
                      pFileHandle) == ulWriteSize)
            {
                blReturn = true;
            }
            else
            {
                printf("File write failed\r\n");
            }

            fileHandlerFileClose(&pFileHandle);
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
//Inputs    : puFileName - Name of file
//Inputs    : puWriteData - Data to be written
//Inputs    : ulWriteSize - Size of data
//Inputs    : ulPosition - Position of data
//Outputs   : None
//Return    : true - File write success, false - File write failed
//Notes     : Designed to write data of same type and same size into file
//          : Actual position is calculated by Position * sizeof(single data)
//*****************************************************************************
bool fileHandlerFileWriteInPosition(uint8 *pucFileName, 
                                    uint8 *pucWriteData, 
                                    uint32 ulWriteSize, 
                                    uint32 ulPosition)
{
    bool blReturn = false;

    if((pucFileName != NULL) && 
       (pucWriteData != NULL) && 
       (ulWriteSize != 0) && 
       (ulPosition >= ZERO))
    {
        FILE *pFileHandle = NULL;

        if(fileHandlerFileOpen(&pFileHandle, 
                               pucFileName, 
                               OPEN_APND) == true)
        {
            fileHandlerFileSeek(&pFileHandle, 
                                (ulPosition * ulWriteSize), 
                                SEEK_SET);
            if(fwrite(pucWriteData, 
                      sizeof(uint8), 
                      ulWriteSize, 
                      pFileHandle) == ulWriteSize)
            {
                blReturn = true;
            }
            else
            {
                printf("File write failed in Write in Position\r\n");
            }

            fileHandlerFileClose(&pFileHandle);
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
//Inputs    : puFileName - Name of file
//Inputs    : ulWriteSize - Size of data
//Inputs    : puWriteMode - File open mode (read or plus modes)
//Outputs   : Data read from file
//Return    : true - File read success, false - File read failed
//Notes     : Read a data from given file
//*****************************************************************************
bool fileHandlerFileRead(uint8 *pucFileName, 
                          uint8 *pucWriteData, 
                          uint32 ulWriteSize, 
                          uint8 *pucWriteMode)
{
    bool blReturn = false;

    if((pucFileName != NULL) && 
       (pucWriteData != NULL) && 
       (pucWriteMode != NULL) && 
       (ulWriteSize != 0) && 
       (strcmp(OPEN_WR, pucWriteMode)) && 
       (strcmp(OPEN_W_PLUS, pucWriteMode)))
    {
        FILE *pFileHandle = NULL;

        if(fileHandlerFileOpen(&pFileHandle, pucFileName, pucWriteMode) == true)
        {
            if(fread(pucWriteData, 
                     sizeof(uint8), 
                     ulWriteSize, 
                     pFileHandle) == ulWriteSize)
            {
                blReturn = true;
            }
            else
            {
                printf("File write failed\r\n");
            }

            fileHandlerFileClose(&pFileHandle);
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
bool fileHandlerFileReadFromPosition(uint8 *pucFileName, 
                                     uint8 *pucWriteData, 
                                     uint32 ulWriteSize, 
                                     uint32 ulPosition)
{
    bool blReturn = false;

    if((pucFileName != NULL) && 
       (pucWriteData != NULL) && 
       (ulWriteSize != 0) && 
       (ulPosition >= ZERO))
    {
        FILE *pFileHandle = NULL;

        if(fileHandlerFileOpen(&pFileHandle, 
                               pucFileName, 
                               OPEN_RD) == true)
        {
            fileHandlerFileSeek(&pFileHandle, 
                                (ulPosition * ulWriteSize), 
                                SEEK_SET);
            if(fread(pucWriteData, 
                     sizeof(uint8), 
                     ulWriteSize, 
                     pFileHandle) == ulWriteSize)
            {
                blReturn = true;
            }
            else
            {
                printf("File read failed in read from Position\r\n");
            }

            fileHandlerFileClose(&pFileHandle);
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
bool fileHandlerFileRemove(uint8 *pucFileName)
{
    bool blReturn = false;

    if(pucFileName != NULL)
    {
       if(remove(pucFileName) == RETURN_OK)
        {
            blReturn = true;
            printf("File %s removed\r\n", pucFileName);
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
bool fileHandlerFileCreate(uint8 *pucFileName)
{
    bool blReturn = false;

    if(pucFileName != NULL)
    {
        FILE *pFileHandle = NULL;

        if(fileHandlerFileOpen(&pFileHandle, pucFileName, OPEN_WR) == true)
        {
            blReturn = true;
            fileHandlerFileClose(&pFileHandle);
        }
        else
        {
            printf("Failed to Create file '%s'\r\n", pucFileName);
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
bool fileHandlerFileCheckExist(uint8 *pucFileName)
{
    bool blReturn = false;

    if(pucFileName != NULL)
    {
        FILE *pFileHandle = NULL;

        if(fileHandlerFileOpen(&pFileHandle, pucFileName, OPEN_RD) == true)
        {
            fileHandlerFileClose(&pFileHandle);
            blReturn = true;
        }
    }

    return blReturn;

}
