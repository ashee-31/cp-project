/* Parking slot function declarations */

#ifndef SLOTS_H
#define SLOTS_H

/* Create the whole lot: bike slots first, then car, then heavy */
void initializeSlots(void);

/* Count free slots of one vehicle type */
int  freeSlotsOfType(int type);

/* Index of the first free slot for a vehicle type, or -1 */
int  findFreeSlotIndex(int type);

/* Index of the occupied slot holding regNo, or -1 */
int  findSlotByReg(const char *regNo);

/* Menu option: PARKING LOT STATUS - grid of [X] / [ ] cells per type */
void showAvailabilityGrid(void);

/* Menu option: detailed table of every slot (who is parked where)  */
void slotDetailsTable(void);

/* Admin option: free a stuck slot without billing (e.g. "C05")     */
void adminReleaseSlot(void);

#endif /* SLOTS_H */
