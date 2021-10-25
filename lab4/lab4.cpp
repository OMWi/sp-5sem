#ifndef UNICODE
#define UNICODE
#endif 

#define IDT_ANIM 100
#define IDT_MOVE 101

#include <Windows.h>
#include <math.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HBITMAP* hImages = new HBITMAP[4];
POINT destination;
LONG curX = 0, curY = 0;
INT imageIndex = 0;
BOOL moving = FALSE;


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

    HWND hwnd = CreateWindow(CLASS_NAME, L"Lab4", WS_CAPTION | WS_SYSMENU,
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




LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static RECT rect;
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        hImages[0] = (HBITMAP)LoadImage((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            L"C:/Projects/sp-5sem/lab4/images/image1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        hImages[1] = (HBITMAP)LoadImage((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            L"C:/Projects/sp-5sem/lab4/images/image2.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        hImages[2] = (HBITMAP)LoadImage((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            L"C:/Projects/sp-5sem/lab4/images/image3.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        hImages[3] = (HBITMAP)LoadImage((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            L"C:/Projects/sp-5sem/lab4/images/image4.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        SetTimer(hwnd, IDT_ANIM, 150, NULL);
        SetTimer(hwnd, IDT_MOVE, 1, NULL);
        return 0;

    case WM_LBUTTONDOWN:
        if (moving == FALSE)
        {
            moving = TRUE;
        }
        destination.x = LOWORD(lParam) - 16;
        destination.y = HIWORD(lParam) - 16;
        return 0;


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc, hdcShadow;
        BITMAP bitmap;
        HGDIOBJ prevBitmap;
        hdc = BeginPaint(hwnd, &ps);

        hdcShadow = CreateCompatibleDC(hdc);
        prevBitmap = SelectObject(hdcShadow, hImages[imageIndex]);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        GetObject(hImages[imageIndex], sizeof(bitmap), &bitmap);
        BitBlt(hdc, curX, curY, bitmap.bmWidth, bitmap.bmHeight, hdcShadow, 0, 0, SRCCOPY);
        SelectObject(hdcShadow, prevBitmap);
        ReleaseDC(hwnd, hdc);
        
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_TIMER:
        switch (wParam)
        {
        case IDT_ANIM:
            imageIndex += 1;
            if (imageIndex > 3) {
                imageIndex = 0;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;

        case IDT_MOVE:
            if (moving) {
                if (curX == destination.x && curY == destination.y) {
                    moving = false;
                    break;
                }
                int distance = sqrt(pow((destination.x - curX), 2) + pow((destination.y - curY), 2));
                double sinVal = (double)(destination.y - curY) / (double)distance;
                double cosVal = (double)(destination.x - curX) / (double)distance;
                curX += 10 * cosVal;
                curY += 10 * sinVal;
                if (abs(curX - destination.x) < 10 && abs(curY - destination.y) < 10) {
                    curX = destination.x;
                    curY = destination.y;
                }
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
            }
            break;
        }
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}