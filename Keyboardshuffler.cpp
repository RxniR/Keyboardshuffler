#include <windows.h>
#include <map>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>

// Global keyboard hook handle
HHOOK keyboardHook;

// Mapping of original keys to remapped keys
std::map<WPARAM, WPARAM> keyMap;

// List of keys that will be remapped
std::vector<WPARAM> keys = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    '0','1','2','3','4','5','6','7','8','9',
    VK_SPACE, VK_RETURN, VK_TAB, VK_BACK
};

// Flag to keep the main loop running
bool running = true;

/*
    Randomly shuffles the list of keys and updates the keyMap.
    This causes the keyboard keys to behave unpredictably every 5 seconds.
*/
void shuffleKeys() {
    std::vector<WPARAM> shuffled = keys;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);

    for (size_t i = 0; i < keys.size(); ++i) {
        keyMap[keys[i]] = shuffled[i];
    }

    std::cout << "[Keymap shuffled]\n";
}

/*
    Callback function for the global low-level keyboard hook.
    Intercepts key presses and replaces them with remapped keys from keyMap.
    Also exits the program cleanly if ESC is pressed.
*/
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        WPARAM vkCode = p->vkCode;

        // Pressing ESC exits the program
        if (vkCode == VK_ESCAPE) {
            running = false;
            PostQuitMessage(0); // Triggers exit from message loop
            return 1; // Block original ESC key
        }

        // Replace the pressed key with its remapped version
        if (keyMap.find(vkCode) != keyMap.end()) {
            INPUT input = { 0 };
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = keyMap[vkCode];
            SendInput(1, &input, sizeof(INPUT));
            return 1; // Block original key press
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/*
    Runs in a separate thread. Every 5 seconds, it reshuffles the keyMap,
    creating the "annoying" effect of keys constantly changing behavior.
*/
void startShuffleLoop() {
    while (running) {
        shuffleKeys();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

/*
    Entry point of the program.
    - Installs the global keyboard hook
    - Starts the shuffle thread
    - Waits for user input (specifically ESC) to exit cleanly
*/
int main() {
    std::cout << "Annoying Key Rebind Program started.\n";
    std::cout << "Every 5 seconds, your keys will be randomly remapped.\n";
    std::cout << "Press ESC to stop the madness and exit.\n";

    // Initial shuffle
    std::thread(shuffleKeys).detach();

    // Start reshuffling loop in background
    std::thread(startShuffleLoop).detach();

    // Install low-level keyboard hook
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (!keyboardHook) {
        std::cerr << "Failed to install keyboard hook!\n";
        return 1;
    }

    // Message loop to keep the program alive
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    UnhookWindowsHookEx(keyboardHook);
    std::cout << "Exiting program.\n";
    return 0;
}
