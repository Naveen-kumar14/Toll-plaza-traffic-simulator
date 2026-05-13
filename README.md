# 🚦 Toll Plaza Traffic Simulator v2.0

> A feature-rich **C++ console-based Traffic Simulation System** built using core **Data Structures & Algorithms** concepts including **Circular Queue, Stack, Vector, and Searching techniques** to simulate real-world toll plaza operations with smart traffic management and analytics.

---

## 🌟 Project Highlights

✨ Realistic toll plaza simulation
⚡ FASTag vs Cash lane management
🚑 Emergency vehicle prioritization
📊 Advanced traffic analytics & statistics
🧠 DSA-focused architecture for academic learning
📁 CSV export support for processed history
🎨 Clean colored terminal UI with capacity indicators

---

# 📸 System Preview

```text
╔════════════════════════════════════╗
║     TOLL PLAZA TRAFFIC SYSTEM      ║
╚════════════════════════════════════╝

[FASTag Lane]  ███████░░░  7/10
[Cash Lane]    ████░░░░░░  4/10
[Emergency]    ██░░░░░░░░  2/10
```

---

# ✨ Features (v2.0)

| 🚀 Feature                 | 📖 Description                                |
| -------------------------- | --------------------------------------------- |
| Duplicate Plate Detection  | Prevents the same vehicle from entering twice |
| Owner/Driver Name          | Stores optional owner information             |
| Smart Capacity Bar         | Visual lane fill indicators using blocks      |
| Vehicle Type Tags          | `[CAR] [BUS] [TRUCK] [BIKE]`                  |
| Emergency Priority Engine  | Clears emergency vehicles first               |
| Process ALL Lanes          | Simultaneous traffic processing               |
| History Filtering          | Filter by vehicle type or lane                |
| Search by Plate            | Linear search across active/history data      |
| Toll Rate Card             | Formatted pricing table                       |
| Undo System                | Revert last operation using Stack             |
| CSV Export                 | Save processed history externally             |
| Lane Efficiency Analytics  | Shows processing efficiency percentage        |
| Vehicle Distribution Stats | Vehicle-type breakdown analytics              |
| Average Toll Calculator    | Dynamic toll collection insights              |
| Rich Demo Data             | 7 seeded vehicles for testing                 |
| 13-Option Interactive Menu | Expanded system controls                      |

---

# 🧠 Data Structures & Algorithms Used

| 📚 Concept              | 🔧 Implementation              |
| ----------------------- | ------------------------------ |
| Circular Queue          | Toll lane vehicle management   |
| Array-Based Stack       | Undo operation history         |
| Dynamic Vector          | Processed vehicle records      |
| Linear Search           | Plate lookup & filtering       |
| Conditional Processing  | Emergency prioritization       |
| Statistical Computation | Revenue & efficiency analytics |

---

# 🛣️ Toll Lane Types

| Lane              | Purpose                     |
| ----------------- | --------------------------- |
| ⚡ FASTag Lane     | Faster automated processing |
| 💵 Cash Lane      | Manual payment processing   |
| 🚑 Emergency Lane | Priority clearance system   |

---

# 💰 Toll Rate Card

| Vehicle Type | FASTag | Cash   | Emergency |
| ------------ | ------ | ------ | --------- |
| 🚗 Car       | Rs.65  | Rs.90  | FREE      |
| 🚌 Bus       | Rs.130 | Rs.180 | FREE      |
| 🚛 Truck     | Rs.160 | Rs.215 | FREE      |
| 🏍️ Bike     | Rs.35  | Rs.50  | FREE      |

> 💡 FASTag users save up to **26%** compared to cash payments.

---

# 📂 Project Structure

```bash
toll_plaza/
│
├── main.cpp
├── simulator.h
├── simulator.cpp
├── vehicle.h
├── vehicle.cpp
├── queue.h
├── queue.cpp
├── stack.h
├── stack.cpp
├── utils.h
├── utils.cpp
├── Makefile
└── README.md
```

---

# ⚙️ Core Modules

| Module          | Responsibility                     |
| --------------- | ---------------------------------- |
| `main.cpp`      | Application entry point            |
| `simulator.cpp` | Menu system & traffic controller   |
| `vehicle.cpp`   | Vehicle models & processed records |
| `queue.cpp`     | Circular queue implementation      |
| `stack.cpp`     | Undo stack implementation          |
| `utils.cpp`     | ANSI colors, helpers & utilities   |

---

# 🚀 Build & Run

## 🐧 Linux / macOS

```bash
make
./toll_plaza
```

---

## 🪟 Windows (MinGW)

```bash
mingw32-make
toll_plaza.exe
```

---

## 🧹 Clean Build Files

```bash
make clean
```

---

# 📋 Interactive Menu

```text
[1]  Add vehicle
[2]  Process from a lane
[3]  Process ALL lanes
[4]  Emergency priority mode
[5]  View all lanes
[6]  View statistics
[7]  View processed history
[8]  Filter history by type/lane
[9]  Search vehicle by plate
[10] View toll rate card
[11] Undo last action
[12] Export history to CSV
[13] Clear all lanes
[0]  Exit
```

---

# 📊 Analytics Generated

✅ Total vehicles processed
✅ Total toll revenue collected
✅ Lane efficiency percentage
✅ Average toll amount
✅ Vehicle category distribution
✅ FASTag vs Cash comparison

---

# 🎯 Learning Outcomes

This project demonstrates practical implementation of:

* Queue Operations
* Stack Operations
* Dynamic Memory Concepts
* File Handling (`CSV Export`)
* Object-Oriented Programming
* Modular Programming
* Traffic Flow Simulation
* Real-time Console UI Design

---

# 🏗️ Future Improvements

* GUI Version using Qt / SFML
* Database Integration
* AI-based traffic prediction
* Multi-threaded lane processing
* Online FASTag recharge simulation
* Real-time dashboard analytics

---

# 👨‍💻 Contributors

| Name           | GitHub                                                                     |
| -------------- | -------------------------------------------------------------------------- |
| Naveen Kumar C | 
| Shiv Samarpan  |                                                               |

---

# ⭐ Repository Goals

This project was built to combine:

📚 Academic DSA learning
⚙️ Real-world traffic simulation
🧠 Problem-solving skills
🚀 Clean modular C++ development

---


This project is developed for **educational and learning purposes**.
