#include <string.h>

#include "d_main.h"
#include "doomdef.h"
#include "doomstat.h"
#include "i_system.h"
#include "i_video.h"
#include "v_video.h"

#include "d_event.h"

#define boolean win_boolean
#include <windows.h>
#undef boolean

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 600

static HWND window;
static HINSTANCE instance;

static BITMAPINFO bitmap_info;
static byte framebuffer[SCREENWIDTH * SCREENHEIGHT * 4];

static byte palette[256 * 3];

static int
TranslateKey(WPARAM key)
{
    int rc;

    switch (key)
    {
        case VK_LEFT:
            return KEY_LEFTARROW;
        case VK_RIGHT:
            return KEY_RIGHTARROW;
        case VK_DOWN:
            return KEY_DOWNARROW;
        case VK_UP:
            return KEY_UPARROW;
        case VK_ESCAPE:
            return KEY_ESCAPE;
        case VK_RETURN:
            return KEY_ENTER;
        case VK_TAB:
            return KEY_TAB;

        case VK_F1:
            return KEY_F1;
        case VK_F2:
            return KEY_F2;
        case VK_F3:
            return KEY_F3;
        case VK_F4:
            return KEY_F4;
        case VK_F5:
            return KEY_F5;
        case VK_F6:
            return KEY_F6;
        case VK_F7:
            return KEY_F7;
        case VK_F8:
            return KEY_F8;
        case VK_F9:
            return KEY_F9;
        case VK_F10:
            return KEY_F10;
        case VK_F11:
            return KEY_F11;
        case VK_F12:
            return KEY_F12;

        case VK_BACK:
        case VK_DELETE:
            return KEY_BACKSPACE;

        case VK_PAUSE:
            return KEY_PAUSE;
        case VK_OEM_PLUS:
            return KEY_EQUALS;
        case VK_ADD:
            return KEY_EQUALS;
        case VK_OEM_MINUS:
            return KEY_MINUS;
        case VK_SUBTRACT:
            return KEY_MINUS;

        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            return KEY_RSHIFT;

        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            return KEY_RCTRL;

        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            return KEY_RALT;
    }

    if (key >= 'A' && key <= 'Z')
    {
        return key - 'A' + 'a';
    }

    if (key >= '0' && key <= '9')
    {
        return (int)key;
    }

    if (rc >= 'A' && rc <= 'Z')
    {
        rc = rc - 'A' + 'a';
    }

    if (rc >= ' ' && rc <= '~')
    {
        return rc;
    }

    return 0;
}

static LRESULT CALLBACK
WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CLOSE:
            I_Quit();
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            event_t event;

            event.type = ev_keydown;
            event.data1 = TranslateKey(wParam);
            event.data2 = 0;
            event.data3 = 0;

            if (event.data1)
            {
                D_PostEvent(&event);
            }

            return 0;
        }

        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            event_t event;

            event.type = ev_keyup;
            event.data1 = TranslateKey(wParam);
            event.data2 = 0;
            event.data3 = 0;

            if (event.data1)
            {
                D_PostEvent(&event);
            }

            return 0;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void
I_InitGraphics(void)
{
    WNDCLASS window_class;

    instance = GetModuleHandle(NULL);

    memset(&window_class, 0, sizeof(window_class));

    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = instance;
    window_class.lpszClassName = "DOOM32";

    RegisterClass(&window_class);

    RECT window_rect;

    window_rect.left = 0;
    window_rect.top = 0;
    window_rect.right = WINDOW_WIDTH;
    window_rect.bottom = WINDOW_HEIGHT;

    AdjustWindowRect(&window_rect,
                     WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
                     FALSE);

    window = CreateWindow(
        "DOOM32", "DOOM",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT,
        CW_USEDEFAULT, window_rect.right - window_rect.left,
        window_rect.bottom - window_rect.top, NULL, NULL, instance, NULL);

    memset(&bitmap_info, 0, sizeof(bitmap_info));

    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = SCREENWIDTH;
    bitmap_info.bmiHeader.biHeight = -SCREENHEIGHT;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
}

void
I_ShutdownGraphics(void)
{
    if (window)
    {
        DestroyWindow(window);
        window = NULL;
    }
}

void
I_StartTic(void)
{
    MSG message;

    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

// stubs to keep the linker happy, actually need to implement this shit properly

void
I_StartFrame(void)
{
}

void
I_UpdateNoBlit(void)
{
}

void
I_SetPalette(byte *new_palette)
{
    memcpy(palette, new_palette, sizeof(palette));
}

void
I_ReadScreen(byte *scr)
{
    memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

void
I_FinishUpdate(void)
{
    HDC dc;
    int x;
    int y;

    for (y = 0; y < SCREENHEIGHT; y++)
    {
        for (x = 0; x < SCREENWIDTH; x++)
        {
            byte pixel;

            pixel = screens[0][y * SCREENWIDTH + x];

            framebuffer[(y * SCREENWIDTH + x) * 4 + 0] = palette[pixel * 3 + 2];
            framebuffer[(y * SCREENWIDTH + x) * 4 + 1] = palette[pixel * 3 + 1];
            framebuffer[(y * SCREENWIDTH + x) * 4 + 2] = palette[pixel * 3 + 0];
            framebuffer[(y * SCREENWIDTH + x) * 4 + 3] = 0;
        }
    }

    dc = GetDC(window);

    StretchDIBits(dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, SCREENWIDTH,
                  SCREENHEIGHT, framebuffer, &bitmap_info, DIB_RGB_COLORS,
                  SRCCOPY);

    ReleaseDC(window, dc);
}
