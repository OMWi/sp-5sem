#ifndef UNICODE
#define UNICODE
#endif 

#define ID_LISTBOX 100
#define ID_EDITBOX 101
#define ID_SEARCHBUTTON 102

#include <Windows.h>
#include <strsafe.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(CLASS_NAME, L"Lab 9", WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL
    );
    if (hwnd == NULL)
    {
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}



HWND hListBox, hEdit, hSearchButton;

int x = 1000;
void SearchRec(HWND hwnd, HKEY hKeyRoot, LPTSTR lpSubKey, TCHAR* s)
{
    if (--x <= 0) return;
    LPTSTR lpEnd;
    LONG lResult;
    DWORD dwSize;
    TCHAR szName[MAX_PATH];
    HKEY hKey;
    FILETIME ftWrite;

    lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        return;
    }

    if (wcsstr(lpSubKey, s) != NULL)
        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)lpSubKey);

    lpEnd = lpSubKey + lstrlen(lpSubKey);

    if (*(lpEnd - 1) != TEXT('\\'))
    {
        *lpEnd = TEXT('\\');
        lpEnd++;
        *lpEnd = TEXT('\0');
    }

    dwSize = MAX_PATH;
    lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
        NULL, NULL, &ftWrite);

    if (lResult == ERROR_SUCCESS)
    {
        int index = 0;
        do {
            index++;

            StringCchCopy(lpEnd, MAX_PATH * 2, szName);

            SearchRec(hwnd, hKeyRoot, lpSubKey, s);
            if (x <= 0) return;

            dwSize = MAX_PATH;

            lResult = RegEnumKeyEx(hKey, index, szName, &dwSize, NULL,
                NULL, NULL, &ftWrite);

        } while (lResult == ERROR_SUCCESS);
    }

    lpEnd--;
    *lpEnd = TEXT('\0');

    RegCloseKey(hKey);
}

void Search(HWND hwnd, TCHAR* s) {
    SendMessage(hwnd, LB_RESETCONTENT, 0, 0);

    LPTSTR lpSubKey = (LPTSTR)L"Software";
    TCHAR szSearchKey[MAX_PATH * 2];
    StringCchCopy(szSearchKey, MAX_PATH * 2, lpSubKey);

    x = 1000;
    SearchRec(hwnd, HKEY_LOCAL_MACHINE, szSearchKey, s);
}   

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        RECT rect;
        GetClientRect(hwnd, &rect);
        hListBox = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT,
            0, 0, rect.right, rect.bottom - 25, hwnd, (HMENU)ID_LISTBOX, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        hEdit = CreateWindow(L"edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, rect.bottom - 25, 200, 25, hwnd, (HMENU)ID_EDITBOX, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        hSearchButton = CreateWindow(L"button", L"Search", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            205, rect.bottom - 25, 80, 25, hwnd, (HMENU)ID_SEARCHBUTTON, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case ID_SEARCHBUTTON:
        {
            TCHAR buffer[1024];
            int textLen = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
            SendMessage(hEdit, WM_GETTEXT, textLen + 1, (LPARAM)buffer);
            Search(hListBox, buffer);
        }
        break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}