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

- C++ (C++17)
- Standard Library only (no external dependencies)

## 🚀 How To Compile And Run

### Linux / MacOS
g++ -std=c++17 main.cpp -o game
./game

### Windows (with MinGW)
g++ -std=c++17 main.cpp -o game.exe
game.exe

### Windows (Visual Studio)
1. Create new Console Application project
2. Replace the content of `main.cpp` with my code
3. Build and run (Ctrl+F5)

## 📁 Project Structure

Just one file for now — `main.cpp`. I'm learning how to structure 
code properly, and my next project will have better organization 
with classes and multiple files.

## 🎯 What I Learned

- Basic C++ syntax (loops, conditions, functions)
- Working with arrays and vectors
- File I/O for save/load system
- Random number generation
- User input handling
- Game loop logic

## 🔜 Next Steps

My next project will be better organized with:

- Multiple files and classes
- Better code structure
- Clean architecture
- More comments in English

## 📫 Contact

Alex - tg: @astbrow

Project link: [https://github.com/yourusername/console-roguelike](https://github.com/yourusername/console-roguelike)
