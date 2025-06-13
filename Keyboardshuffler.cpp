#include <windows.h>
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

HHOOK keyboardHook;
std::map<WPARAM, WPARAM> keyMap;
std::vector<WPARAM> keys = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    '0','1','2','3','4','5','6','7','8','9',
    VK_SPACE, VK_RETURN, VK_TAB, VK_BACK
};

// Remap keys randomly
void shuffleKeys() {
    std::vector<WPARAM> shuffled = keys;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);

    for (size_t i = 0; i < keys.size(); ++i) {
        keyMap[keys[i]] = shuffled[i];
    }

    std::cout << "[KEYMAP SHUFFLED]\n";
}

// Hook callback
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        WPARAM vkCode = p->vkCode;

        if (keyMap.find(vkCode) != keyMap.end()) {
            INPUT input = { 0 };
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = keyMap[vkCode];
            SendInput(1, &input, sizeof(INPUT));
            return 1; // suppress original key
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Key shuffler thread
void startShuffleLoop() {
    while (true) {
        shuffleKeys();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
    std::thread(shuffleKeys).detach(); // initial shuffle
    std::thread(startShuffleLoop).detach();

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (!keyboardHook) {
        std::cerr << "Failed to install hook!\n";
        return 1;
    }

    std::cout << "Annoying key rebind program started. Press ESC to exit.\n";

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_KEYDOWN && ((KBDLLHOOKSTRUCT*)msg.lParam)->vkCode == VK_ESCAPE)
            break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}
