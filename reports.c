/* Report and statistics functions */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkease.h"
#include "reports.h"
#include "vehicle.h"
#include "slots.h"
#include "queue.h"
#include "billing.h"
#include "fileio.h"

/* Show vehicles that are currently parked. */
void showParkedVehicles(void)
{
    int i, n = 0;
    int vi;
    char label[6], whenBuf[20];

    printTitle("CURRENTLY PARKED VEHICLES");
    for (i = 0; i < TOTAL_SLOTS; i++)
        if (slots[i].occupied) n++;

    if (n == 0) {
        printf("  No vehicle is parked right now.\n");
        return;
    }

    printf("  %-6s %-18s %-13s %-14s %s\n",
           "Slot", "Type", "Reg. No", "Owner", "Entry time");
    printf("  ---------------------------------------------------------------\n");
    for (i = 0; i < TOTAL_SLOTS; i++) {
        if (!slots[i].occupied) continue;
        vi = findVehicleIndex(slots[i].regNo);
        slotLabel(slots[i].slotId, label);
        formatTime(slots[i].entryTime, whenBuf);
        printf("  %-6s %-18s %-13s %-14s %s\n",
               label, typeName(slots[i].type), slots[i].regNo,
               (vi >= 0) ? vehicles[vi].ownerName : "(unknown)", whenBuf);
    }
    printf("  ---------------------------------------------------------------\n");
    printf("  Total parked vehicles : %d\n", n);
}

/* Show all registered vehicles. */
void adminViewVehicles(void)
{
    int i;
    char status[100];
    printTitle("REGISTERED VEHICLES (ADMIN)");
    if (vehicleCount == 0) {
        printf("  No vehicle has been registered yet.\n");
        return;
    }
    printf("  %-13s %-16s %-20s %s\n",
           "Reg. No", "Owner", "Type", "Status");
    printf("  ---------------------------------------------------------------\n");
    for (i = 0; i < vehicleCount; i++) {
        vehicleStatusText(vehicles[i].regNo, status, (int)sizeof status);
        printf("  %-13s %-16s %-20s %s\n",
               vehicles[i].regNo, vehicles[i].ownerName,
               typeName(vehicles[i].type), status);
    }
    printf("  ---------------------------------------------------------------\n");
    printf("  Total registered vehicles : %d\n", vehicleCount);
}

/* Show completed transactions. */
void adminViewTransactions(void)
{
    int count = 0, i;
    double total = 0.0;
    long hours, minsPart, minutes;
    char label[6], entryBuf[20], exitBuf[20];

    printTitle("TRANSACTION HISTORY (ADMIN)");
    loadTransactions(&count);
    if (count == 0) {
        printf("  No transactions have been recorded yet.\n");
        return;
    }

    printf("  %-13s %-5s %-16s %-16s %8s\n",
           "Reg. No", "Slot", "Entry", "Exit", "Fee Rs.");
    printf("  ------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        slotLabel(transactions[i].slotId, label);
        formatTime(transactions[i].entryTime, entryBuf);
        formatTime(transactions[i].exitTime, exitBuf);
        minutes  = durationMinutes(transactions[i].entryTime, transactions[i].exitTime);
        hours    = minutes / 60;
        minsPart = minutes % 60;
        printf("  %-13s %-5s %-16s %-16s %8.2f\n",
               transactions[i].regNo, label, entryBuf, exitBuf, transactions[i].amount);
        printf("        duration : %ldh %02ldm\n", hours, minsPart);
        total += transactions[i].amount;
    }
    printf("  ------------------------------------------------------------\n");
    printf("  Transactions : %d      Total revenue : Rs. %.2f\n", count, total);
}

/* Show revenue totals. */
void adminRevenue(void)
{
    int count = 0, i, vi;
    double total = 0.0;
    double byType[4] = {0.0, 0.0, 0.0, 0.0};     /* index 0 = unknown   */
    int byCount[4]  = {0, 0, 0, 0};

    printTitle("REVENUE SUMMARY (ADMIN)");
    loadTransactions(&count);
    if (count == 0) {
        printf("  No transactions have been recorded yet.\n");
        return;
    }
    for (i = 0; i < count; i++) {
        vi = findVehicleIndex(transactions[i].regNo);
        if (vi >= 0) { byType[vehicles[vi].type] += transactions[i].amount; byCount[vehicles[vi].type]++; }
        else         { byType[0] += transactions[i].amount; byCount[0]++; }
        total += transactions[i].amount;
    }
    printf("  Total transactions : %d\n", count);
    printf("  Total revenue      : Rs. %.2f\n", total);
    printf("  -----------------------------------------\n");
    printf("  By vehicle type:\n");
    printf("    Bike          : Rs. %10.2f  (%d vehicles)\n", byType[TYPE_BIKE],  byCount[TYPE_BIKE]);
    printf("    Car           : Rs. %10.2f  (%d vehicles)\n", byType[TYPE_CAR],   byCount[TYPE_CAR]);
    printf("    Heavy Vehicle : Rs. %10.2f  (%d vehicles)\n", byType[TYPE_HEAVY], byCount[TYPE_HEAVY]);
    printf("    Other/Unknown : Rs. %10.2f  (%d vehicles)\n", byType[0], byCount[0]);
}

/* Show overall parking statistics. */
void adminVehicleStatistics(void)
{
    int i, parkedBike = 0, parkedCar = 0, parkedHeavy = 0;
    int count = 0, regBike = 0, regCar = 0, regHeavy = 0;
    double total = 0.0;

    printTitle("VEHICLE STATISTICS (ADMIN)");

    /* registered vehicles by type */
    for (i = 0; i < vehicleCount; i++) {
        if      (vehicles[i].type == TYPE_BIKE)  regBike++;
        else if (vehicles[i].type == TYPE_CAR)   regCar++;
        else if (vehicles[i].type == TYPE_HEAVY) regHeavy++;
    }

    /* currently parked vehicles by type */
    for (i = 0; i < TOTAL_SLOTS; i++) {
        if (!slots[i].occupied) continue;
        if      (slots[i].type == TYPE_BIKE)  parkedBike++;
        else if (slots[i].type == TYPE_CAR)   parkedCar++;
        else if (slots[i].type == TYPE_HEAVY) parkedHeavy++;
    }

    /* lifetime figures from the transaction file */
    loadTransactions(&count);
    for (i = 0; i < count; i++) total += transactions[i].amount;

    printf("  Registered vehicles : %3d  [Bike %d | Car %d | Heavy %d]\n",
           vehicleCount, regBike, regCar, regHeavy);
    printf("  Currently parked    : %3d  [Bike %d | Car %d | Heavy %d]\n",
           parkedBike + parkedCar + parkedHeavy,
           parkedBike, parkedCar, parkedHeavy);
    printf("  Free slots          : %3d  [Bike %d/%d | Car %d/%d | Heavy %d/%d]\n",
           freeSlotsOfType(TYPE_BIKE) + freeSlotsOfType(TYPE_CAR) +
               freeSlotsOfType(TYPE_HEAVY),
           freeSlotsOfType(TYPE_BIKE),  TOTAL_BIKE_SLOTS,
           freeSlotsOfType(TYPE_CAR),   TOTAL_CAR_SLOTS,
           freeSlotsOfType(TYPE_HEAVY), TOTAL_HEAVY_SLOTS);
    printf("  Waiting in queue    : %3d\n", queueLength());
    printf("  -------------------------------------------\n");
    printf("  Transactions recorded : %d\n", count);
    printf("  Lifetime revenue      : Rs. %.2f\n", total);
}
