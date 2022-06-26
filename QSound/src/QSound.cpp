#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>
#include <thread>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int nShow;
void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    
    horizontal = desktop.right;
    vertical = desktop.bottom;
}
void ButtonListen(HWND hwnd)
{
    if (GetAsyncKeyState(0x51))
    {
        OutputDebugStringA("Q pressed...\n");
    }
}
bool Stop = false;
bool Hiden = false;
std::string message;
HWND hwnd;
HWND message_input_box;
wchar_t input_message[100];

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Qmain";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    nShow = nCmdShow;
    hwnd = CreateWindowEx(0, CLASS_NAME, L"QSound", WS_OVERLAPPEDWINDOW, //main window
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,       // Parent window    
        NULL,
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    SetWindowLong(hwnd, GWL_STYLE, 0); //hide title bar
    auto hFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI"); //create font
    
    //ES_MULTILINE, ES_AUTOVSCROLL or ES_WANTRETURN

    message_input_box = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, //input box
        50, 25, 200, 18, 
        hwnd, (HMENU)2, hInstance, 0);

    SendMessage(message_input_box, WM_SETFONT, WPARAM(hFont), TRUE); //set font for input box
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, 0); //set window on top 

    if (hwnd == NULL)
    {
        return 0;
    }

    //ShowWindow(hwnd, nCmdShow);
    ShowWindow(hwnd, nShow);

    int screen_width = 0, screen_height = 0;
    int window_width, window_height;
    window_width = 300;
    window_height = 100;
    GetDesktopResolution(screen_width, screen_height);
    SetWindowPos(hwnd, NULL, 
        (screen_width - window_width),
        0,
        300, 100, NULL);

    MSG msg = { };
    

    while (!Stop)
    {
        if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN) { //turn off error like sound when Enter pressed
                GetWindowText(GetDlgItem(hwnd, 2), input_message, 100);
                OutputDebugString(input_message);
                SetWindowTextA(message_input_box, ""); //clear input box
                ShowWindow(hwnd, false); //hide window
               

                Hiden = true;
                continue;
            }
           
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            
            
            
        }
        else
        {
            if ((GetAsyncKeyState(0x51) & 0x01))
            {
                if (Hiden) //if window hiden -> show
                { 
                    ShowWindow(hwnd, true); 
                    SetFocus(message_input_box);
                    Hiden = false;
                }
                else if (!Hiden) //if window visible -> hide
                {
                    ShowWindow(hwnd, false); 
                    SetFocus(message_input_box);
                    Hiden = true;
                }
            }
        }
    }
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ACTIVATE: {
        return 0;
    }
    
    case WM_DESTROY:
        Stop = true;
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HBRUSH brush = CreateSolidBrush(RGB(0, 125, 125)); //set background color
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(brush));
        
        EndPaint(hwnd, &ps);
        return 0;
    }
    //case WM_KEYDOWN:
    case WM_KEYDOWN:
        switch (wParam)
        {
        //combination Ctrl + shift + Q
        //case VK_CONTROL: //TODO: Implement combinations support 

        case VK_CONTROL: //Ctrl key
            
            break;
        case 0x51: //Q key
           
            
            Hiden = !Hiden;
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
            break;
        }
        break;
    return 0;

    }
    
}