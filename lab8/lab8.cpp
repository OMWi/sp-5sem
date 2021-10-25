#ifndef UNICODE
#define UNICODE
#endif 

#define ID_IDLE 10
#define ID_NORMAL 11
#define ID_HIGH 12
#define ID_REALTIME 13
#define ID_LISTBOX1 100
#define ID_LISTBOX2 101

#include <Windows.h>
#include <TlHelp32.h>
#include <windowsx.h>
#include <psapi.h>

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

    HWND hwnd = CreateWindow(CLASS_NAME, L"Title", WS_CAPTION | WS_SYSMENU,
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



PROCESSENTRY32 procs[1024];
HWND listBox1;
HWND listBox2;
DWORD aProcesses[1024];
HMENU hPopupMenu;

void setPriority(DWORD priorityClass, HWND listBox) {
    int itemId = SendMessage(listBox, LB_GETCURSEL, 0, NULL);
    if (itemId != -1) {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procs[itemId].th32ProcessID);
        SetPriorityClass(hProcess, priorityClass);
        CloseHandle(hProcess);
    }
}

void getModules(DWORD pid, HWND listBox)
{
    HANDLE th = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (th != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 mod;
        mod.dwSize = sizeof(MODULEENTRY32);
        Module32First(th, &mod);
        do {
            SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)mod.szModule);
        } while (Module32Next(th, &mod));
    }
    CloseHandle(th);
}

void getProcesses(HWND listBox)
{
    int procCount = 0;
    procs[0].dwSize = sizeof(PROCESSENTRY32);

    HANDLE th = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    Process32First(th, &procs[0]);
    while (Process32Next(th, &procs[procCount]))
    {
        procCount++;
        procs[procCount].dwSize = sizeof(PROCESSENTRY32);
    }
    CloseHandle(th);
    for (int i = 0; i < procCount; i++) {
        SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)procs[i].szExeFile);
    }
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

        listBox1 = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            0, 0, rect.right/2, rect.bottom, hwnd, (HMENU)ID_LISTBOX1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        listBox2 = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            rect.right/2, 0, rect.right/2, rect.bottom, hwnd, (HMENU)ID_LISTBOX2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        getProcesses(listBox1);
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
        int wmid = LOWORD(wParam);
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_LISTBOX1:
                if (wmEvent == LBN_SELCHANGE)
                {
                    int itemId = SendMessage(listBox1, LB_GETCURSEL, 0, NULL);
                    if (itemId != -1) {
                        SendMessage(listBox2, LB_RESETCONTENT, 0, NULL);
                        getModules(procs[itemId].th32ProcessID, listBox2);
                    }
                }
                break;
            case ID_IDLE:
                setPriority(IDLE_PRIORITY_CLASS, listBox1);
                break;
            case ID_NORMAL:
                setPriority(NORMAL_PRIORITY_CLASS, listBox1);
                break;
            case ID_HIGH:
                setPriority(HIGH_PRIORITY_CLASS, listBox1);
                break;
            case ID_REALTIME:
                setPriority(REALTIME_PRIORITY_CLASS, listBox1);
                break;
            default:
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }
    }
    return 0;

    case WM_CONTEXTMENU:
        if ((HWND)wParam == listBox1) {
            int itemId = SendMessage(listBox1, LB_GETCURSEL, 0, NULL);
            if (itemId != -1) {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procs[itemId].th32ProcessID);
                int pC = GetPriorityClass(hProcess);
                CloseHandle(hProcess);

                hPopupMenu = CreatePopupMenu();
                AppendMenu(hPopupMenu, pC == REALTIME_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_REALTIME, L"Real time");
                AppendMenu(hPopupMenu, pC == HIGH_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_HIGH, L"High");
                AppendMenu(hPopupMenu, pC == NORMAL_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_NORMAL, L"Normal");
                AppendMenu(hPopupMenu, pC == IDLE_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_IDLE, L"Idle");



                TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hwnd, NULL);
            }
        }
        break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}