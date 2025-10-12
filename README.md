# C_Text_editor

A simple terminal-based text editor written in C, currently under development.  
Inspired by lightweight editors like *kilo*, this project demonstrates raw-mode terminal input, non-blocking key reading, and basic text handling.

---

## Features (In Progress)
- Raw terminal mode for character-by-character input
- Detection and printing of ASCII and control characters
- Non-blocking input using `read()` with `VMIN` and `VTIME`
- Automatic restoration of terminal settings on exit
- Basic `q` key to quit

---

## Installation
Clone the repository and compile using GCC:

```bash
git clone <repository_url>
cd C_Text_editor
gcc -Wall -Wextra -pedantic editor.c -o editor
