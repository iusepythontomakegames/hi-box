#include <windows.h>

#define BOX_SIZE 80

static int  boxX, boxY;
static RECT gClient;

static void SnapBoxToTopLeft(void) {
    boxX = 0;
    boxY = 0;
}

static void CenterBox(void) {
    boxX = (gClient.right  - BOX_SIZE) / 2;
    boxY = (gClient.bottom - BOX_SIZE) / 2;
    if (boxX < 0) boxX = 0;
    if (boxY < 0) boxY = 0;
}

static LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_CREATE:
        GetClientRect(h, &gClient);
        CenterBox();
        return 0;

    case WM_SIZE:
        GetClientRect(h, &gClient);
        return 0;

    case WM_KEYDOWN:
        if (w == 'B') {
            SnapBoxToTopLeft();
            InvalidateRect(h, NULL, TRUE);
        }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        RECT rc;
        GetClientRect(h, &rc);

        FillRect(dc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

        HBRUSH brush = CreateSolidBrush(RGB(214, 62, 62));
        RECT box;
        box.left   = boxX;
        box.top    = boxY;
        box.right  = boxX + BOX_SIZE;
        box.bottom = boxY + BOX_SIZE;
        FillRect(dc, &box, brush);
        DeleteObject(brush);

        HFONT font = CreateFontA(-16, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                                 DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                 CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                 DEFAULT_PITCH | FF_SWISS, "Segoe UI");
        HGDIOBJ old = SelectObject(dc, font);
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(90, 90, 90));
        RECT tr = rc;
        tr.top = tr.bottom - 28;
        DrawTextA(dc, "Press B to snap the box to the top left", -1, &tr,
                  DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        SelectObject(dc, old);
        DeleteObject(font);

        EndPaint(h, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(h, m, w, l);
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {
    (void)prev; (void)cmd;

    WNDCLASSEXA wc;
    ZeroMemory(&wc, sizeof wc);
    wc.cbSize        = sizeof wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = inst;
    wc.hCursor       = LoadCursorA(NULL, (LPCSTR)IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = "HiBoxWindow";
    if (!RegisterClassExA(&wc)) return 1;

    RECT want = { 0, 0, 640, 480 };
    AdjustWindowRect(&want, WS_OVERLAPPEDWINDOW, FALSE);

    HWND h = CreateWindowExA(0, "HiBoxWindow", "hi box",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             want.right - want.left,
                             want.bottom - want.top,
                             NULL, NULL, inst, NULL);
    if (!h) return 1;

    ShowWindow(h, show);
    UpdateWindow(h);
    SetFocus(h);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 0;
}
