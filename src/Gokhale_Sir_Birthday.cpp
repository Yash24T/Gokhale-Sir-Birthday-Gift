/* 
     _   _                           ____  _      _   _         _             
    | | | | __ _ _ __  _ __  _   _  | __ )(_)_ __| |_| |__   __| | __ _ _   _ 
    | |_| |/ _` | '_ \| '_ \| | | | |  _ \| | '__| __| '_ \ / _` |/ _` | | | |
    |  _  | (_| | |_) | |_) | |_| | | |_) | | |  | |_| | | | (_| | (_| | |_| |
    |_| |_|\__,_| .__/| .__/ \__, | |____/|_|_|   \__|_| |_|\__,_|\__,_|\__, |
                |_|   |_|    |___/                                      |___/ 
*/



#include <Windows.h> 
#include <assert.h> 
#include <stdio.h>
#include <gdiplus.h>
#include <math.h>
#include<mmsystem.h>


using namespace Gdiplus;

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "gdi32.lib") 
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "winmm.lib")

#define TRUE    1
#define FALSE   0

#define VK_A    0x41
#define VK_C    0x43
#define VK_F    0x46
#define VK_S    0x53


#define EXIT_IF(cond, lpszMsg, lpszCaption)     do\
                                                {\
                                                    if((cond))\
                                                    {\
                                                        MessageBox((HWND)NULL,\
                                                                    TEXT(lpszMsg),\
                                                                    TEXT(lpszCaption),\
                                                                    MB_ICONERROR);\
                                                        ExitProcess(EXIT_FAILURE);\
                                                    }\
                                                }while(0) 

using namespace Gdiplus;

#define RIGHTHALRINGTIMER       101
#define LEFTHALFRINGTIMER       102
#define WALKING_SIR_TIMER       103
#define TEXT_TIMER              104

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ToggleFullScreen(void);

void IMAGE(HDC hdc, HWND hWnd, PAINTSTRUCT ps, INT R_X, INT R_Y);
void wave(HDC hdc, HWND hWnd, INT O_X, INT O_Y, INT WIDTH, INT R, INT G, INT B,
             BOOL IsRToLFlag, FLOAT SinWaveLenght);
void ActivateSidePixels(HDC hdc, INT X, INT Y, INT R, INT G, INT B);

void MakeSaturnImage(HDC hdc, HWND hWnd, PAINTSTRUCT ps, INT X_O, INT Y_O);
void Scene1AstroMediComp(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
void MakeSaturnCircle(HDC hdc, PAINTSTRUCT ps, INT X_O, INT Y_O);
void MakeSaturnRingsWithCircle(HDC hdc, PAINTSTRUCT ps, INT X_O, INT Y_O, 
                                INT RingSize, INT RingWidth);
void MakeRingsMove(HDC hdc, PAINTSTRUCT ps, INT X_O, INT Y_O, 
                    INT RING_1_SIZE, INT RING_2_SIZE,
                    INT RING_1_WIDTH, INT RING_2_WIDTH);
void Dub_MakeRingsMove(HDC hdc, PAINTSTRUCT ps, HWND hWnd);
void TextAstroMediComp(HDC hdc, HWND hWnd, PAINTSTRUCT ps, 
                        INT X, INT Y, INT R, INT G, INT B);
void Scene2(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
void MakeAgneepath(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
INT  RandRange(INT START, INT END);
void MakeSir(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
void MyDrawText(HDC hdc, HWND hWnd, PAINTSTRUCT ps, 
                WCHAR* STRING,INT X, INT Y, INT R, INT G, INT B);
void MoveText(HDC hdc, HWND hWnd, PAINTSTRUCT ps, INT StartX, INT StartY, 
                INT EndX, INT EndY, WCHAR* String, 
                INT R, INT G, INT B, BOOL IsLToR);
void CreditWindow(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
void MyDrawText2(HDC hdc, HWND hWnd, PAINTSTRUCT ps, 
                WCHAR* STRING,INT X, INT Y, INT R, INT G, INT B, 
                INT SIZE);
void InstructionWindow(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
void MakeSirframe0(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
void MakeSirframe1(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
void MakeSirframe2(HDC hdc, HWND hWnd, PAINTSTRUCT ps);
void MakeSirframe3(HDC hdc, HWND hWnd, PAINTSTRUCT ps);

//Window Global Variables
INT cxsize, cysize;
HWND ghWnd = NULL; 
BOOL gbFullScreen = FALSE; 
DWORD dwWindowStyle; 
WINDOWPLACEMENT gpPreviousWindowPlacement;

//Flags
INT StartAstroMediCompSceneFlag = FALSE;
INT StartImageFlag = FALSE;
INT StartRingMovementFlag = 0;
INT ISLEFTHALFRINGCOMPLETE = 0;
INT ISRIGHTHALFRINGCOMPLETE = 0;

// Saturn Ring  global variables
INT ROUNDS = 0;
DOUBLE X_C;
DOUBLE Y_C = 2;
DOUBLE X_O;
DOUBLE Y_O;
INT RING_1_SIZE = 45;
INT RING_2_SIZE = 60;
INT RING_1_WIDTH = 9;
INT RING_2_WIDTH = 13;


DOUBLE PIXEL_X_P_1, PIXEL_X_N_1, PIXEL_X_P_2, PIXEL_X_N_2;
DOUBLE PIXEL_Y_1, PIXEL_Y_2;

DOUBLE Prev_X_N_1;
DOUBLE Prev_X_P_1;
DOUBLE Prev_Y_1;

DOUBLE Prev_X_N_2;
DOUBLE Prev_X_P_2;
DOUBLE Prev_Y_2;    

DOUBLE PrevToPrev_X_N_1;
DOUBLE PrevToPrev_X_P_1;
DOUBLE PrevToPrev_Y_1;

DOUBLE PrevToPrev_X_N_2;
DOUBLE PrevToPrev_X_P_2;
DOUBLE PrevToPrev_Y_2;

//Wave Variables
INT WAVE_INCREMENTER = 0;
INT ISINCREMENTACHIEVED = 0;
BOOL CheckIsHeightDecrement = 0;
INT HeightPixel = 70;
DOUBLE ValueMultiplierForX = 1;

//Saturn Circle Var
FLOAT IncreaseBlack = 0.1;

//Scene 2 variables
INT SceneTwoAstroMediCompX = 0;
INT SceneTwoAstroMediCompY = 0;

INT ChangeWordCarriedBySirFlag = 0;
BOOL AreWordArrangedFlag = 0;

BOOL IsTextMoving = 0;
INT MakeWordFly = 0;

INT WalkingPositionCaseNumber = 0;

//Sir Body Variable
INT SirReferX = 100, SirReferY = 0;
INT Leg1ReferX1 = -30, Leg2ReferX2 = 30;
BOOL IsLeg1ReachEnd = 0, IsLeg2ReachEnd = 0;

//Credit Window Variables
BOOL StartCreditWindow = 0;


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
    static TCHAR szAppName[] = TEXT("Birthday Application"); 
    static TCHAR szWindowClassName[] = TEXT("Happy Birthday Sir"); 

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    HBRUSH hBrush = NULL; 
    HCURSOR hCursor = NULL; 
    HICON hIcon = NULL; 
    HICON hIconSm = NULL; 
    HWND hWnd = NULL; 

    ATOM bClassRegistrationStatus = 0; 

    WNDCLASSEX wnd; 
    MSG msg; 

    ZeroMemory((void*)&wnd, sizeof(WNDCLASSEX)); 
    ZeroMemory((void*)&msg, sizeof(MSG)); 

    hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH); 
    EXIT_IF(hBrush == NULL, "Error in loading brush", "GetStockObject"); 

    hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW); 
    EXIT_IF(hCursor == NULL, "Error in Loading cursor", "LoadCursor"); 

    hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); 
    EXIT_IF(hIcon == NULL, "Error in loading icon", "LoadIcon"); 

    hIconSm = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); 
    EXIT_IF(hIconSm == NULL, "Error in loading icon", "LoadIcon"); 

    wnd.cbSize = sizeof(WNDCLASSEX); 
    wnd.cbClsExtra = 0; 
    wnd.cbWndExtra = 0; 
    wnd.hbrBackground = hBrush; 
    wnd.hCursor = hCursor; 
    wnd.hIcon = hIcon; 
    wnd.hInstance = hInstance; 
    wnd.hIconSm = hIconSm; 
    wnd.lpfnWndProc = WndProc; 
    wnd.lpszClassName = szWindowClassName; 
    wnd.lpszMenuName = NULL; 
    wnd.style = CS_HREDRAW | CS_VREDRAW; 

    bClassRegistrationStatus = RegisterClassEx(&wnd); 
    EXIT_IF(bClassRegistrationStatus == 0, "Failed to register a window class",
            "RegisterClassEx"); 
    
    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szWindowClassName, szAppName, WS_OVERLAPPEDWINDOW, 
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                            (HWND)NULL, (HMENU)NULL, hInstance, NULL); 
    EXIT_IF(hWnd == NULL, "Failed to create a window in memory", "CreateWindowEx"); 

    ShowWindow(hWnd, nShowCmd); 
    UpdateWindow(hWnd); 

    ghWnd = hWnd; 

    while(GetMessage(&msg, (HWND)NULL, 0U, 0U))
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (msg.wParam); 
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR BUFFER[256];
    RECT ImageSceneRect;
    GetClientRect(hWnd, &ImageSceneRect);

    switch(uMsg)
    {
        case WM_SIZE:
            cysize = HIWORD(lParam);
            cxsize = LOWORD(lParam);
            X_O = cxsize/2;
            Y_O = cysize/2;

            SirReferY = cysize - 400;
            break;

        case WM_TIMER:
            switch(wParam)
            {
                case RIGHTHALRINGTIMER:
                    //KillTimer(hWnd, RIGHTHALRINGTIMER);
                    if(StartImageFlag == TRUE)
                    {
                        KillTimer(hWnd, RIGHTHALRINGTIMER);
                        InvalidateRect(hWnd, NULL, TRUE);
                        break;
                    }
                    if(ISRIGHTHALFRINGCOMPLETE == 0)
                    {
                        if(Y_C < 1.5 && Y_C > -1.5)
                        {
                            Y_C -= 0.1;
                        }
                        else
                            Y_C -= 0.01;
                    }
                    else if(ISLEFTHALFRINGCOMPLETE == 0)
                    {
                        if(Y_C < 1.5 && Y_C > -1.5)
                            Y_C += 0.1;
                        else
                            Y_C += 0.01;
                    }

                    if(IncreaseBlack >= 1)
                    {
                        IncreaseBlack = 0.1;
                    }

                    IncreaseBlack += 0.01;
                    InvalidateRect(hWnd, NULL, TRUE);
                    SetTimer(hWnd, RIGHTHALRINGTIMER, 225, NULL);
                    break;
                
                case WALKING_SIR_TIMER:
                    //KillTimer(hWnd, WALKING_SIR_TIMER);
                    if(StartAstroMediCompSceneFlag == TRUE)
                    {
                        KillTimer(hWnd, WALKING_SIR_TIMER);
                        InvalidateRect(hWnd, NULL, TRUE);
                        break;
                    }

                    WalkingPositionCaseNumber = (WalkingPositionCaseNumber + 1) % 4;

                    if(MakeWordFly > cysize - 600)
                    {
                        if(ChangeWordCarriedBySirFlag == 3)
                        {
                            KillTimer(hWnd, WALKING_SIR_TIMER);
                            AreWordArrangedFlag = 1;
                            InvalidateRect(hWnd, NULL, TRUE);
                            break;
                        }

                        ChangeWordCarriedBySirFlag++;
                        SirReferX = 100;
                        MakeWordFly = 0;
                        Leg1ReferX1 = -30;
                        Leg2ReferX2 = 30;
                    }

                    else
                    {
                        if(SirReferX > cxsize/2 - 300)
                        {
                            MakeWordFly += 12;
                        }
                        SirReferX += 10;
                    }

                    InvalidateRect(hWnd, NULL, TRUE);
                    SetTimer(hWnd, NULL, 50, NULL);
                    break;

            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            if(StartAstroMediCompSceneFlag == TRUE)
                Scene1AstroMediComp(hdc, hWnd, ps);
            
            else if(StartImageFlag == TRUE)
                Scene2(hdc, hWnd, ps);

            else if(StartCreditWindow == TRUE)
                CreditWindow(hdc, hWnd, ps);

            else
                InstructionWindow(hdc, hWnd, ps);

            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(EXIT_SUCCESS);
            break;
        
        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_F: 
                    if(gbFullScreen == FALSE)
                    {
                        ToggleFullScreen(); 
                        gbFullScreen = TRUE; 
                    }
                    else 
                    {
                        ToggleFullScreen(); 
                        gbFullScreen = FALSE; 
                    }
                    break;
                
                case VK_C:
                    if(StartCreditWindow == FALSE)
                    {
                        PlaySound(NULL, 0, 0);
                        PlaySound((LPCSTR)"Credit_Music.wav", NULL, SND_LOOP | SND_ASYNC);
                        StartCreditWindow = TRUE;
                        StartAstroMediCompSceneFlag = FALSE;
                        StartImageFlag = FALSE;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    break;
                
                case VK_A:
                    if(StartAstroMediCompSceneFlag == FALSE)
                    {
                        PlaySound(NULL, 0, 0);
                        PlaySound((LPCSTR)"Dr. Vijay D. Gokhale Sir's Office Setup.wav", 
                                    NULL, SND_LOOP | SND_ASYNC);

                        StartAstroMediCompSceneFlag = TRUE;
                        StartImageFlag = FALSE;
                        Prev_X_N_1 = X_O + 0;
                        Prev_X_P_1 = X_O + 0;
                        Prev_Y_1 = Y_O + int(2 * (RING_1_SIZE - 15));

                        Prev_X_N_2 = X_O + 0;
                        Prev_X_P_2 = X_O + 0;
                        Prev_Y_2 = Y_O + int(2 * (RING_2_SIZE - 15));    

                        PrevToPrev_X_N_1 = Prev_X_N_1;
                        PrevToPrev_X_P_1 = Prev_X_P_1;
                        PrevToPrev_Y_1 = Prev_Y_1;

                        PrevToPrev_X_N_2 = Prev_X_N_2;
                        PrevToPrev_X_P_2 = Prev_X_P_2;
                        PrevToPrev_Y_2 = Prev_Y_2;

                        SetTimer(hWnd, RIGHTHALRINGTIMER, 200, NULL);
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    break;

                case VK_S:
                    if(StartImageFlag == FALSE)
                    {
                        ImageSceneRect.bottom -= 80;
                        StartAstroMediCompSceneFlag = FALSE;
                        StartImageFlag = TRUE;

                        SirReferX = 100;
                        ChangeWordCarriedBySirFlag = 0;

                        PlaySound(NULL, 0, 0);
                        PlaySound((LPCSTR)"Poem.wav", NULL, SND_LOOP | SND_ASYNC);

                        SetTimer(hWnd, WALKING_SIR_TIMER, 200, NULL);
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    break;
                
                case VK_ESCAPE:
                    PostQuitMessage(EXIT_SUCCESS);
                    break;
            }
            break;
    }
    
    return (DefWindowProc(hWnd, uMsg, wParam, lParam)); 
}

void ToggleFullScreen(void)
{
    MONITORINFO mi; 
    HMONITOR hMonitor; 
    BOOL bVar; 

    if(gbFullScreen == FALSE)
    {
        dwWindowStyle = GetWindowLong(ghWnd, GWL_STYLE); 
        if(dwWindowStyle & WS_OVERLAPPEDWINDOW)
        { 
            gpPreviousWindowPlacement.length = sizeof(WINDOWPLACEMENT); 
            bVar = GetWindowPlacement(ghWnd, &gpPreviousWindowPlacement); 
            mi.cbSize = sizeof(MONITORINFO); 
            hMonitor = MonitorFromWindow(ghWnd, MONITORINFOF_PRIMARY); 
            GetMonitorInfo(hMonitor, &mi); 
            if(bVar && hMonitor) 
            {
                SetWindowLong(ghWnd, GWL_STYLE, dwWindowStyle & ~WS_OVERLAPPEDWINDOW); 
                SetWindowPos(ghWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, 
                                mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, 
                                SWP_NOZORDER | SWP_FRAMECHANGED); 
            }
        }
        ShowCursor(FALSE); 
    }
    else
    {
        SetWindowLong(ghWnd, GWL_STYLE, dwWindowStyle & WS_OVERLAPPEDWINDOW); 
        gpPreviousWindowPlacement.length = sizeof(WINDOWPLACEMENT); 
        SetWindowPlacement(ghWnd, &gpPreviousWindowPlacement); 
        SetWindowPos(ghWnd, HWND_TOP, 0, 0, 0, 0, 
                        SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED); 
        ShowCursor(TRUE); 
    }
}

void CreditWindow(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    INT CColor = 0;

    while(CColor <= 255)
    {
        MyDrawText2(hdc, hWnd, ps, L"\"Knowledge Is Interrelated\"", 
                    cxsize/2 - 400, cysize/2 - 70, CColor, CColor, CColor, 50);
        MyDrawText2(hdc, hWnd, ps, L"        ~ Dr. Vijay D. Gokhale", 
                    cxsize/2 - 200, cysize/2 + 20, CColor, CColor, CColor, 30);
        CColor += 1;
        Sleep(10);
    }

    Sleep(5000);

    RECT rc;

    GetClientRect(hWnd, &rc);

    SetDCBrushColor(hdc, RGB(0, 0, 0));
    HBRUSH hBrush = (HBRUSH)GetStockObject(DC_BRUSH);

    FillRect(hdc, &rc, hBrush);

    CColor = 0;
    while(CColor <= 255)
    {
        MyDrawText2(hdc, hWnd, ps, L"HAPPY BIRTHDAY SIR", 
                    cxsize/2 - 400, cysize/2, CColor, CColor, CColor, 60);
        CColor++;
        Sleep(7);
    }

    Sleep(5000);

    SetDCBrushColor(hdc, RGB(0, 0, 0));
    hBrush = (HBRUSH)GetStockObject(DC_BRUSH);

    FillRect(hdc, &rc, hBrush);

    CColor = 0;

    while(CColor <= 255)
    {
        MyDrawText2(hdc, hWnd, ps, L"Technology Used", 
                    cxsize/2 - 150, cysize/2 - 200, CColor, CColor, CColor, 40);

        MyDrawText2(hdc, hWnd, ps, L"Win32SDK", 
                    cxsize/2 - 30, cysize/2 - 120, CColor, CColor, CColor, 30);

        MyDrawText2(hdc, hWnd, ps, L"Poem: Tu Khud Ki Khoj Me Nikal", 
                    cxsize/2 - 220, cysize/2, CColor, CColor, CColor, 30);

        MyDrawText2(hdc, hWnd, ps, L"Recited by: Shri Amitabh Bachchan", 
                    cxsize/2 - 235, cysize/2 + 50, CColor, CColor, CColor, 30);

        CColor += 1;
        Sleep(5);
    }

    Sleep(5000);

    SetDCBrushColor(hdc, RGB(0, 0, 0));
    hBrush = (HBRUSH)GetStockObject(DC_BRUSH);

    FillRect(hdc, &rc, hBrush);

    CColor = 0;

    while (CColor <= 255)
    {
        MyDrawText2(hdc, hWnd, ps, L"Inspiration by Dr. Vijay D. Gokhale Sir", 
                    cxsize/2 - 415, cysize/2 - 250, CColor, CColor, CColor, 40);

        MyDrawText2(hdc, hWnd, ps, L"Ignited by Yogeshwar Shukla Sir", 
                    cxsize/2 - 260, cysize/2 - 100, CColor, CColor, CColor, 30);

        MyDrawText2(hdc, hWnd, ps, L"Made by Yash Thakare", 
                    cxsize/2 - 200, cysize/2, CColor, CColor, CColor, 30);

        MyDrawText2(hdc, hWnd, ps, L"    Guidance by   \n  Prateek Thakare\nAishwarya Kendle", 
                    cxsize/2 - 150, cysize/2 + 100, CColor, CColor, CColor, 30);   
        CColor += 1;
        Sleep(7);
    }

    Sleep(10000);

    StartCreditWindow = FALSE;

    DeleteObject(hBrush);
}

void InstructionWindow(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    DWORD pen_style = PS_SOLID;
    COLORREF col = RGB(255, 255, 255);
    HPEN hPen2 = CreatePen(pen_style, 8, col);
    HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    POINT p[] = {{cxsize/2 - 100, cysize/2 - 250}, 
                {cxsize/2 + 200, cysize/2 - 250},
                {cxsize/2 + 200, cysize/2 + 150}, 
                {cxsize/2 - 100, cysize/2 + 150},
                {cxsize/2 - 100, cysize/2 - 250}};

    Polyline(hdc, p, 5);

    MyDrawText2(hdc, hWnd, ps, L"Press", cxsize/2 - 25, 
                cysize/2 - 200, 255, 255, 255, 35);

    MyDrawText2(hdc, hWnd, ps, L"F : Full Screen", 
                cxsize/2 - 70, cysize/2 - 120, 255, 255, 255, 30);

    MyDrawText2(hdc, hWnd, ps, L"A : Scene 1", 
                cxsize/2 - 50, cysize/2 - 70, 255, 255, 255, 30);

    MyDrawText2(hdc, hWnd, ps, L"S : Scene 2", 
                cxsize/2 - 50, cysize/2 - 20, 255, 255, 255, 30);

    MyDrawText2(hdc, hWnd, ps, L"C : Credits", 
                cxsize/2 - 50, cysize/2 + 30, 255, 255, 255, 30);
}

void Scene2(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    RECT rc;

    GetClientRect(hWnd, &rc);

    SetDCBrushColor(hdc, RGB(255, 255, 255));
    HBRUSH hBrush = (HBRUSH)GetStockObject(DC_BRUSH);

    FillRect(hdc, &rc, hBrush);

    //TextAstroMediComp(hdc, hWnd, ps, SceneTwoAstroMediCompX, cysize/2, 0, 0, 0);

    switch(ChangeWordCarriedBySirFlag)
    {
        case 0:
            MyDrawText(hdc, hWnd, ps, L"ASTROLOGY", 
                        SirReferX - 200, SirReferY - 50 - MakeWordFly, 
                        132, 132, 132);
            MakeSir(hdc, hWnd, ps);
            break;
        
        case 1:
            MyDrawText(hdc, hWnd, ps, L"ASTROLOGY", 10, 200, 132, 132, 132);
            MyDrawText(hdc, hWnd, ps, L"MEDICAL", 
                        SirReferX - 150, SirReferY - 50 - MakeWordFly, 
                        51, 51, 51);
            MakeSir(hdc, hWnd, ps);
            break;

        case 2:
            MyDrawText(hdc, hWnd, ps, L"ASTROLOGY", 10, 200, 132, 132, 132);
            MyDrawText(hdc, hWnd, ps, L"MEDICAL", cxsize - 300, 350, 51, 51, 51);
            MyDrawText(hdc, hWnd, ps, L"COMPUTER", 
                        SirReferX - 170, SirReferY - 50 - MakeWordFly, 
                        222, 222, 222);
            MakeSir(hdc, hWnd, ps);
            break;
        
        case 3:
            IMAGE(hdc, hWnd, ps, cxsize/2, cysize/2);
            return;
            break;

    }
    MakeAgneepath(hdc, hWnd, ps);

    DeleteObject(hBrush);
}

void MoveText(HDC hdc, HWND hWnd, PAINTSTRUCT ps, INT StartX,
             INT StartY, INT EndX, INT EndY, WCHAR* String, 
             INT R, INT G, INT B, BOOL IsLToR)
{
    INT LoopX, LoopY;

    for(LoopY = 0; LoopY < EndY; LoopY += 5)
        MyDrawText(hdc, hWnd, ps, String, StartX, LoopY, R, G, B);

    if(IsLToR == 1)
    {
        for(LoopX = StartX; LoopX < EndX; LoopX++)
            MyDrawText(hdc, hWnd, ps, String, LoopX, EndY, R, G, B);
    }
    else
    {
        for(LoopX = StartX; LoopX > EndX; LoopX--)
            MyDrawText(hdc, hWnd, ps, String, LoopX, EndY, R, G, B);
    }
}

void MyDrawText(HDC hdc, HWND hWnd, PAINTSTRUCT ps, 
                WCHAR* STRING,INT X, INT Y, INT R, INT G, INT B)
{
    Graphics graphics(hdc);

   Font myFont(L"Times New Roman", 50);
   PointF origin((float)X, (float)Y);
   SolidBrush myBrush(Color(255, R, G, B));

   graphics.DrawString(STRING, -1, &myFont, origin, &myBrush);
}

void MyDrawText2(HDC hdc, HWND hWnd, PAINTSTRUCT ps, 
                WCHAR* STRING,INT X, INT Y, INT R, INT G, INT B, INT SIZE)
{
    Graphics graphics(hdc);

   Font myFont(L"Times New Roman", SIZE);
   PointF origin((float)X, (float)Y);
   SolidBrush myBrush(Color(255, R, G, B));

   graphics.DrawString(STRING, -1, &myFont, origin, &myBrush);
}


void MakeSir(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    switch(WalkingPositionCaseNumber)
    {
        case 0:
            MakeSirframe0(hdc, hWnd, ps);
            break;
        case 1:
            MakeSirframe1(hdc, hWnd, ps);
            break;
        case 2:
            MakeSirframe2(hdc, hWnd, ps);
            break;
        case 3:
            MakeSirframe3(hdc, hWnd, ps);
            break;
    }

}

void MakeSirframe0(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    DWORD pen_style = PS_SOLID;
    COLORREF col = RGB(0, 0, 0);
    HPEN hPen2 = CreatePen(pen_style, 5, col);
    HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    INT LegX = SirReferX; 
    INT LegY = SirReferY + 200;

    Ellipse(hdc, SirReferX - 50 + 15, SirReferY + 30, 
                SirReferX + 50 - 15, SirReferY + 100);

    Ellipse(hdc, SirReferX + 10, SirReferY + 50, 
                SirReferX + 20, SirReferY + 60);

    POINT Mid[] = {{SirReferX, SirReferY + 100}, 
                    {SirReferX, SirReferY +  200}};
    Polyline(hdc, Mid, 2);

    POINT hand1[] = {{SirReferX, SirReferY + 110}, 
                    {SirReferX + 75, SirReferY + 60}, 
                    {SirReferX + 75, SirReferY + 10}};
    Polyline(hdc, hand1, 3);

    POINT hand2[] = {{SirReferX, SirReferY + 110}, 
                    {SirReferX - 75, SirReferY + 60}, 
                    {SirReferX - 75, SirReferY + 10}};
    Polyline(hdc, hand2, 3);

    //Back Leg
    POINT Leg1[] = {{LegX, LegY}, {LegX - 10, LegY + 50}, 
                                    {LegX - 30, LegY + 80}};
    Polyline(hdc, Leg1, 3);

    //Front Leg
    POINT Leg2[] = {{LegX, LegY}, {LegX + 30, LegY + 50}, 
                                    {LegX + 28, LegY + 80}};
    Polyline(hdc, Leg2, 3);

    DeleteObject(hPen2);
    DeleteObject(holdPen2);

}

void MakeSirframe1(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    DWORD pen_style = PS_SOLID;
    COLORREF col = RGB(0, 0, 0);
    HPEN hPen2 = CreatePen(pen_style, 5, col);
    HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    INT LegX = SirReferX; 
    INT LegY = SirReferY + 200;

    Ellipse(hdc, SirReferX - 50 + 15, SirReferY + 30, 
                SirReferX + 50 - 15, SirReferY + 100);

    Ellipse(hdc, SirReferX + 10, SirReferY + 50, 
                SirReferX + 20, SirReferY + 60);

    POINT Mid[] = {{SirReferX, SirReferY + 100}, 
                    {SirReferX, SirReferY +  200}};
    Polyline(hdc, Mid, 2);

    POINT hand1[] = {{SirReferX, SirReferY + 110}, 
                    {SirReferX + 75, SirReferY + 60}, 
                    {SirReferX + 75, SirReferY + 10}};
    Polyline(hdc, hand1, 3);

    POINT hand2[] = {{SirReferX, SirReferY + 110}, 
                    {SirReferX - 75, SirReferY + 60}, 
                    {SirReferX - 75, SirReferY + 10}};
    Polyline(hdc, hand2, 3);

   //Back Leg
    POINT Leg1[] = {{LegX, LegY}, {LegX + 2, LegY + 35}, 
                                    {LegX - 35, LegY + 60}};
    Polyline(hdc, Leg1, 3);

    //Front Leg
    POINT Leg2[] = {{LegX, LegY}, {LegX + 10, LegY + 30}, 
                                    {LegX + 10, LegY + 80}};
    Polyline(hdc, Leg2, 3);

    DeleteObject(hPen2);
    DeleteObject(holdPen2);

}

void MakeSirframe2(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    DWORD pen_style = PS_SOLID;
    COLORREF col = RGB(0, 0, 0);
    HPEN hPen2 = CreatePen(pen_style, 5, col);
    HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    INT LegX = SirReferX; 
    INT LegY = SirReferY + 200;

    Ellipse(hdc, SirReferX - 50 + 15, SirReferY + 30, 
            SirReferX + 50 - 15, SirReferY + 100);

    Ellipse(hdc, SirReferX + 10, SirReferY + 50, 
            SirReferX + 20, SirReferY + 60);

    POINT Mid[] = {{SirReferX, SirReferY + 100}, 
                    {SirReferX, SirReferY +  200}};
    Polyline(hdc, Mid, 2);

    POINT hand1[] = {{SirReferX, SirReferY + 110}, 
                    {SirReferX + 75, SirReferY + 60}, 
                    {SirReferX + 75, SirReferY + 10}};
    Polyline(hdc, hand1, 3);

    POINT hand2[] = {{SirReferX, SirReferY + 110}, 
                    {SirReferX - 75, SirReferY + 60}, 
                    {SirReferX - 75, SirReferY + 10}};
    Polyline(hdc, hand2, 3);

    //Back Leg
    POINT Leg1[] = {{LegX, LegY}, {LegX + 8, LegY + 35}, 
                                    {LegX - 25, LegY + 70}};
    Polyline(hdc, Leg1, 3);

    //Front Leg
    POINT Leg2[] = {{LegX, LegY}, {LegX + 2, LegY + 30}, 
                                    {LegX - 5, LegY + 80}};
    Polyline(hdc, Leg2, 3);

    DeleteObject(hPen2);
    DeleteObject(holdPen2);

}

void MakeSirframe3(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    DWORD pen_style = PS_SOLID;
    COLORREF col = RGB(0, 0, 0);
    HPEN hPen2 = CreatePen(pen_style, 5, col);
    HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    INT LegX = SirReferX; 
    INT LegY = SirReferY + 200;

    Ellipse(hdc, SirReferX - 50 + 15, SirReferY + 30,
             SirReferX + 50 - 15, SirReferY + 100);

    Ellipse(hdc, SirReferX + 10, SirReferY + 50, 
            SirReferX + 20, SirReferY + 60);

    POINT Mid[] = {{SirReferX, SirReferY + 100}, 
                    {SirReferX, SirReferY +  200}};
    Polyline(hdc, Mid, 2);

    POINT hand1[] = {{SirReferX, SirReferY + 110}, 
                    {SirReferX + 75, SirReferY + 60}, 
                    {SirReferX + 75, SirReferY + 10}};
    Polyline(hdc, hand1, 3);

    POINT hand2[] = {{SirReferX, SirReferY + 110}, 
                    {SirReferX - 75, SirReferY + 60}, 
                    {SirReferX - 75, SirReferY + 10}};
    Polyline(hdc, hand2, 3);

   //Back Leg
    POINT Leg1[] = {{LegX, LegY}, {LegX + 30, LegY + 35}, 
                                    {LegX + 28, LegY + 80}};
    Polyline(hdc, Leg1, 3);

    //Front Leg
    POINT Leg2[] = {{LegX, LegY}, {LegX - 10, LegY + 50}, 
                                    {LegX - 30, LegY + 80}};
    Polyline(hdc, Leg2, 3);

    DeleteObject(hPen2);
    DeleteObject(holdPen2);

}

void MakeAgneepath(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    INT i, j;
    INT Y;
    
    DWORD pen_style = PS_SOLID;
    COLORREF col = RGB(255, 0, 0);
    HPEN hPen2 = CreatePen(pen_style, 30, col);
    HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    POINT pointp[] = {{0, cysize - 80}, {cxsize, cysize - 80}};

    Polyline(hdc, pointp, 2);

    for(i = 0; i < cxsize; i++)
    {
        for(j = 0; j < 2; j++)
        {
            Y = RandRange(0, 30);

            SetPixel(hdc, i, cysize - Y - 80, RGB(255, 0, 0));

            ActivateSidePixels(hdc, i, cysize - Y - 80, 255, 0, 0);
        }
    }

    DeleteObject(hPen2);
    DeleteObject(holdPen2);
}

INT RandRange(INT START, INT END)
{
    INT NUM;
    
    NUM = rand();

    return(START + (NUM % END));
}

void IMAGE(HDC hdc, HWND hWnd, PAINTSTRUCT ps, INT R_X, INT R_Y)
{
    RECT rect;
    GetClientRect(hWnd, &rect);

    SetDCBrushColor(hdc, RGB(255, 255, 255));
    HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);

    FillRect(hdc, &rect, brush);

    TCHAR BUFFER[256];

    Graphics graphics(hdc);

    INT ReferX, ReferY;

    INT row, col;

    INT T_R_1 = 132, T_G_1  = 132, T_B_1 = 132;

    INT T_R_2 = 51, T_G_2 = 51, T_B_2 = 51;

    INT T_R_3 = 222, T_G_3 = 222, T_B_3 = 222;

    Bitmap SirImageBitMap(L"imageofgokhalesircroped.jpg");

    ReferX = R_X - (SirImageBitMap.GetWidth())/2;
    ReferY = R_Y - (SirImageBitMap.GetHeight())/2;


    Color pixelColor;
    COLORREF colorRef;

    while(  (T_R_1 < 255 && T_G_1 < 255 && T_B_1 < 255) ||
            (T_R_2 < 255 && T_G_2 < 255 && T_B_2 < 255) ||
            (T_R_3 < 255 && T_G_3 < 255 && T_B_3 < 255))
    {   
        MyDrawText(hdc, hWnd, ps, L"ASTROLOGY", 
                    10, 200, T_R_1, T_G_1, T_B_1);

        MyDrawText(hdc, hWnd, ps, L"MEDICAL",
                    cxsize - 200, 350, 
                    T_R_2, T_G_2, T_B_2);
        MyDrawText(hdc, hWnd, ps, L"COMPUTER", 
                    10, ReferY + SirImageBitMap.GetHeight() - 200, 
                    T_R_3, T_G_3, T_B_3);

        if((T_R_1 < 255 && T_G_1 < 255 && T_B_1 < 255))
        {
            T_R_1++;
            T_G_1++;
            T_B_1++;
        }
        if((T_R_2 < 255 && T_G_2 < 255 && T_B_2 < 255))
        {
            T_R_2++;
            T_G_2++;
            T_B_2++;
        }
        if((T_R_3 < 255 && T_G_3 < 255 && T_B_3 < 255))
        {
            T_R_3++;
            T_G_3++;
            T_B_3++;
        }
        Sleep(5);
    }

    MyDrawText(hdc, hWnd, ps, L"ASTRO", 10, 200, 132, 132, 132);

    MyDrawText(hdc, hWnd, ps, L"MEDI", cxsize - 200, 350, 51, 51, 51);

    MyDrawText(hdc, hWnd, ps, L"COMP", 10,
                 ReferY + SirImageBitMap.GetHeight() - 200, 
                 222, 222, 222);

    Sleep(300);

    wave(hdc, hWnd, 250, 200, cxsize/2 - 100, 132, 132, 132, 0, 0.7);

    for (UINT row = 0; row < SirImageBitMap.GetWidth(); row++)
    {
        for (UINT col = 0; col < SirImageBitMap.GetHeight(); col++)
        {
            SirImageBitMap.GetPixel(row, col, &pixelColor);
            colorRef = pixelColor.ToCOLORREF();
            if((GetRValue(colorRef) > 125 && GetRValue(colorRef) < 140) && 
                (GetGValue(colorRef) > 125 && GetGValue(colorRef) < 140) && 
                (GetBValue(colorRef) > 125 && GetBValue(colorRef) < 140))
                SetPixel(hdc, row + ReferX, col + ReferY, colorRef);
        }
        Sleep(8);
    }

    wave(hdc, hWnd, cxsize - 230, 350, cxsize/2 - 100, 51, 51, 51, 1, 0.5);

    for (UINT row = 0; row < SirImageBitMap.GetWidth(); row++)
    {
        for (UINT col = 0; col < SirImageBitMap.GetHeight(); col++)
        {
            SirImageBitMap.GetPixel(row, col, &pixelColor);
            colorRef = pixelColor.ToCOLORREF();
            if((GetRValue(colorRef) > 45 && GetRValue(colorRef) < 75) && 
                (GetGValue(colorRef) > 45 && GetGValue(colorRef) < 75) && 
                (GetBValue(colorRef) > 45 && GetBValue(colorRef) < 75))
                SetPixel(hdc, row + ReferX, col + ReferY, colorRef);
        }
        Sleep(8);
    }

   wave(hdc, hWnd, 250, ReferY + SirImageBitMap.GetHeight() - 200, 
            cxsize/2 - 100, 222, 222, 222, 0, 0.4);
    for (UINT row = 0; row < SirImageBitMap.GetWidth(); row++)
    {
        for (UINT col = 0; col < SirImageBitMap.GetHeight(); col++)
        {
            SirImageBitMap.GetPixel(row, col, &pixelColor);

            colorRef = pixelColor.ToCOLORREF();

        if((GetRValue(colorRef) > 190 && GetRValue(colorRef) < 230) && 
                (GetGValue(colorRef) > 190 && GetGValue(colorRef) < 230) && 
                (GetBValue(colorRef) > 190 && GetBValue(colorRef) < 230))
                SetPixel(hdc, row + ReferX, col + ReferY, colorRef);

        }
        Sleep(8);
    }

    Sleep(1000);

    for (UINT row = 0; row < SirImageBitMap.GetWidth(); row++)
    {
        for (UINT col = 0; col < SirImageBitMap.GetHeight(); col++)
        {
            SirImageBitMap.GetPixel(row, col, &pixelColor);
            colorRef = pixelColor.ToCOLORREF();
            SetPixel(hdc, row + ReferX, col + ReferY, colorRef);
        }

        Sleep(8);
    }

   TextAstroMediComp(hdc, hWnd, ps, cxsize/2 - 250, cysize - 100, 0, 0, 0);

   Sleep(15000);

   StartImageFlag = FALSE;
}

/*
    Black : 170 x 55
    Grey  : 127 x 127
    Light Grey : 170 x 123


    132 : all grey
    51 all; black
    222 all; light grey
*/

void wave(HDC hdc, HWND hWnd, INT O_X, INT O_Y, INT WIDTH, 
            INT R, INT G, INT B, BOOL IsRToLFlag, FLOAT SinWaveLenght)
{
    TCHAR BUFFER[256];
    INT HeightPixel = 70;
    BOOL CheckIsHeightDecrement = 0;
    INT i;
    double X_COORDINATE, Y_COORDINATE;
    if(IsRToLFlag == 1)
    {
        for(X_COORDINATE = 0; (X_COORDINATE > -(WIDTH / 50)); X_COORDINATE -= 0.01)
        {
            Y_COORDINATE = (sin(SinWaveLenght * X_COORDINATE) * 30);

            SetPixel(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                            (int)(O_Y + Y_COORDINATE), RGB(R, G, B));

            ActivateSidePixels(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                                    (int)(O_Y + Y_COORDINATE), R, G, B);

            for(i = HeightPixel; i > 0; i--)
            {
                SetPixel(hdc, (int)(O_X + (X_COORDINATE * 30)),
                         (int)(O_Y + Y_COORDINATE + i), RGB(R, G, B));

                SetPixel(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                        (int)(O_Y + Y_COORDINATE - i), RGB(R, G, B));

                ActivateSidePixels(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                                    (int)(O_Y + Y_COORDINATE + i), R, G, B);

                ActivateSidePixels(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                                    (int)(O_Y + Y_COORDINATE - i), R, G, B);

            }

            if(CheckIsHeightDecrement > 22)
            {
                HeightPixel--;
                CheckIsHeightDecrement = 0;
            }

            CheckIsHeightDecrement++;
        }
    }

    else
    {
        for(X_COORDINATE = 0; (X_COORDINATE < WIDTH / 50); X_COORDINATE += 0.01)
        {
            Y_COORDINATE = (sin(SinWaveLenght * X_COORDINATE) * 30);

            SetPixel(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                            (int)(O_Y + Y_COORDINATE), RGB(R, G, B));

            ActivateSidePixels(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                                    (int)(O_Y + Y_COORDINATE), R, G, B);

            for(i = HeightPixel; i > 0; i--)
            {
                SetPixel(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                                (int)(O_Y + Y_COORDINATE + i), RGB(R, G, B));

                SetPixel(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                                (int)(O_Y + Y_COORDINATE - i), RGB(R, G, B));

                ActivateSidePixels(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                                        (int)(O_Y + Y_COORDINATE + i), R, G, B);

                ActivateSidePixels(hdc, (int)(O_X + (X_COORDINATE * 30)), 
                                        (int)(O_Y + Y_COORDINATE - i), R, G, B);

            }

            if(CheckIsHeightDecrement > 22)
            {
                HeightPixel--;
                CheckIsHeightDecrement = 0;
            }

            CheckIsHeightDecrement++;
        }
        
    }
}

void ActivateSidePixels(HDC hdc, INT X, INT Y, INT R, INT G, INT B)
{
    //Active Side Pixels : It will activate all 8 pixel around a pixel.

    SetPixel(hdc, X - 1, Y + 1, RGB(R, G, B));
    SetPixel(hdc, X + 1, Y - 1, RGB(R, G, B));
    SetPixel(hdc, X - 1, Y - 1, RGB(R, G, B));
    SetPixel(hdc, X + 1, Y + 1, RGB(R, G, B));
    SetPixel(hdc, X - 1, Y    , RGB(R, G, B));
    SetPixel(hdc, X    , Y + 1, RGB(R, G, B));
    SetPixel(hdc, X + 1, Y    , RGB(R, G, B));
    SetPixel(hdc, X    , Y - 1, RGB(R, G, B));

}

void Scene1AstroMediComp(HDC hdc, HWND hWnd, PAINTSTRUCT ps)
{
    
    TextAstroMediComp(hdc, hWnd, ps, cxsize/2 - 250, 
                        cysize - 100, 255, 255, 255);

    MakeSaturnImage(hdc, hWnd, ps, cxsize/2, cysize/2);

}

void TextAstroMediComp(HDC hdc, HWND hWnd, PAINTSTRUCT ps, 
                        INT X, INT Y, INT R, INT G, INT B)
{

    Graphics graphics(hdc);

    WCHAR string[] = L"ASTROMEDICOMP";

    Font myFont(L"Times New Roman", 50);
    PointF origin((float)X, (float)Y);
    SolidBrush myBrush(Color(255, R, G, B));

    graphics.DrawString(string, -1, &myFont, origin, &myBrush);

}

void MakeSaturnImage(HDC hdc, HWND hWnd, PAINTSTRUCT ps, INT X_O, INT Y_O)
{
    
    MakeSaturnCircle(hdc, ps, X_O, Y_O);
    MakeSaturnRingsWithCircle(hdc, ps, X_O, Y_O, 45, 9);
    MakeSaturnRingsWithCircle(hdc, ps, X_O, Y_O, 60, 13);
    //MakeRingsMove(hdc, ps, X_O, Y_O, 45, 60, 9, 13);
    Dub_MakeRingsMove(hdc, ps, hWnd);

}

void MakeSaturnRingsWithCircle(HDC hdc, PAINTSTRUCT ps, INT X_O, INT Y_O, 
                                INT RingSize, INT RingWidth)
{
    // Equation for making rings of Saturn:
    // (x^2/36) + (y^2/4) = 1

    DOUBLE X_C;
    DOUBLE Y_C;

    DOUBLE PIXEL_X_P, PIXEL_X_N;
    DOUBLE PIXEL_Y;

    DOUBLE Prev_X_N = X_O + 0;
    DOUBLE Prev_X_P = X_O + 0;
    DOUBLE Prev_Y = Y_O + 100;

    DWORD pen_style = PS_SOLID;
	COLORREF col = RGB(180, 163, 55);
    HPEN hPen2 = CreatePen(pen_style, RingWidth, col);
	HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    POINT Line[2];

    col = RGB(162, 165, 154);
    hPen2 = CreatePen(pen_style, RingWidth, col);
	holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    for(Y_C = 2; Y_C >= -2; Y_C -= 0.01)
    {
        X_C = sqrt(36 - (9 * Y_C * Y_C));

        //-----------Ring 1-----------

        PIXEL_X_P = X_O + (int)(X_C * RingSize);
        PIXEL_Y = Y_O + (int)(Y_C * (RingSize - 15));
        PIXEL_X_N = X_O - (int)(X_C * RingSize);

        if(((int)( X_C * RingSize) < 150 || 
            (int)( X_C * RingSize) < 135) && Y_C <= 0)
        {
            break;
        }

        Line[0] = POINT{(int)PIXEL_X_P, (int)PIXEL_Y};
        Line[1] = POINT{(int)Prev_X_P, (int)PIXEL_Y};

        Polyline(hdc, Line, 2);

        Line[0] = POINT{(int)PIXEL_X_N, (int)PIXEL_Y};
        Line[1] = POINT{(int)Prev_X_N, (int)PIXEL_Y};

        Polyline(hdc, Line, 2);

        Prev_X_N = PIXEL_X_N;
        Prev_X_P = PIXEL_X_P;
        Prev_Y   = PIXEL_Y;
    }
}


void Dub_MakeRingsMove(HDC hdc, PAINTSTRUCT ps, HWND hWnd)
{
    TCHAR BUFFER[265];
    DWORD pen_style = PS_SOLID;
    COLORREF col = RGB(0, 0, 0);
    HPEN hPen2 = CreatePen(pen_style, RING_1_WIDTH, col);
    HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);

    POINT Line[2];

    if(Y_C < -2 && ISRIGHTHALFRINGCOMPLETE == 0)
    {
        ISRIGHTHALFRINGCOMPLETE = 1;
        ISLEFTHALFRINGCOMPLETE = 0;
        Y_C = -2;
    }
    
    if(Y_C > 2 && ISLEFTHALFRINGCOMPLETE == 0)
    {
        ISLEFTHALFRINGCOMPLETE = 1;
        Y_C = 2;
    }

    if(Y_C >= -2 && ISRIGHTHALFRINGCOMPLETE == 0) // For loop condition
    {
        X_C = sqrt(36 - (9 * Y_C * Y_C));

        //-----------Ring 1----------
        PIXEL_X_P_1 = X_O + (int)(X_C * RING_1_SIZE);
        PIXEL_Y_1 = Y_O + (int)(Y_C * (RING_1_SIZE - 15));
        PIXEL_X_N_1 = X_O - (int)(X_C * RING_1_SIZE);

        if(((int)( X_C * RING_1_SIZE) < 150 || 
            (int)( X_C * RING_2_SIZE) < 135) && Y_C <= 0)
        {
            PrevToPrev_X_P_1 = Prev_X_P_1;
            PrevToPrev_Y_1 = Prev_Y_1;

            Prev_X_N_1 = PIXEL_X_N_1;
            Prev_X_P_1 = PIXEL_X_P_1;
            Prev_Y_1   = PIXEL_Y_1;

            PrevToPrev_X_P_2 = Prev_X_P_2;
            PrevToPrev_Y_2 = Prev_Y_2;

            Prev_X_N_2 = PIXEL_X_N_2;
            Prev_X_P_2 = PIXEL_X_P_2;
            Prev_Y_2   = PIXEL_Y_2;
            return;
        }

        col = RGB(162, 165, 154);
        hPen2 = CreatePen(pen_style, RING_1_WIDTH, col);
        holdPen2 = (HPEN)SelectObject(hdc, hPen2);

        Line[0] = POINT{(int)Prev_X_P_1, (int)Prev_Y_1};
        Line[1] = POINT{(int)PrevToPrev_X_P_1, (int)PrevToPrev_Y_1};

        Polyline(hdc, Line, 2);

        DeleteObject(hPen2);
        DeleteObject(holdPen2);

        col = RGB(0, 0, 0);
        hPen2 = CreatePen(pen_style, RING_1_WIDTH, col);
        holdPen2 = (HPEN)SelectObject(hdc, hPen2);

        Line[0] = POINT{(int)PIXEL_X_P_1, (int)PIXEL_Y_1};
        Line[1] = POINT{(int)Prev_X_P_1, (int)PIXEL_Y_1};

        Polyline(hdc, Line, 2);

        DeleteObject(hPen2);
        DeleteObject(holdPen2);

        PrevToPrev_X_P_1 = Prev_X_P_1;
        PrevToPrev_Y_1 = Prev_Y_1;

        Prev_X_N_1 = PIXEL_X_N_1;
        Prev_X_P_1 = PIXEL_X_P_1;
        Prev_Y_1   = PIXEL_Y_1;

        //----------------Ring 2----------
        
        PIXEL_X_P_2 = X_O + (int)(X_C * RING_2_SIZE);
        PIXEL_Y_2 = Y_O + (int)(Y_C * (RING_2_SIZE - 15));
        PIXEL_X_N_2 = X_O - (int)(X_C * RING_2_SIZE);

        col = RGB(162, 165, 154);
        hPen2 = CreatePen(pen_style, RING_2_WIDTH - 1, col);
        holdPen2 = (HPEN)SelectObject(hdc, hPen2);

        Line[0] = POINT{(int)Prev_X_P_2, (int)Prev_Y_2};
        Line[1] = POINT{(int)PrevToPrev_X_P_2, (int)PrevToPrev_Y_2};
        Polyline(hdc, Line, 2);

        DeleteObject(hPen2);
        DeleteObject(holdPen2);

        col = RGB(0, 0, 0);
        hPen2 = CreatePen(pen_style, RING_2_WIDTH - 1, col);
        holdPen2 = (HPEN)SelectObject(hdc, hPen2);

        Line[0] = POINT{(int)PIXEL_X_P_2, (int)PIXEL_Y_2};
        Line[1] = POINT{(int)Prev_X_P_2, (int)PIXEL_Y_2};

        Polyline(hdc, Line, 2);

        DeleteObject(hPen2);
        DeleteObject(holdPen2);

        PrevToPrev_X_P_2 = Prev_X_P_2;
        PrevToPrev_Y_2 = Prev_Y_2;

        Prev_X_N_2 = PIXEL_X_N_2;
        Prev_X_P_2 = PIXEL_X_P_2;
        Prev_Y_2   = PIXEL_Y_2;
    }

    else if(Y_C <= 2 && ISLEFTHALFRINGCOMPLETE == 0)
    {
        
        X_C = sqrt(36 - (9 * Y_C * Y_C));

        //-----------Ring 1----------
        PIXEL_X_P_1 = X_O + (int)(X_C * RING_1_SIZE);
        PIXEL_Y_1 = Y_O + (int)(Y_C * (RING_1_SIZE - 15));
        PIXEL_X_N_1 = X_O - (int)(X_C * RING_1_SIZE);

        if(((int)( X_C * RING_1_SIZE) < 150 || 
            (int)( X_C * RING_2_SIZE) < 135) && Y_C <= 0)
        {
            PrevToPrev_X_N_1 = Prev_X_N_1;
            PrevToPrev_Y_1 = Prev_Y_1;

            Prev_X_N_1 = PIXEL_X_N_1;
            Prev_X_P_1 = PIXEL_X_P_1;
            Prev_Y_1   = PIXEL_Y_1;

            PrevToPrev_X_N_2 = Prev_X_N_2;
            PrevToPrev_Y_2 = Prev_Y_2;

            Prev_X_N_2 = PIXEL_X_N_2;
            Prev_X_P_2 = PIXEL_X_P_2;
            Prev_Y_2   = PIXEL_Y_2;

            return;
        }

        col = RGB(162, 165, 154);
        hPen2 = CreatePen(pen_style, RING_1_WIDTH, col);
        holdPen2 = (HPEN)SelectObject(hdc, hPen2);

        Line[0] = POINT{(int)Prev_X_N_1, (int)Prev_Y_1};
        Line[1] = POINT{(int)PrevToPrev_X_N_1, (int)PrevToPrev_Y_1};
        Polyline(hdc, Line, 2);

        DeleteObject(hPen2);
        DeleteObject(holdPen2);

        col = RGB(0, 0, 0);
        hPen2 = CreatePen(pen_style, RING_1_WIDTH, col);
        holdPen2 = (HPEN)SelectObject(hdc, hPen2);

        Line[0] = POINT{(int)PIXEL_X_N_1, (int)PIXEL_Y_1};
        Line[1] = POINT{(int)Prev_X_N_1, (int)PIXEL_Y_1};

        Polyline(hdc, Line, 2);

        DeleteObject(hPen2);
        DeleteObject(holdPen2);

        PrevToPrev_X_N_1 = Prev_X_N_1;
        PrevToPrev_Y_1 = Prev_Y_1;

        Prev_X_N_1 = PIXEL_X_N_1;
        Prev_X_P_1 = PIXEL_X_P_1;
        Prev_Y_1   = PIXEL_Y_1;

        //----------------Ring 2----------
        
        PIXEL_X_P_2 = X_O + (int)(X_C * RING_2_SIZE);
        PIXEL_Y_2 = Y_O + (int)(Y_C * (RING_2_SIZE - 15));
        PIXEL_X_N_2 = X_O - (int)(X_C * RING_2_SIZE);

        col = RGB(162, 165, 154);
        hPen2 = CreatePen(pen_style, RING_2_WIDTH - 1, col);
        holdPen2 = (HPEN)SelectObject(hdc, hPen2);
        

        Line[0] = POINT{(int)Prev_X_N_2, (int)Prev_Y_2};
        Line[1] = POINT{(int)PrevToPrev_X_N_2, (int)PrevToPrev_Y_2};
        Polyline(hdc, Line, 2);

        DeleteObject(hPen2);
        DeleteObject(holdPen2);

        col = RGB(0, 0, 0);
        hPen2 = CreatePen(pen_style, RING_2_WIDTH - 1, col);
        holdPen2 = (HPEN)SelectObject(hdc, hPen2);
        

        Line[0] = POINT{(int)PIXEL_X_N_2, (int)PIXEL_Y_2};
        Line[1] = POINT{(int)Prev_X_N_2, (int)PIXEL_Y_2};

        Polyline(hdc, Line, 2);

        DeleteObject(hPen2);
        DeleteObject(holdPen2);

        PrevToPrev_X_N_2 = Prev_X_N_2;
        PrevToPrev_Y_2 = Prev_Y_2;

        Prev_X_N_2 = PIXEL_X_N_2;
        Prev_X_P_2 = PIXEL_X_P_2;
        Prev_Y_2   = PIXEL_Y_2;
    }
    else
    {                
        PrevToPrev_X_N_1 = Prev_X_N_1;
        PrevToPrev_X_P_1 = Prev_X_P_1;
        PrevToPrev_Y_1 = Prev_Y_1;

        PrevToPrev_X_N_2 = Prev_X_N_2;
        PrevToPrev_X_P_2 = Prev_X_P_2;
        PrevToPrev_Y_2 = Prev_Y_2;

        Prev_X_N_1 = X_O + 0;
        Prev_Y_1 = Y_O + int(2 * (RING_1_SIZE - 15));
        Prev_X_P_1 = X_O + 0;

        Prev_X_N_2 = X_O + 0;
        Prev_Y_2 = Y_O + int(2 * (RING_2_SIZE - 15));
        Prev_X_P_2 = X_O + 0;
        ISRIGHTHALFRINGCOMPLETE = 0;
        ISLEFTHALFRINGCOMPLETE = 0;
    } 

    DeleteObject(hPen2);
    DeleteObject(holdPen2);
}


void MakeSaturnCircle(HDC hdc, PAINTSTRUCT ps, INT X_O, INT Y_O)
{
    Graphics graphics(hdc);

    LinearGradientBrush linGrBrush(
    Point(X_O - 150, Y_O),
    Point(X_O + 150, Y_O),
    Color(255, 0, 0, 0),   
    Color(255, 245, 201, 35));  

    REAL relativeIntensities[] = {0.0f, 0.5f, 1.0f};
    REAL relativePositions[]   = {0.0f, IncreaseBlack, 1.0f};

    linGrBrush.SetBlend(relativeIntensities, relativePositions, 3);

    graphics.FillEllipse(&linGrBrush, X_O - 150, Y_O - 250 + 70, 300, 300);

}


