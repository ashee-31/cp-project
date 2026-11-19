/* Parking slot setup and slot management */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parkease.h"
#include "slots.h"
#include "vehicle.h"
#include "queue.h"
#include "fileio.h"

/* Create bike, car, and heavy-vehicle slots. */
void initializeSlots(void)
{
    int id = 1, i;
    for (i = 0; i < TOTAL_BIKE_SLOTS;  i++, id++) {
        slots[id - 1].slotId = id;  slots[id - 1].type = TYPE_BIKE;
        slots[id - 1].occupied = 0; slots[id - 1].regNo[0] = '\0';
        slots[id - 1].entryTime = 0;
    }
    for (i = 0; i < TOTAL_CAR_SLOTS;   i++, id++) {
        slots[id - 1].slotId = id;  slots[id - 1].type = TYPE_CAR;
        slots[id - 1].occupied = 0; slots[id - 1].regNo[0] = '\0';
        slots[id - 1].entryTime = 0;
    }
    for (i = 0; i < TOTAL_HEAVY_SLOTS; i++, id++) {
        slots[id - 1].slotId = id;  slots[id - 1].type = TYPE_HEAVY;
        slots[id - 1].occupied = 0; slots[id - 1].regNo[0] = '\0';
        slots[id - 1].entryTime = 0;
    }
}

/* Count free slots of one vehicle type */
int freeSlotsOfType(int type)
{
    int i, freeCount = 0;
    for (i = 0; i < TOTAL_SLOTS; i++)
        if (slots[i].type == type && !slots[i].occupied) freeCount++;
    return freeCount;
}

/* Index of the first free slot for a vehicle type, or -1 */
int findFreeSlotIndex(int type)
{
    int i;
    for (i = 0; i < TOTAL_SLOTS; i++)
        if (slots[i].type == type && !slots[i].occupied) return i;
    return -1;
}

/* Index of the occupied slot holding regNo, or -1 */
int findSlotByReg(const char *regNo)
{
    int i;
    for (i = 0; i < TOTAL_SLOTS; i++)
        if (slots[i].occupied && strcmp(slots[i].regNo, regNo) == 0) return i;
    return -1;
}

/* Print one type of slot in a five-column grid. */
static void printTypeGrid(int type, const char *heading,
                          int count, int startIndex)
{
    int i, row, col;
    char label[6];
    printf("\n  %s : %d free / %d total\n", heading,
           freeSlotsOfType(type), count);
    for (row = 0; row < (count + 4) / 5; row++) {
        printf("  ");
        for (col = 0; col < 5; col++) {
            i = startIndex + row * 5 + col;
            if (i >= startIndex + count) break;
            slotLabel(slots[i].slotId, label);
            printf("[%s %c]  ", label, slots[i].occupied ? 'X' : ' ');
        }
        printf("\n");
    }
}

/* Show the parking lot status. */
void showAvailabilityGrid(void)
{
    int freeBike  = freeSlotsOfType(TYPE_BIKE);
    int freeCar   = freeSlotsOfType(TYPE_CAR);
    int freeHeavy = freeSlotsOfType(TYPE_HEAVY);

    printTitle("PARKING LOT STATUS");
    printTypeGrid(TYPE_BIKE,  "BIKE SLOTS (B01-B20)",
                  TOTAL_BIKE_SLOTS, 0);
    printTypeGrid(TYPE_CAR,   "CAR SLOTS (C01-C15)",
                  TOTAL_CAR_SLOTS, TOTAL_BIKE_SLOTS);
    printTypeGrid(TYPE_HEAVY, "HEAVY VEHICLE SLOTS (H01-H05)",
                  TOTAL_HEAVY_SLOTS, TOTAL_BIKE_SLOTS + TOTAL_CAR_SLOTS);

    printf("\n  Legend : X = occupied, blank = free\n");
    printf("  -----------------------------------------\n");
    printf("  Free : Bike %2d/%2d | Car %2d/%2d | Heavy %2d/%2d\n",
           freeBike, TOTAL_BIKE_SLOTS, freeCar, TOTAL_CAR_SLOTS,
           freeHeavy, TOTAL_HEAVY_SLOTS);
    printf("  TOTAL       : %2d free / %2d total\n",
           freeBike + freeCar + freeHeavy, TOTAL_SLOTS);
}

/* Show details for every slot. */
void slotDetailsTable(void)
{
    int i;
    char label[6];
    printTitle("PARKING SLOT DETAILS (ADMIN)");
    printf("  %-6s %-18s %s\n", "Slot", "Type", "Status");
    printf("  -------------------------------------------------\n");
    for (i = 0; i < TOTAL_SLOTS; i++) {
        slotLabel(slots[i].slotId, label);
        printf("  %-6s %-18s %s\n", label, typeName(slots[i].type),
               slots[i].occupied ? slots[i].regNo : "FREE");
    }
    printf("  -------------------------------------------------\n");
    printf("  Bike  slots : %2d free / %2d total\n",
           freeSlotsOfType(TYPE_BIKE),  TOTAL_BIKE_SLOTS);
    printf("  Car   slots : %2d free / %2d total\n",
           freeSlotsOfType(TYPE_CAR),   TOTAL_CAR_SLOTS);
    printf("  Heavy slots : %2d free / %2d total\n",
           freeSlotsOfType(TYPE_HEAVY), TOTAL_HEAVY_SLOTS);
}

/* Convert a slot label such as B07 or C03 to a slot ID. */
static int parseSlotLabel(const char *label)
{
    int n;
    if (strlen(label) != 3) return -1;
    if (label[1] < '0' || label[1] > '9' ||
        label[2] < '0' || label[2] > '9') return -1;
    n = (label[1] - '0') * 10 + (label[2] - '0');
    if (n == 0) return -1;
    switch (label[0]) {
        case 'B': if (n <= TOTAL_BIKE_SLOTS)  return n;
                  break;
        case 'C': if (n <= TOTAL_CAR_SLOTS)
                      return TOTAL_BIKE_SLOTS + n;
                  break;
        case 'H': if (n <= TOTAL_HEAVY_SLOTS)
                      return TOTAL_BIKE_SLOTS + TOTAL_CAR_SLOTS + n;
                  break;
        default:  break;
    }
    return -1;
}

/* Manually release an occupied slot without billing. */
void adminReleaseSlot(void)
{
    char label[8], answer[8];
    int slotId, idx = -1, i, vi;
    printTitle("MANUALLY RELEASE A SLOT (ADMIN)");
    readString("Enter slot label (e.g. B07, C03, H02) : ",
               label, (int)sizeof label);
    toUpperStr(label);
    slotId = parseSlotLabel(label);
    if (slotId < 0) {
        printf("  [!] Invalid label. Use the slot grid, e.g. B07, C03, H02.\n");
        return;
    }
    for (i = 0; i < TOTAL_SLOTS; i++)
        if (slots[i].slotId == slotId) { idx = i; break; }
    if (idx < 0) { printf("  [!] Slot %s does not exist.\n", label); return; }
    if (!slots[idx].occupied) {
        printf("  [i] Slot %s is already free.\n", label);
        return;
    }
    vi = findVehicleIndex(slots[idx].regNo);
    printf("  Slot %s holds vehicle %s (owner: %s).\n", label,
           slots[idx].regNo,
           (vi >= 0) ? vehicles[vi].ownerName : "(unknown)");
    readString("  Release this slot WITHOUT billing? (yes/no) : ",
               answer, (int)sizeof answer);
    if (!isYes(answer)) {
        printf("  [i] Cancelled. Nothing was changed.\n");
        return;
    }
    printf("  [OK] Slot %s has been released (vehicle %s removed).\n",
           label, slots[idx].regNo);
    slots[idx].occupied  = 0;
    slots[idx].regNo[0]  = '\0';
    slots[idx].entryTime = 0;
    saveSlots();
    assignWaitingVehicle(idx);   /* give the slot to a waiting vehicle */
}
