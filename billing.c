/* Parking duration and fee calculation */

#include <stdio.h>
#include "parkease.h"
#include "billing.h"

/* Find the number of whole minutes, rounding up. */
long durationMinutes(time_t entryTime, time_t exitTime)
{
    long seconds = (long)difftime(exitTime, entryTime);
    long minutes = seconds / 60;
    if (seconds % 60 != 0) minutes++;
    if (minutes < 0) minutes = 0;
    return minutes;
}

/* Calculate the parking fee. */
double calculateFee(int type, time_t entryTime, time_t exitTime)
{
    long minutes, hours;
    double firstRate, hourlyRate;

    switch (type) {
        case TYPE_BIKE:  firstRate = BIKE_FIRST_RATE;  hourlyRate = BIKE_HOURLY_RATE;  break;
        case TYPE_CAR:   firstRate = CAR_FIRST_RATE;   hourlyRate = CAR_HOURLY_RATE;   break;
        case TYPE_HEAVY: firstRate = HEAVY_FIRST_RATE; hourlyRate = HEAVY_HOURLY_RATE; break;
        default:         firstRate = CAR_FIRST_RATE;   hourlyRate = CAR_HOURLY_RATE;   break;
    }
    minutes = durationMinutes(entryTime, exitTime);
    if (minutes <= GRACE_MINUTES) return 0.0;
    hours = (minutes + 59) / 60;              /* ceiling to full hours */
    if (hours < 1) hours = 1;
    if (hours == 1) return firstRate;
    return firstRate + (double)(hours - 1) * hourlyRate;
}

/* Print a parking receipt. */
void printReceipt(const char *regNo, const char *owner, int type,
                  int slotId, time_t entryTime, time_t exitTime, double fee)
{
    char label[6], entryBuf[20], exitBuf[20];
    long minutes    = durationMinutes(entryTime, exitTime);
    long hours      = minutes / 60;
    long minsPart   = minutes % 60;

    slotLabel(slotId, label);
    formatTime(entryTime, entryBuf);
    formatTime(exitTime, exitBuf);

    printf("\n  ---------------- PARKING RECEIPT ----------------\n");
    printf("   Reg. No     : %s\n", regNo);
    printf("   Owner       : %s\n", owner);
    printf("   Vehicle     : %s\n", typeName(type));
    printf("   Slot        : %s\n", label);
    printf("   Entry time  : %s\n", entryBuf);
    printf("   Exit time   : %s\n", exitBuf);
    printf("   Duration    : %ldh %02ldm (%ld minutes)\n",
           hours, minsPart, minutes);
    if (fee <= 0.0)
        printf("   Parking fee : Rs. 0.00  (within %d-minute grace period)\n",
               GRACE_MINUTES);
    else
        printf("   Parking fee : Rs. %.2f\n", fee);
    printf("  --------------------------------------------------\n");
}

/* Show the parking fee chart. */
void showFeeRates(void)
{
    printTitle("PARKING FEE RATES");
    printf("  -------------------------------------------------\n");
    printf("   Vehicle type    First hour   Every extra hour\n");
    printf("  -------------------------------------------------\n");
    printf("   Bike            Rs. %5.2f    Rs. %5.2f\n",
           BIKE_FIRST_RATE, BIKE_HOURLY_RATE);
    printf("   Car             Rs. %5.2f    Rs. %5.2f\n",
           CAR_FIRST_RATE, CAR_HOURLY_RATE);
    printf("   Heavy Vehicle   Rs. %5.2f    Rs. %5.2f\n",
           HEAVY_FIRST_RATE, HEAVY_HOURLY_RATE);
    printf("  -------------------------------------------------\n");
    printf("  * Any part of an hour is billed as a full hour.\n");
    printf("  * Stays up to %d minutes are FREE (grace period).\n",
           GRACE_MINUTES);
}
