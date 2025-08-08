//**************************** possixOperation ********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : possixOperation.h
// Summary  : Includes necessary possix operations
// Note     : None
// Author   : Pragalf T Jose
// Date     : 07/08/2025
//
//*****************************************************************************
#ifndef _POSSIX_OPERATION_H
#define _POSSIX_OPERATION_H

//******************************* Include Files *******************************
#include "possixHandler.h"

//******************************* Global Types ********************************

//***************************** Global Constants ******************************
#define POLER_SEM               "PollerSemaphore"
#define LOGER_SEM               "LoggerSemaphore"
#define TRNSFR_SEM              "TransferSemaphore"
#define INPUT_MQ_NAME           "/InputMessageQueue"
#define OUTPUT_MQ_NAME          "/OutputMessageQueue"
#define MODE_FLAGS              666
#define MSG_PRIORITY            1
#define MAX_MESSAGES            10
#define MAX_MSG_LEN             64

//***************************** Global Variables ******************************

//**************************** Forward Declarations ***************************

void possixOperationSystemInit();
void possixOperationSystemDeinit();
bool possixOperationInputMQueueOpen();
bool possixOperationOutputMQueueOpen();
bool possixOperationInputMQueueClose();
bool possixOperationOutputMQueueClose();
bool possixOperationInputMessageSend(void *pvMessage);
bool possixOperationOutputMessageSend(void *pvMessage);
bool possixOperationInputMessageReceive(void *pvMessage);
bool possixOperationOutputMessageReceive(void *pvMessage);
bool possixOperationFileMutexCreate();
bool possixOperationConditionalMutexCreate();
bool possixOperationFileMutexDelete();
bool possixOperationsConditionalMutexDelete();
bool possixOperationFileMutexLock();
bool possixOperationFileMutexUnlock();
bool possixOperationConditionalMutexLock();
bool possixOperationConditionalMutexUnlock();
bool possixOperationPollerSemaphoreOpen();
bool possixOperationTransportSemaphoreOpen();
bool possixOperationLoggerSemaphoreOpen();
bool possixOperationPollerSemaphoreClose();
bool possixOperationTransferSemaphoreClose();
bool possixOperationLoggerSemaphoreClose();
bool possixOperationPollerSemaphoreWait();
bool possixOperationTransportSemaphoreWait();
bool possixOperationLoggerSemaphoreWait();
bool possixOperationPollerSemaphorePost();
bool possixOperationTransportSemaphorePost();
bool possixOperationLoggerSemaphorePost();
bool possixOperationConditionalVarInit();
bool possixOperationConditionalVarDelete();
bool possixOperationConditionalVarWait();
bool possixOperationConditionalVarSignal();
bool possixOperationConditionalVarBroadcast();
bool possixOperationConditionSetValue();
bool possixOperationConditionClearValue();
bool possixOperationConditionCheckValue();

#endif // _POSSIX_OPERATION_H
// EOF