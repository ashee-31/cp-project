/* Waiting queue function declarations */

#ifndef QUEUE_H
#define QUEUE_H

#include <time.h>

/* Add a vehicle at the REAR of the waiting queue */
void enqueueVehicle(const char *regNo, time_t waitTime);

/* 1-based position of a vehicle in the queue; 0 if not waiting */
int  queuePosition(const char *regNo);

/* Number of vehicles currently waiting */
int  queueLength(void);

/* Remove a specific vehicle from the queue (it left / gave up) */
void removeFromQueue(const char *regNo);

/* Clear the whole queue (used by the admin reset) */
void freeQueue(void);

/* Menu option: display the queue from front to rear */
void displayQueue(void);

/* A slot has just been freed: give it to the first waiting vehicle
   of the matching type. Returns 1 if somebody got the slot.         */
int  assignWaitingVehicle(int slotIndex);

#endif /* QUEUE_H */
