# Roguelike.cpp
My new, larger pet project. The game in a style rouge-like and star powers!

## 📝 About The Project

This is my learning project that I created while studying C++. 
It's a simple game where you control a character on a 20x20 grid, 
fight enemies, and unlock special abilities.

**Current features:**
- Turn-based movement and combat
- 9 enemies with random stats
- Save/Load system
- 4 unlockable "Star Powers":
  - Soul Strength (heal on kill)
  - Furious Hero (increasing damage)
  - Last Chance (survive lethal damage)
  - Living Steel (restore armor)

## 🎮 How To Play

| Key | Action |
|-----|--------|
| L | Move left / Attack enemy on left |
| R | Move right / Attack enemy on right |
| U | Move up / Attack enemy above |
| D | Move down / Attack enemy below |
| C | Check your unlocked Star Powers |
| S | Save game |

## 🛠️ Built With

* C++ (C++17)
* Standard Library only (no external dependencies)

## 🚀 How To Compile And Run

### Linux / MacOS
```bash
g++ -std=c++17 main.cpp -o game
./game
