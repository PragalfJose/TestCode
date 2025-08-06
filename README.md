POSSIX Familiaisation
---------------------
Program includes creation of multiple threads and implementing Mutex, Semaphores and Message Queue.

Main thhread
------------
 Create multiple threads and initialise Mutex and Semaphores. Also create Message queue for communication. 
  * Wait for Consumer semaphore. (Initial value is 1)
  * Waiting for a keypres from user
  * If key is between 0 and 10 post a semaphore for Proucer thread
  * Else exit from program and clear all threads
  * Send pressed Key value to Consumer thread through Message queue

Producer Thread
---------------
 Produce a random value when Keypress is detected in Main thread.
  * Wait for semaphore from main thread.
  * If semaphore is posted, generate a random number and write it to a shared file.
  * Uses mutex for file writing since it is a shared memory.
  * Lock mutex before writing and unloack after writing.
  * Increment a data Count.
    
Consumer Thread
---------------
 Read data from file.
   * If data count changed from previous value, data read from file.
   * Use mutex for file read operation.
   * Mutex locked before reading and unlocked after reading.
   * Read message from Main thread and print it along with data.
   * Post Consumer semaphore.
