/* File input and output function declarations */

#ifndef FILEIO_H
#define FILEIO_H

#include <time.h>

/* Make sure a data file exists (creates an empty one if missing) */
void ensureFile(const char *name);

/* Vehicle records */
void saveVehicles(void);
void loadVehicles(void);

/* Parking slot records. loadSlots() returns 1 if a valid slot file
   existed, 0 otherwise (first run or corrupt file: re-initialize).  */
void saveSlots(void);
int  loadSlots(void);

/* Transaction records */
void appendTransaction(const char *regNo, int slotId, time_t entryTime,
                       time_t exitTime, double amount);
/* Load all transactions into the shared transactions[] array.
   Sets *countOut. */
void loadTransactions(int *countOut);

/* Waiting queue records */
void saveQueue(void);
void loadQueue(void);

/* Admin option: delete every data file and start fresh */
void adminResetData(void);

#endif /* FILEIO_H */
