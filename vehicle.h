/* Vehicle management function declarations */

#ifndef VEHICLE_H
#define VEHICLE_H

/* Linear search: index of a registered vehicle in vehicles[], or -1 */
int  findVehicleIndex(const char *regNo);

/* Append a vehicle record to the array and save it immediately.
   Returns 1 on success, 0 if the database is full.                  */
int  addVehicle(const char *regNo, const char *ownerName, int type);

/* Interactive menu option: register a new vehicle */
void registerVehicle(void);

/* Interactive menu option: search a vehicle by registration number */
void searchVehicle(void);

/* Text describing where a vehicle currently is
   ("PARKED at slot C03 since ...", "WAITING in queue ...", or
   "Not parked right now")                                           */
void vehicleStatusText(const char *regNo, char *out, int size);

#endif /* VEHICLE_H */
