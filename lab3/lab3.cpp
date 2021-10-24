#ifndef UNICODE
#define UNICODE
#endif 

#define id_draw_btn  100
#define id_clear_btn  101

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND drawBtn, clearBtn;

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

    HWND hwnd = CreateWindow(CLASS_NAME, L"Lab3", WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
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



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static RECT rect;
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_CREATE:
        GetClientRect(hwnd, &rect);
        drawBtn = CreateWindow(L"BUTTON", L"Draw", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            0, 500, rect.right/2, rect.bottom-500, hwnd, HMENU(id_draw_btn), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
        );
        clearBtn = CreateWindow(L"BUTTON", L"Clear", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            rect.right/2, 500, rect.right/2, rect.bottom-500, hwnd, HMENU(id_clear_btn), HINSTANCE(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), NULL
        );

        rect.bottom = 500;

        return 0;

    case WM_COMMAND:
    {
        int wmid = LOWORD(wParam);
        switch (wmid)
        {
        case id_draw_btn:
        {
            hdc = GetDC(hwnd);
            FillRect(hdc, &rect, CreateSolidBrush(RGB(241, 224, 214)));
            Rectangle(hdc, 5, 300, 50, 420);
            Rectangle(hdc, 50, 420, 105, 490);
            Rectangle(hdc, 5, 5, 20, 20);
            MoveToEx(hdc, 5, 300, NULL);
            LineTo(hdc, 50, 420);
            MoveToEx(hdc, 5, 420, NULL);
            LineTo(hdc, 50, 300);
            MoveToEx(hdc, 50, 420, NULL);
            LineTo(hdc, 105, 490);
            MoveToEx(hdc, 50, 490, NULL);
            LineTo(hdc, 105, 420);
            Ellipse(hdc, 105, 100, 155, 480);
            Ellipse(hdc, 165, 120, 200, 130);
            MoveToEx(hdc, 130, 100, NULL);
            LineTo(hdc, 130, 480);
            MoveToEx(hdc, 105, 290, NULL);
            LineTo(hdc, 155, 290);
            POINT pointArr[] = { {20, 20}, {165, 120}, {180, 100}, {170, 20} };
            Polygon(hdc, pointArr, sizeof(pointArr) / sizeof(pointArr[0]));
            POINT pointArr2[] = { {190, 100}, {200, 80}, {250, 60} };
            Polygon(hdc, pointArr2, sizeof(pointArr2) / sizeof(pointArr2[0]));
            POINT pointArr3[] = { {250, 50}, {400, 40}, {700, 20}, {730, 40}, {750, 60} };
            Polygon(hdc, pointArr3, sizeof(pointArr3) / sizeof(pointArr3[0]));
            Arc(hdc, 300, 300, 400, 400, 500, 350, 600, 450);
            Arc(hdc, 50, 300, 105, 420, 50, 300, 105, 420);
            break;
        }            

        case id_clear_btn:
            InvalidateRect(hwnd, &rect, TRUE);
            break;
        }
    }
    return 0;

    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT info = (LPDRAWITEMSTRUCT)lParam;
        RECT innerRect = info->rcItem;
        SetBkMode(info->hDC, TRANSPARENT);
        SetTextColor(info->hDC, RGB(240, 240, 240));

        if (info->CtlID == id_draw_btn) {
            LPCWSTR text = L"Draw";
            FillRect(info->hDC, &innerRect, CreateSolidBrush(RGB(89, 87, 117)));
            DrawText(info->hDC, text, wcslen(text), &innerRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        }
        if (info->CtlID == id_clear_btn) {
            LPCWSTR text = L"Clear";
            FillRect(info->hDC, &innerRect, CreateSolidBrush(RGB(189, 152, 143)));
            DrawText(info->hDC, text, wcslen(text), &innerRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}