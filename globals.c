/* Global variables used by the different modules */

#include "parkease.h"

/* Registered vehicles (filled by fileio.c -> loadVehicles) */
struct Vehicle vehicles[MAX_VEHICLES];
int vehicleCount = 0;

/* All parking slots (created by slots.c -> initializeSlots) */
struct Slot slots[TOTAL_SLOTS];

/* Waiting queue: fixed-size array */
char queueRegNo[MAX_QUEUE][16];
time_t queueWaitTime[MAX_QUEUE];
int queueCount = 0;

/* Transaction records: fixed-size array */
struct Transaction transactions[MAX_TRANSACTIONS];
