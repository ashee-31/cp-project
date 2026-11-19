#include <stdio.h>
#include <string.h>
#include "parkease.h"
#include "queue.h"
#include "vehicle.h"
#include "fileio.h"

void enqueueVehicle(const char *regNo, time_t waitTime)
{
    if (queueCount >= MAX_QUEUE) {
        printf("  [!] The waiting queue is full.\n");
        return;
    }
    strcpy(queueRegNo[queueCount], regNo);
    queueWaitTime[queueCount] = waitTime;
    queueCount++;
}

int queuePosition(const char *regNo)
{
    int i;
    for (i = 0; i < queueCount; i++)
        if (strcmp(queueRegNo[i], regNo) == 0) return i + 1;
    return 0;
}

int queueLength(void)
{
    return queueCount;
}

void removeFromQueue(const char *regNo)
{
    int i, j;
    for (i = 0; i < queueCount; i++) {
        if (strcmp(queueRegNo[i], regNo) == 0) {
            for (j = i; j < queueCount - 1; j++) {
                strcpy(queueRegNo[j], queueRegNo[j + 1]);
                queueWaitTime[j] = queueWaitTime[j + 1];
            }
            queueCount--;
            return;
        }
    }
}

void freeQueue(void)
{
    queueCount = 0;
}

void displayQueue(void)
{
    int i, vi;
    char sinceBuf[20];
    printTitle("WAITING QUEUE");
    if (queueCount == 0) {
        printf("  The waiting queue is empty. All is well!\n");
        return;
    }
    printf("  %-4s %-14s %-20s %-16s\n", "Pos", "Reg. No", "Type", "Waiting since");
    printf("  -------------------------------------------------\n");
    for (i = 0; i < queueCount; i++) {
        vi = findVehicleIndex(queueRegNo[i]);
        formatTime(queueWaitTime[i], sinceBuf);
        printf("  %-4d %-14s %-20s %-16s\n",
               i + 1, queueRegNo[i],
               (vi >= 0) ? typeName(vehicles[vi].type) : "Unknown",
               sinceBuf);
    }
    printf("  -------------------------------------------------\n");
    printf("  Total vehicles waiting : %d\n", queueCount);
}

int assignWaitingVehicle(int slotIndex)
{
    int i, vi;
    char label[6], nowBuf[20];
    int slotType = slots[slotIndex].type;

    for (i = 0; i < queueCount; i++) {
        vi = findVehicleIndex(queueRegNo[i]);
        if (vi >= 0 && vehicles[vi].type == slotType) {
            slots[slotIndex].occupied = 1;
            strcpy(slots[slotIndex].regNo, queueRegNo[i]);
            slots[slotIndex].entryTime = time(NULL);
            saveSlots();
            slotLabel(slots[slotIndex].slotId, label);
            formatTime(slots[slotIndex].entryTime, nowBuf);
            printf("  [OK] Slot %s auto-allocated to waiting vehicle %s (%s).\n",
                   label, queueRegNo[i], typeName(slotType));
            printf("       Entry time : %s\n", nowBuf);
            removeFromQueue(queueRegNo[i]);
            saveQueue();
            return 1;
        }
    }
    return 0;
}
