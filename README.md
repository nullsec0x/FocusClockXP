#  Focus Clock XP
**Creator:** Nullsec0x  

A lightweight Windows XP x64 application that helps you stay productive.  
Create a task, set a timer, hit start — when time’s up, an alarm sound plays.  
If you try to quit while a task is running, you’ll get a confirmation prompt:
> “[task name] is running — are you sure you want to quit?”

##  Features
- Task creation with custom names
- Countdown timer
- Alarm notification when the timer ends
- Quit confirmation if a timer is active
- Designed for **Windows XP x64**

## 📂 Project Structure
```
main.c         # Core application logic
resource.h     # Resource identifiers
resource.rc    # Icons, menus, and dialogs
```

## 🛠 Compilation

### **On Windows XP (MinGW)**
1. Install [MinGW](http://www.mingw.org/)
2. Open `cmd` in the project directory
3. Run:
   ```bash
   gcc main.c resource.rc -o focusclock.exe -mwindows
   ```

### **On Linux (Cross-compile for XP)**
1. Install MinGW-w64:
   ```bash
   sudo apt install mingw-w64
   ```
2. Run:
   ```bash
   x86_64-w64-mingw32-gcc main.c resource.rc -o focusclock.exe -mwindows
   ```

##  Usage
- Run `FocusClockXP.exe` on Windows XP x64  
- Create a task, set your time, start  
- When finished, alarm will play  

## 📦 Releases
Compiled binaries are available in the [Releases](../../releases) section.

## 📜 License
MIT License — do whatever you want, but credit **Nullsec0x**.

##  Screenshots
*(Add screenshots here if you want)*
