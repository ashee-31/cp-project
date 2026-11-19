# ParkEase

ParkEase is a menu-driven parking lot management program written in C. It
supports vehicle registration, parking entry and exit, fee calculation,
vehicle-specific parking slots, a waiting queue, and administrator reports.
The program runs in a terminal and saves its records in text data files in
the current working directory.

## Features

- Separate operator and administrator menus.
- Registration and lookup of bikes, cars, and heavy vehicles.
- Automatic assignment to the next free slot for the vehicle's type.
- Waiting queue when no matching slot is available; queued vehicles are
  automatically assigned a matching slot when one is freed.
- Parking receipts, transaction history, revenue summaries, and vehicle
  statistics.
- Persistent vehicle, slot, transaction, and queue records in text files.
- Administrator tools for viewing and managing parking, including a full data
  reset.

## Parking capacity and fees

The default lot has 40 slots:

| Vehicle type | Slots | First hour | Each additional hour |
| --- | ---: | ---: | ---: |
| Bike | 20 | Rs. 20 | Rs. 10 |
| Car | 15 | Rs. 40 | Rs. 20 |
| Heavy vehicle | 5 | Rs. 60 | Rs. 30 |

Stays of up to 15 minutes are free. After that, any partial hour is billed as
a full hour. These settings and the administrator password are defined in
`parkease.h`.

## Project files

| File(s) | Responsibility |
| --- | --- |
| `main.c` | Program entry point, startup data loading, and role-based menus. |
| `parkease.h`, `globals.c` | Shared configuration, data structures, utility declarations, and global state. |
| `vehicle.c`, `vehicle.h` | Vehicle registration, lookup, and current status. |
| `slots.c`, `slots.h` | Slot initialization, allocation helpers, availability display, and manual release. |
| `entryexit.c`, `entryexit.h` | Vehicle entry and exit workflows. |
| `billing.c`, `billing.h` | Parking duration and fee calculation, receipts, and fee display. |
| `queue.c`, `queue.h` | Array-based waiting queue and automatic allocation when a matching slot opens. |
| `reports.c`, `reports.h` | Parked vehicle lists, transaction history, revenue, and statistics. |
| `fileio.c`, `fileio.h` | Loading and saving persistent records. |
| `util.c` | Input validation, string helpers, and display formatting. |

## Requirements

- A C compiler such as GCC (MinGW-w64 GCC on Windows).
- A terminal or command prompt.

No external libraries or package installation are required; the program uses
the C standard library.

## Build and run

Run these commands from the project directory so the generated data files stay
with the program.

### Windows (PowerShell with GCC installed)

```powershell
gcc -Wall -Wextra -O2 -o parkease.exe main.c globals.c util.c vehicle.c slots.c entryexit.c billing.c queue.c reports.c fileio.c
.\parkease.exe
```

### Linux or macOS

```sh
gcc -Wall -Wextra -O2 -o parkease main.c globals.c util.c vehicle.c slots.c entryexit.c billing.c queue.c reports.c fileio.c
./parkease
```

On startup, choose **Operator** for day-to-day parking operations or
**Administrator** for management and reports. The default administrator
password is `admin123`; change `ADMIN_PASSWORD` in `parkease.h` before building
if needed. The program allows three login attempts per administrator login.

## Data files

The application creates and updates these files in the directory from which
it is run:

- `vehicles.dat` — registered vehicle records.
- `slots.dat` — current slot assignments and entry times.
- `transactions.dat` — completed exits and fees.
- `queue.dat` — vehicles currently waiting for a slot.

Keep these files alongside the executable between runs to preserve application
data. The administrator's **Reset ALL data** action deletes and recreates the
records; this cannot be undone.
