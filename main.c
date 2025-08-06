//**************************** ProjectConsole *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : main.c
// Summary  : Creating two threads to do file operation
// Note     : None
// Author   : Pragalf T Jose
// Date     : 04/08/2025
//
//*****************************************************************************

//***************************** Include Files *********************************
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>
#include <fcntl.h>              // For O_CREAT, O_EXCL
#include <sys/stat.h>           // For mode constants
#include <Common.h>
#include <fileHandler.h>

//***************************** Global Types **********************************

//***************************** Global Constants ******************************
#define MAX_VALUE               999
#define MIN_VALUE               1
#define STRING_LEN              64
#define FILE_NAME               "TestDoc.txt"
#define DATA_SIZE               26
#define SEM_NAME1               "ProducerSemaphore"
#define SEM_NAME2               "ConsumerSemaphore"
#define MQ_NAME                 "/MessageQueue"
#define MODE_FLAGS              666
#define MSG_PRIORITY            1
#define MAX_MESSAGES            10
#define MAX_MSG_LEN             64

//***************************** Global Variables ******************************
uint32 ulDataCount = 0;
pthread_mutex_t mFileMutex = {0};
mqd_t pmMessageQueue = 0;
sem_t *psProducerSemaphore = NULL;
sem_t *psConsumerSemaphore = NULL;

//******************************.ProducerThread.*******************************
//Purpose : Generate a random number
//Inputs : None
//Outputs : None
//Return : None
//Notes : None
//*****************************************************************************
void* ProducerThread()
{
    uint16 unRandomNumber = 0;
    uint8 pucRandomNumberString[STRING_LEN] = {0};

    sem_wait(psProducerSemaphore);
    while(TRUE)
    {
        unRandomNumber = (rand() % (MAX_VALUE- MIN_VALUE + 1)) + MIN_VALUE;
        memset(pucRandomNumberString, 0, sizeof(pucRandomNumberString));
        sprintf(pucRandomNumberString, 
                "From Producer task : %03d\r\n", 
                unRandomNumber);
        pthread_mutex_lock(&mFileMutex);
        fileHandlerFileWriteInPosition(FILE_NAME, 
                                       pucRandomNumberString, 
                                       DATA_SIZE, 
                                       ulDataCount);
        ulDataCount++;
        pthread_mutex_unlock(&mFileMutex);
        sem_wait(psProducerSemaphore);
    }

    pthread_exit((void *)0);
}

//******************************.ConsumerThread.*******************************
//Purpose : Print the random number from Producer
//Inputs : None
//Outputs : None
//Return : None
//Notes : None
//*****************************************************************************
void* ConsumerThread()
{
    uint32 ulTemporaryDataCount = ulDataCount;
    uint8 pucReadData[STRING_LEN] = {0};

    while(TRUE)
    {
        pthread_testcancel();
        if(ulDataCount != ulTemporaryDataCount)
        {
            memset(pucReadData, 0, sizeof(pucReadData));
            if(mq_receive(pmMessageQueue, 
                          pucReadData, 
                          MAX_MSG_LEN, 
                          NULL) == ERROR_VALUE)
            {
                printf("Message Receive Failed\r\n");
            }
            printf("%s", pucReadData);
            memset(pucReadData, 0, sizeof(pucReadData));
            pthread_mutex_lock(&mFileMutex);
            fileHandlerFileReadFromPosition(FILE_NAME, 
                                     pucReadData, 
                                     DATA_SIZE, 
                                     ulDataCount-1);
            printf("In Counsumer - %s",pucReadData);
            ulTemporaryDataCount = ulDataCount;
            pthread_mutex_unlock(&mFileMutex);
            sem_post(psConsumerSemaphore);
        }
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
    uint8 ucInputValue = 0;
    uint8 pucMessageString[STRING_LEN] = {0};
    pthread_t ptFirstThread = 0;
    pthread_t ptSecondThread = 0;

    struct mq_attr stAttributes = {
                                 .mq_flags = 0,
                                 .mq_maxmsg = MAX_MESSAGES, 
                                 .mq_msgsize = MAX_MSG_LEN, 
                                 .mq_curmsgs = 0
                                };

    pmMessageQueue = mq_open(MQ_NAME, 
                             O_CREAT | O_RDWR, 
                             MODE_FLAGS, 
                             &stAttributes);
    printf("pmMessageQueue : %d\r\n", pmMessageQueue);
    if(pmMessageQueue == ERROR_VALUE)
    {
        printf("Message Queue Creation failed\r\n");
        exit(-1);
    }
    if(fileHandlerFileCheckExist(FILE_NAME) == true)
    {
        if(fileHandlerFileRemove(FILE_NAME) != true)
        {
            printf("Failed to remove %s file\r\n",FILE_NAME);
        }

        if(fileHandlerFileCreate(FILE_NAME) != true)
        {
            printf("File %s create failed\r\n",FILE_NAME);
            exit(-1);
        }
    }
    psProducerSemaphore = sem_open(SEM_NAME1, O_CREAT | O_EXCL, MODE_FLAGS, 0);
    if(psProducerSemaphore == SEM_FAILED)
    {
        sem_unlink(SEM_NAME1);
        psProducerSemaphore = sem_open(SEM_NAME1, O_CREAT, MODE_FLAGS, 0);;
        if(psProducerSemaphore == SEM_FAILED)
        {
            printf("Producer Semaphore Creation failed\r\n");
            exit(-1);
        }
    }
    psConsumerSemaphore = sem_open(SEM_NAME2, O_CREAT | O_EXCL, MODE_FLAGS, 1);
    if(psConsumerSemaphore == SEM_FAILED)
    {
        sem_unlink(SEM_NAME2);
        psConsumerSemaphore = sem_open(SEM_NAME2, O_CREAT, MODE_FLAGS, 1);;
        if(psConsumerSemaphore == SEM_FAILED)
        {
            printf("Consumer Semaphore Creation failed\r\n");
            exit(-1);
        }
    }
    if(pthread_mutex_init(&mFileMutex, NULL) != RETURN_OK)
    {
        printf("Mutex Initialisation failed\r\n");
        exit(-1);
    }
    if(pthread_create(&ptFirstThread, 
                      NULL, 
                      &ProducerThread, 
                      NULL) != RETURN_OK)
    {
        printf("Producer Thread Creation failed\r\n");
        exit(-1);
    }
    if(pthread_create(&ptSecondThread, 
                      NULL, 
                      &ConsumerThread, 
                      NULL) != RETURN_OK)
    {
        printf("Consumer Thread Creation failed\r\n");
        exit(-1);
    }

    while(TRUE)
    {
        sem_wait(psConsumerSemaphore);
        printf("Enter a number between 1 to 10 : ");
        scanf("%[^\n]c", &ucInputValue);
        getchar();
        if(ucInputValue < '0' || ucInputValue > '9')
        {
            printf("Exit from loop\r\n");
            break;
        }
        memset(pucMessageString, 0, sizeof(pucMessageString));
        sprintf(pucMessageString, 
                "Key Pressed in  Main : %c\r\n", 
                ucInputValue);
        if(mq_send(pmMessageQueue, 
                   pucMessageString, 
                   MAX_MSG_LEN, 
                   MSG_PRIORITY) == ERROR_VALUE)
        {
            printf("Message Send Failed\r\n");
        }
        sem_post(psProducerSemaphore);
    }

    pthread_cancel(ptFirstThread);          // Cancel reruest for First thread
    // pthread_join(ptFirstThread,NULL);       // Wait for first thread to exit
    printf("First Thread Exited\r\n");
    pthread_cancel(ptSecondThread);         // Cancel request for second thread
    pthread_join(ptSecondThread,NULL);      // Wait for second thread to exit
    printf("Second Thread Exited\r\n");

    sem_close(psProducerSemaphore);         // Close the semaphore descriptor
    sem_unlink(SEM_NAME1);                  // Unlink the named semaphore
    sem_close(psConsumerSemaphore);         // Close the semaphore descriptor
    sem_unlink(SEM_NAME2);                  // Unlink the named semaphore
    mq_close(pmMessageQueue);               // Close Message queue
    mq_unlink(MQ_NAME);                     // Unlink Message queue
    pthread_mutex_destroy(&mFileMutex);     // Delete mutex after use

}
