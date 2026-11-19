/* Report function declarations */

#ifndef REPORTS_H
#define REPORTS_H

/* Menu option: parked vehicles */
void showParkedVehicles(void);

/* Admin option: list all registered vehicles with live status */
void adminViewVehicles(void);

/* Admin option: full transaction history */
void adminViewTransactions(void);

/* Admin option: revenue summary by vehicle type */
void adminRevenue(void);

/* Admin option: vehicle statistics (registered / parked / queue) */
void adminVehicleStatistics(void);

#endif /* REPORTS_H */
