#define UNICODE
#define _UNICODE
#include <windows.h>
#include <shellapi.h> n
#include "resource.h"

HWND g_hMainWindow = NULL;
BOOL g_bTimerRunning = FALSE;
UINT_PTR g_nTimerID = 1;
DWORD g_dwRemainingTime = 0;
NOTIFYICONDATA g_nid;

#define IDC_TASK_LABEL          1001
#define IDC_TASK_EDIT           1002
#define IDC_HOURS_LABEL         1003
#define IDC_MINUTES_LABEL       1004
#define IDC_SECONDS_LABEL       1005
#define IDC_HOURS_EDIT          1006
#define IDC_MINUTES_EDIT        1007
#define IDC_SECONDS_EDIT        1008
#define IDC_COUNTDOWN_DISPLAY   1009
#define IDC_START_BUTTON        1010
#define IDC_RESET_BUTTON        1011

HWND g_hTaskEdit;
HWND g_hHoursEdit;
HWND g_hMinutesEdit;
HWND g_hSecondsEdit;
HWND g_hCountdownDisplay;
HWND g_hStartButton;
HWND g_hResetButton;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void UpdateCountdownDisplay(HWND hwnd);
void EnableInputFields(BOOL bEnable);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"FocusClockXPClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    g_hMainWindow = CreateWindowEx(
        0,                                  
        L"FocusClockXPClass",              
        L"Focus Clock XP",                 
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
        CW_USEDEFAULT, CW_USEDEFAULT,      
        400, 300,                           
        NULL,                               
        NULL,                               
        hInstance,                          
        NULL                                
    );

    if (g_hMainWindow == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(g_hMainWindow, nCmdShow);
    UpdateWindow(g_hMainWindow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void UpdateCountdownDisplay(HWND hwnd)
{
    DWORD dwHours = g_dwRemainingTime / 3600;
    DWORD dwMinutes = (g_dwRemainingTime % 3600) / 60;
    DWORD dwSeconds = g_dwRemainingTime % 60;

    WCHAR szTime[9];
    wsprintf(szTime, L"%02d:%02d:%02d", dwHours, dwMinutes, dwSeconds);
    SetWindowText(g_hCountdownDisplay, szTime);
}

void EnableInputFields(BOOL bEnable)
{
    EnableWindow(g_hTaskEdit, bEnable);
    EnableWindow(g_hHoursEdit, bEnable);
    EnableWindow(g_hMinutesEdit, bEnable);
    EnableWindow(g_hSecondsEdit, bEnable);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            CreateWindowEx(
                0, L"STATIC", L"Task:",
                WS_VISIBLE | WS_CHILD,
                20, 20, 50, 20, 
                hwnd, (HMENU)IDC_TASK_LABEL, GetModuleHandle(NULL), NULL
            );

            g_hTaskEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                75, 17, 280, 24, 
                hwnd, (HMENU)IDC_TASK_EDIT, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"STATIC", L"H",
                WS_VISIBLE | WS_CHILD,
                20, 60, 20, 20,
                hwnd, (HMENU)IDC_HOURS_LABEL, GetModuleHandle(NULL), NULL
            );

            g_hHoursEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE, L"EDIT", L"00",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER,
                45, 57, 40, 24,
                hwnd, (HMENU)IDC_HOURS_EDIT, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"STATIC", L"M",
                WS_VISIBLE | WS_CHILD,
                100, 60, 20, 20,
                hwnd, (HMENU)IDC_MINUTES_LABEL, GetModuleHandle(NULL), NULL
            );

            g_hMinutesEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE, L"EDIT", L"00",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER,
                125, 57, 40, 24,
                hwnd, (HMENU)IDC_MINUTES_EDIT, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"STATIC", L"S",
                WS_VISIBLE | WS_CHILD,
                180, 60, 20, 20,
                hwnd, (HMENU)IDC_SECONDS_LABEL, GetModuleHandle(NULL), NULL
            );

            g_hSecondsEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE, L"EDIT", L"00",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER,
                205, 57, 40, 24,
                hwnd, (HMENU)IDC_SECONDS_EDIT, GetModuleHandle(NULL), NULL
            );

            g_hCountdownDisplay = CreateWindowEx(
                0, L"STATIC", L"00:00:00",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                20, 100, 350, 50,
                hwnd, (HMENU)IDC_COUNTDOWN_DISPLAY, GetModuleHandle(NULL), NULL
            );

            g_hStartButton = CreateWindowEx(
                0, L"BUTTON", L"Start",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                100, 180, 80, 30,
                hwnd, (HMENU)IDC_START_BUTTON, GetModuleHandle(NULL), NULL
            );

            g_hResetButton = CreateWindowEx(
                0, L"BUTTON", L"Reset",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                200, 180, 80, 30,
                hwnd, (HMENU)IDC_RESET_BUTTON, GetModuleHandle(NULL), NULL
            );

            ZeroMemory(&g_nid, sizeof(NOTIFYICONDATA));
            g_nid.cbSize = sizeof(NOTIFYICONDATA);
            g_nid.hWnd = hwnd;
            g_nid.uID = 100; 
            g_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
            g_nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); 
            g_nid.uCallbackMessage = WM_APP + 1; 
            lstrcpy(g_nid.szTip, L"Focus Clock XP"); 
            Shell_NotifyIcon(NIM_ADD, &g_nid);

            break;
        }
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
                case IDC_START_BUTTON:
                {
                    if (!g_bTimerRunning)
                    {
                        WCHAR szHours[3], szMinutes[3], szSeconds[3];
                        GetWindowText(g_hHoursEdit, szHours, 3);
                        GetWindowText(g_hMinutesEdit, szMinutes, 3);
                        GetWindowText(g_hSecondsEdit, szSeconds, 3);

                        DWORD dwHours = _wtoi(szHours);
                        DWORD dwMinutes = _wtoi(szMinutes);
                        DWORD dwSeconds = _wtoi(szSeconds);

                        g_dwRemainingTime = (dwHours * 3600) + (dwMinutes * 60) + dwSeconds;

                        if (g_dwRemainingTime > 0)
                        {
                            g_bTimerRunning = TRUE;
                            SetWindowText(g_hStartButton, L"Stop");
                            EnableInputFields(FALSE);
                            SetTimer(hwnd, g_nTimerID, 1000, NULL);
                            UpdateCountdownDisplay(hwnd);

                            WCHAR szTaskName[256];
                            GetWindowText(g_hTaskEdit, szTaskName, 256);
                            WCHAR szTipText[256];
                            wsprintf(szTipText, L"Focus Clock XP - %s (%02d:%02d:%02d remaining)", szTaskName, dwHours, dwMinutes, dwSeconds);
                            lstrcpy(g_nid.szTip, szTipText);
                            Shell_NotifyIcon(NIM_MODIFY, &g_nid);
                        }
                    }
                    else
                    {
                        g_bTimerRunning = FALSE;
                        KillTimer(hwnd, g_nTimerID);
                        SetWindowText(g_hStartButton, L"Start");
                        EnableInputFields(TRUE);

                        lstrcpy(g_nid.szTip, L"Focus Clock XP");
                        Shell_NotifyIcon(NIM_MODIFY, &g_nid);
                    }
                    break;
                }
                case IDC_RESET_BUTTON:
                {
                    g_bTimerRunning = FALSE;
                    KillTimer(hwnd, g_nTimerID);
                    g_dwRemainingTime = 0;
                    UpdateCountdownDisplay(hwnd);
                    SetWindowText(g_hStartButton, L"Start");
                    EnableInputFields(TRUE);
                    SetWindowText(g_hTaskEdit, L"");
                    SetWindowText(g_hHoursEdit, L"00");
                    SetWindowText(g_hMinutesEdit, L"00");
                    SetWindowText(g_hSecondsEdit, L"00");

                    lstrcpy(g_nid.szTip, L"Focus Clock XP");
                    Shell_NotifyIcon(NIM_MODIFY, &g_nid);
                    break;
                }
            }
            break;
        }
        case WM_TIMER:
        {
            if (wParam == g_nTimerID)
            {
                if (g_dwRemainingTime > 0)
                {
                    g_dwRemainingTime--;
                    UpdateCountdownDisplay(hwnd);

                    WCHAR szTaskName[256];
                    GetWindowText(g_hTaskEdit, szTaskName, 256);
                    WCHAR szTipText[256];
                    DWORD dwHours = g_dwRemainingTime / 3600;
                    DWORD dwMinutes = (g_dwRemainingTime % 3600) / 60;
                    DWORD dwSeconds = g_dwRemainingTime % 60;
                    wsprintf(szTipText, L"Focus Clock XP - %s (%02d:%02d:%02d remaining)", szTaskName, dwHours, dwMinutes, dwSeconds);
                    lstrcpy(g_nid.szTip, szTipText);
                    Shell_NotifyIcon(NIM_MODIFY, &g_nid);
                }
                else
                {
                    KillTimer(hwnd, g_nTimerID);
                    g_bTimerRunning = FALSE;
                    SetWindowText(g_hStartButton, L"Start");
                    EnableInputFields(TRUE);
                    PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

                    lstrcpy(g_nid.szTip, L"Focus Clock XP");
                    Shell_NotifyIcon(NIM_MODIFY, &g_nid);
                }
            }
            break;
        }
        case WM_CLOSE:
        {
            if (g_bTimerRunning)
            {
                WCHAR szTaskName[256];
                GetWindowText(g_hTaskEdit, szTaskName, 256);
                WCHAR szMessage[512];
                wsprintf(szMessage, L"The task \"%s\" is running. Are you sure you want to quit?", szTaskName);
                if (MessageBox(hwnd, szMessage, L"Confirm Exit", MB_YESNO | MB_ICONQUESTION) == IDYES)
                {
                    Shell_NotifyIcon(NIM_DELETE, &g_nid); 
                    DestroyWindow(hwnd);
                }
            }
            else
            {
                Shell_NotifyIcon(NIM_DELETE, &g_nid); 
                DestroyWindow(hwnd);
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


