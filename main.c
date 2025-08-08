//**************************** ProjectConsole *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : main.c
// Summary  : Creating three threads to do log and control GPIO Status
// Note     : None
// Author   : Pragalf T Jose
// Date     : 04/08/2025
//
//*****************************************************************************

//***************************** Include Files *********************************
#include "Common.h"
#include "fileHandler.h"
#include "possixOperation.h"

//***************************** Global Types **********************************

//***************************** Global Constants ******************************

//***************************** Global Variables ******************************
static uint32 ulDataCount = 0;

//*******************************.PollerThread.********************************
//Purpose   : Detect GPIO input state change
//Inputs    : None
//Outputs   : None
//Return    : None
//Notes     : Check GPIO for input changes and notify TransportThread 
//          : through Message Queue
//*****************************************************************************
void* PollerThread()
{
    uint8 ucInputValue = 0;
    int8 pcMessageString[STRING_LEN] = {0};
    int8 pcReadData[STRING_LEN] = {0};

    while(1)
    {
        possixOperationPollerSemaphoreWait();
        printf("Enter a number between 1 to 10 : ");
        scanf("%[^\n]c", &ucInputValue);
        getchar();
        if(ucInputValue < '0' || ucInputValue > '9')
        {
            printf("Exit from loop\r\n");
            break;
        }
        possixOperationInputMessageReceive(pcReadData);
        memset(pcMessageString, 0, sizeof(pcMessageString));
        sprintf((char*)pcMessageString, 
                "Key Pressed in  Poller : %c\r\n", 
                ucInputValue);
        possixOperationInputMessageSend(pcMessageString);
        possixOperationConditionalMutexLock();
        possixOperationConditionSetValue();
        possixOperationConditionalVarBroadcast();
        possixOperationConditionalMutexUnlock();

    }
    
    pthread_exit((void *)0);

}

//******************************.TransportThread.******************************
//Purpose   : Transfer message from PollerThread to LoggerThread
//Inputs    : None
//Outputs   : None
//Return    : None
//Notes     : Receive message from PollerThread and send acknowledge to it.
//          : Send a message to Loger thread regarding GPIO state change
//*****************************************************************************
void* TransportThread()
{
    int8 pcReadData[STRING_LEN] = {0};
    int8 pcMessageString[MAX_STR_LEN] = {0};

    while(TRUE)
    {
        possixOperationConditionalMutexLock();
        while(possixOperationConditionCheckValue() == false)
        {
            possixOperationConditionalVarWait();
        } 
        possixOperationConditionClearValue();
        possixOperationConditionalMutexUnlock();
        memset(pcReadData, 0, sizeof(pcReadData));
        possixOperationInputMessageReceive(pcReadData);
        if(strstr((char*)pcReadData, "Key Pressed"))
        {
            memset(pcMessageString, 0, sizeof(pcMessageString));
            sprintf((char*)pcMessageString, "Key Press Acknowledged\r\n");
            possixOperationInputMessageSend(pcMessageString);
            memset(pcMessageString, 0, sizeof(pcMessageString));
            possixOperationOutputMessageReceive(pcMessageString);
            memset(pcMessageString, 0, sizeof(pcMessageString));
            sprintf((char*)pcMessageString, "%s\r\n",pcReadData);
            possixOperationOutputMessageSend(pcMessageString);
        }
        possixOperationLoggerSemaphorePost();
    }
    
    pthread_exit((void *)0);

}

//******************************.LoggerThread.*********************************
//Purpose   : Controls state of GPIO according to message from TramsportThread
//Inputs    : None
//Outputs   : None
//Return    : None
//Notes     : Receive a message from TransportThread and change GPIO state.
//          : Send an acknowledge to TransportThread 
//*****************************************************************************
void* LoggerThread()
{
    int8 pcMessageString[MAX_STR_LEN] = {0};
    int8 pcReadData[STRING_LEN] = {0};

    while(TRUE)
    {
        possixOperationLoggerSemaphoreWait();
        memset(pcReadData, 0, sizeof(pcReadData));
        possixOperationOutputMessageReceive(pcReadData);
        printf("%s", pcReadData);
        if(strstr((char*)pcReadData, "Key Pressed"))
        {
            memset(pcMessageString, 0, sizeof(pcMessageString));
            sprintf((char*)pcMessageString, 
                    "%04d  : %s", 
                    ulDataCount, pcReadData);
            ulDataCount++;
            fileHandlerFileWrite(FILE_NAME, 
                                 pcMessageString, 
                                 DATA_SIZE, 
                                 OPEN_APND);
            memset(pcReadData, 0, sizeof(pcReadData));
            sprintf((char*)pcReadData, "Input Acknowledged\r\n");
            possixOperationOutputMessageSend(pcReadData);
        }
        possixOperationPollerSemaphorePost();
    }

    pthread_exit((void *)0);
}
//******************************.main.*****************************************
//Purpose   : Multi thread program to perform file operations
//Inputs    : None
//Outputs   : None
//Return    : None
//Notes     : Perform file operations using basic POSIX fetures
//*****************************************************************************
int main()
{
    pthread_t ulPollerThread = 0;
    pthread_t ulTransportThread = 0;
    pthread_t ulLoggerThread = 0;

    fileHandlerInitialCheck();
    possixOperationSystemInit();
    if(possixHandlerThreadCreate(&ulPollerThread, PollerThread, NULL) != true)
    {
        printf("Poller Thread creation failed\r\n");
        exit(ERROR_VALUE);
    }
    if(possixHandlerThreadCreate(&ulLoggerThread, LoggerThread, NULL) != true)
    {
        printf("Logger Thread creation failed\r\n");
        exit(ERROR_VALUE);
    }
    if(possixHandlerThreadCreate(&ulTransportThread, 
                                 TransportThread, 
                                 NULL) != true)
    {
        printf("Transport Thread creation failed\r\n");
        exit(ERROR_VALUE);
    }
  
    possixHandlerThreadJoin(ulPollerThread);
    possixHandlerThreadCancel(ulTransportThread);
    possixHandlerThreadJoin(ulTransportThread);
    possixHandlerThreadCancel(ulLoggerThread);
    possixHandlerThreadJoin(ulLoggerThread);
    possixOperationSystemDeinit();
    printf("Exiting Program\r\n");
}
