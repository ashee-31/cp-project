#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkease.h"
#include "fileio.h"
#include "queue.h"
#include "slots.h"

void ensureFile(const char *name)
{
    FILE *fp = fopen(name, "r");
    if (fp != NULL) { fclose(fp); return; }
    fp = fopen(name, "w");
    if (fp != NULL) fclose(fp);
}

void saveVehicles(void)
{
    FILE *fp = fopen(DATA_VEHICLES, "w");
    int i;
    if (fp == NULL) { printf("  [!] Cannot write %s\n", DATA_VEHICLES); return; }
    for (i = 0; i < vehicleCount; i++)
        fprintf(fp, "%s|%s|%d\n", vehicles[i].regNo, vehicles[i].ownerName, vehicles[i].type);
    fclose(fp);
}

void loadVehicles(void)
{
    FILE *fp = fopen(DATA_VEHICLES, "r");
    char line[128];
    if (fp == NULL) return;
    vehicleCount = 0;
    while (vehicleCount < MAX_VEHICLES && fgets(line, sizeof line, fp) != NULL) {
        if (sscanf(line, "%15[^|]|%31[^|]|%d",
                   vehicles[vehicleCount].regNo,
                   vehicles[vehicleCount].ownerName,
                   &vehicles[vehicleCount].type) == 3)
            vehicleCount++;
    }
    fclose(fp);
}

void saveSlots(void)
{
    FILE *fp = fopen(DATA_SLOTS, "w");
    int i;
    if (fp == NULL) { printf("  [!] Cannot write %s\n", DATA_SLOTS); return; }
    for (i = 0; i < TOTAL_SLOTS; i++)
        fprintf(fp, "%d %d %d %s %ld\n",
                slots[i].slotId, slots[i].type, slots[i].occupied,
                slots[i].regNo[0] ? slots[i].regNo : "-",
                (long)slots[i].entryTime);
    fclose(fp);
}

int loadSlots(void)
{
    FILE *fp = fopen(DATA_SLOTS, "r");
    char regNo[16];
    long entryTime;
    int i;
    if (fp == NULL) return 0;
    for (i = 0; i < TOTAL_SLOTS; i++) {
        if (fscanf(fp, "%d %d %d %15s %ld",
                   &slots[i].slotId, &slots[i].type,
                   &slots[i].occupied, regNo, &entryTime) != 5) {
            fclose(fp);
            return 0;
        }
        if (strcmp(regNo, "-") == 0) slots[i].regNo[0] = '\0';
        else strcpy(slots[i].regNo, regNo);
        slots[i].entryTime = (time_t)entryTime;
    }
    fclose(fp);
    return 1;
}

void appendTransaction(const char *regNo, int slotId,
                       time_t entryTime, time_t exitTime, double amount)
{
    FILE *fp = fopen(DATA_TXN, "a");
    if (fp == NULL) { printf("  [!] Cannot write %s\n", DATA_TXN); return; }
    fprintf(fp, "%s %d %ld %ld %.2f\n",
            regNo, slotId, (long)entryTime,
            (long)exitTime, amount);
    fclose(fp);
}

void loadTransactions(int *countOut)
{
    FILE *fp = fopen(DATA_TXN, "r");
    int count = 0;
    long entryTime, exitTime;
    *countOut = 0;
    if (fp == NULL) return;
    while (count < MAX_TRANSACTIONS &&
           fscanf(fp, "%15s %d %ld %ld %lf",
                  transactions[count].regNo,
                  &transactions[count].slotId,
                  &entryTime, &exitTime,
                  &transactions[count].amount) == 5) {
        transactions[count].entryTime = (time_t)entryTime;
        transactions[count].exitTime = (time_t)exitTime;
        count++;
    }
    fclose(fp);
    *countOut = count;
}

void saveQueue(void)
{
    FILE *fp = fopen(DATA_QUEUE, "w");
    int i;
    if (fp == NULL) { printf("  [!] Cannot write %s\n", DATA_QUEUE); return; }
    for (i = 0; i < queueCount; i++)
        fprintf(fp, "%s %ld\n", queueRegNo[i], (long)queueWaitTime[i]);
    fclose(fp);
}

void loadQueue(void)
{
    FILE *fp = fopen(DATA_QUEUE, "r");
    long waitTime;
    queueCount = 0;
    if (fp == NULL) return;
    while (queueCount < MAX_QUEUE &&
           fscanf(fp, "%15s %ld", queueRegNo[queueCount], &waitTime) == 2) {
        queueWaitTime[queueCount] = (time_t)waitTime;
        queueCount++;
    }
    fclose(fp);
}

void adminResetData(void)
{
    char answer[32];
    printTitle("RESET ALL DATA (ADMIN)");
    printf("  [!] This permanently deletes ALL registered vehicles,\n");
    printf("      all transactions and the waiting queue.\n");
    readString("  Type RESET to confirm (anything else cancels) : ",
               answer, (int)sizeof answer);
    if (strcmp(answer, "RESET") != 0) {
        printf("  [i] Reset cancelled. All data is safe.\n");
        return;
    }
    remove(DATA_VEHICLES);
    remove(DATA_SLOTS);
    remove(DATA_TXN);
    remove(DATA_QUEUE);
    vehicleCount = 0;
    freeQueue();
    initializeSlots();
    saveVehicles();
    saveSlots();
    saveQueue();
    printf("  [OK] All data has been cleared. The lot is empty and fresh.\n");
}
