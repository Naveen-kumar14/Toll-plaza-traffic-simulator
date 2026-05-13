# 🚦 Toll Plaza Traffic Simulator v2.0

A **C++ console-based** Toll Plaza simulator built with core **Data Structures** — Queue, Stack, and Vector.

---

## ✨ Features (v2.0 Enhancements)

| Feature | Description |
|---|---|
| **Duplicate plate check** | Prevents same vehicle entering twice |
| **Owner name field** | Optional owner/driver name per vehicle |
| **Visual capacity bar** | `█░░░` fill bar showing lane usage |
| **Vehicle type icon** | `[CAR]` `[BUS]` `[TRUCK]` `[BIKE]` labels |
| **Emergency priority** | Clear all emergency vehicles first |
| **Filter history** | Filter processed records by type or lane |
| **Toll rate card** | View all rates in a formatted table |
| **Export to CSV** | Save full history as `.csv` file |
| **Lane efficiency %** | Stats show processed/entered ratio per lane |
| **Vehicle mix stats** | Breakdown of car/bus/truck/bike counts |
| **Average toll** | Auto-computed from total collected |
| **7 demo vehicles** | Richer seed data with owner names |
| **13-option menu** | Expanded from 9 options |

---

## 🔢 Data Structures Used

| DSA | Where Used |
|---|---|
| **Circular Queue** | Each toll lane (FASTag / Cash / Emergency) |
| **Array-based Stack** | Undo history (up to 50 operations) |
| **Dynamic Vector** | Processed vehicle history |
| **Linear Search** | Vehicle lookup across lanes and history |

---

## 💰 Toll Rates

| Vehicle | FASTag | Cash | Emergency |
|---|---|---|---|
| Car | Rs.65 | Rs.90 | FREE |
| Bus | Rs.130 | Rs.180 | FREE |
| Truck | Rs.160 | Rs.215 | FREE |
| Bike | Rs.35 | Rs.50 | FREE |

> FASTag saves up to 26% vs cash!

---

## 📁 File Structure

```
toll_plaza/
├── main.cpp         # Entry point, seeds demo data
├── simulator.h/.cpp # Main controller — all menu actions
├── vehicle.h/.cpp   # Vehicle & ProcessedRecord structs
├── queue.h/.cpp     # Circular array-based Queue
├── stack.h/.cpp     # Array-based Stack (undo)
├── utils.h/.cpp     # ANSI colors, helpers, plate generator
├── Makefile         # One-command build
└── README.md
```

---

## 🚀 Build & Run

### Linux / macOS
```bash
make
./toll_plaza
```

### Windows (MinGW)
```bash
mingw32-make
toll_plaza.exe
```

### Clean build
```bash
make clean
```

---

## 📋 Menu Options

```
[1]  Add vehicle               [8]  Filter history by type/lane
[2]  Process from a lane       [9]  Search vehicle by plate
[3]  Process ALL lanes         [10] View toll rate card
[4]  Emergency priority        [11] Undo last action
[5]  View all lanes            [12] Export history to CSV
[6]  View statistics           [13] Clear all lanes
[7]  View processed history    [0]  Exit
```

---

## Contributors

1. Naveen Kumar.c ([Naveen-kumar14](https://github.com/Naveen-kumar14))
2. Akash.r ([Akash-r-git](https://github.com/Akash-r-git))
