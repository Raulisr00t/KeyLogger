#include <windows.h>
#include <iostream>
#include <winuser.h>

using namespace std;

HHOOK g_hMouseHook = NULL;

LRESULT CALLBACK HookCallBack(int nCode, WPARAM wParam, LPARAM lParam) {
    if (wParam == WM_LBUTTONDOWN) {
        cout << "[#] Left Mouse Click" << endl;
    }
    if (wParam == WM_RBUTTONDOWN) {
        cout << "[#] Right Mouse Click" << endl;
    }
    if (wParam == WM_MBUTTONDOWN) {
        cout << "[#] Middle Mouse Click" << endl;
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL MouseLogger() {
    MSG Msg = { 0 };
    g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, HookCallBack, NULL, 0);

    if (!g_hMouseHook) {
        cout << "[!] Error in Mouse Logging!" << endl;
        cout << "[!] Error: " << GetLastError() << endl;
        return FALSE;
    }

    while (GetMessageW(&Msg, NULL, 0, 0)) {
        DefWindowProcW(Msg.hwnd, Msg.message, Msg.wParam, Msg.lParam);
    }

    return TRUE;
}

int main(int argc, char* argv[]) {
    HANDLE hThread;
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MouseLogger, NULL, 0, NULL);
    if (hThread == NULL || hThread == INVALID_HANDLE_VALUE) {
        cout << "[!] Error in Creating Thread!" << endl;
        cout << "[!] Error: " << GetLastError() << endl;
        return 1;
    }
    WaitForSingleObject(hThread, 10000); // if you want increase seconds configure 10000

    if (g_hMouseHook && !UnhookWindowsHookEx(g_hMouseHook)) {
        cout << "[!] UnhookWindowsHookEx Failed With Error: " << GetLastError() << endl;
        return 1;
    }

    return 0;
}
