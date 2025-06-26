# This tool is for educational and entertainment purposes only. The author is not responsible for misuse or malicious repackaging.
## Annoying Key Rebinder

This C++ program remaps your keyboard keys randomly every 5 seconds, making typing a chaotic experience. It's intended as a harmless prank, educational project, or a demonstration of using global low-level keyboard hooks in Windows.

### 🧠 How It Works

- A global keyboard hook (`WH_KEYBOARD_LL`) intercepts all key presses.
- Every 5 seconds, the program randomly reshuffles common keys (A–Z, 0–9, space, enter, etc.).
- When a key is pressed, it's replaced with another based on the current random mapping.
- Pressing `ESC` cleanly exits the program.

### 💻 Features

- Remaps common keys (letters, numbers, and a few control keys).
- Runs with a visible console window (no stealth or hidden behavior).
- ESC key gracefully shuts down the program.
- Outputs messages to the console for visibility and debugging.
- Fully documented and open-source.

### ⚠️ Antivirus & False Positives

Due to the nature of this program (keyboard hooking and input simulation), some antivirus software, browsers, or [VirusTotal](https://www.virustotal.com/) may flag the executable as **potentially malicious**.

This is expected behavior for programs that:
- Interact with global keyboard input
- Use Windows API functions like `SetWindowsHookEx`, `SendInput`, etc.

#### ✅ Transparency

**You can and should review the `Keyboardshuffler.cpp` file yourself**. The full source code is included in this repository and is short, readable, and fully commented.

> **Never download or run any executable you do not trust.**

If you want to be completely safe, compile the code yourself using Visual Studio.

#### Downloading

### Steps

1. Enter the Releases page in the repository.
2. Click the Keyboardshuffler.exe link.
3. The browser will give you a warning, sadly, but you can simply click the three dots, click keep, click show more, then click keep again.
4. Run the `.exe` file.

#### Compiling the project with Visual Studio (Optional)
### Prerequisites

- Windows
- Visual Studio (Community edition is fine)
- Basic knowledge of compiling C++ console applications

1. Clone or download this repository.
2. Open the `.sln` or `.cpp` file in Visual Studio.
3. Build the project (preferably in **Release** mode).
4. Run the `.exe` file.

Make sure to run the executable **as Administrator** if the hook doesn't activate properly.

### 🧪 Example Behavior

After a few seconds, typing "HELLO" might result in "4WZTI". Five seconds later, it might become "P90LA". Every 5 seconds, the mapping changes again.

### ✅ Safe Exit

To stop the madness, just press `ESC`. The program will unhook itself and exit cleanly.

### 📂 Files

- `Keyboardshuffler.cpp` – The full source code.
- `README.md` – You're reading it.

---

#### Author

Made by RxniR
Feel free to fork, modify, and experiment, however malicious intent will not be tolerated!

