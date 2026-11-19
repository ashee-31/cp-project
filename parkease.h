/* Shared constants, structures, globals, and utility functions */

#ifndef PARKEASE_H
#define PARKEASE_H

#include <time.h>

/* Configuration */

/* Number of parking slots for each vehicle type */
#define TOTAL_BIKE_SLOTS    20
#define TOTAL_CAR_SLOTS     15
#define TOTAL_HEAVY_SLOTS    5
#define TOTAL_SLOTS (TOTAL_BIKE_SLOTS + TOTAL_CAR_SLOTS + TOTAL_HEAVY_SLOTS)

/* Fee rates in INR (Rs.). "First rate" covers the 1st hour, "hourly
   rate" is charged for every additional hour (any partial hour counts
   as a full hour). A stay shorter than GRACE_MINUTES is free.
   Bike : 20 / 10    Car : 40 / 20    Heavy : 60 / 30                 */
#define BIKE_FIRST_RATE     20.0
#define BIKE_HOURLY_RATE    10.0
#define CAR_FIRST_RATE      40.0
#define CAR_HOURLY_RATE     20.0
#define HEAVY_FIRST_RATE    60.0
#define HEAVY_HOURLY_RATE   30.0
#define GRACE_MINUTES       15

#define ADMIN_PASSWORD      "admin123"
#define MAX_VEHICLES        500
#define MAX_QUEUE           500
#define MAX_TRANSACTIONS    5000

/* Data file names (the File Handling Module works on these four) */
#define DATA_VEHICLES   "vehicles.dat"
#define DATA_SLOTS      "slots.dat"
#define DATA_TXN        "transactions.dat"
#define DATA_QUEUE      "queue.dat"

/* Vehicle type codes */
#define TYPE_BIKE   1
#define TYPE_CAR    2
#define TYPE_HEAVY  3

/* Structures used by the program */

/* A registered vehicle (Vehicle Records file) */
struct Vehicle {
    char   regNo[16];        /* registration number, e.g. "MH12AB1234" */
    char   ownerName[32];    /* owner's full name                      */
    int    type;             /* TYPE_BIKE / TYPE_CAR / TYPE_HEAVY      */
};

/* One parking slot. A slot only accepts vehicles of its own type.
   Labels: B01..B20 (bike), C01..C15 (car), H01..H05 (heavy).        */
struct Slot {
    int    slotId;           /* 1..TOTAL_SLOTS                         */
    int    type;             /* vehicle type the slot can hold         */
    int    occupied;         /* 1 = occupied, 0 = free                 */
    char   regNo[16];        /* reg. no. of parked vehicle ("" if free)*/
    time_t entryTime;        /* when the vehicle entered (time.h)      */
};

/* One completed parking transaction, saved when a vehicle exits
   (Transaction Records file)                                        */
struct Transaction {
    char   regNo[16];
    int    slotId;
    time_t entryTime;
    time_t exitTime;
    double amount;           /* parking fee collected in Rs.           */
};

/* Global program data */

extern struct Vehicle vehicles[MAX_VEHICLES];  /* registered vehicles */
extern int vehicleCount;                       /* how many are loaded */
extern struct Slot slots[TOTAL_SLOTS];         /* all parking slots   */

/* Waiting queue (array implementation) */
extern char queueRegNo[MAX_QUEUE][16];
extern time_t queueWaitTime[MAX_QUEUE];
extern int queueCount;

/* Loaded transaction records */
extern struct Transaction transactions[MAX_TRANSACTIONS];

/* Utility functions */

/* Safe keyboard input */
void readLine(char *buf, int size);
void trimStr(char *s);
void readString(const char *prompt, char *out, int size);
void readName(const char *prompt, char *out, int size);
int  readInt(const char *prompt, int min, int max);

/* Small string helpers */
void toUpperStr(char *s);
int  isYes(const char *s);

/* Screen and formatting helpers */
void pauseScreen(void);
void printTitle(const char *title);
const char *typeName(int type);
void slotLabel(int slotId, char *out /* size >= 6 */);
void formatTime(time_t t, char *out /* size >= 20 */);

#endif /* PARKEASE_H */
