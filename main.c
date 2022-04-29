#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif


#include "Logger.h"
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include "model.h"
#include "viewer.h"
#include "FileReading.h"
#include "menu.h"
#include <Commdlg.h>
#include <math.h>
#include "carriage.h"

#define NO_SCROLL 25



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;             /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_OWNDC;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(MyMenu);
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("text_viewer"),       /* Title Text */
           WS_OVERLAPPEDWINDOW| WS_VSCROLL | WS_HSCROLL, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           CW_USEDEFAULT,                 /* The programs width */
           CW_USEDEFAULT,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument                 /* No Window Creation data */
           );


    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}




/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static textModel_t text_model;
    static viewModel_t view_model;
    static carriage_t carriage;
    static logger_t log;
    static size_t prevBufSize;

    static HMENU hmenu;
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    TEXTMETRIC tm;
    SCROLLINFO si;


    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
        {
            hmenu = GetMenu(hwnd);
            switch(LOWORD(wParam)){

                case IDM_MENU_FILE_OPEN:
                    EnableMenuItem(hmenu, IDM_MENU_TEXT_USUAL, MF_GRAYED);
                    EnableMenuItem(hmenu, IDM_MENU_TEXT_WRAP, MF_ENABLED);

                    freeTextModel(&text_model);
                    view_model.wndParam.size.vertical = HIWORD(lParam);
                    view_model.wndParam.size.horizontal = LOWORD(lParam);
                    FillUsualMode(&view_model,&text_model,&carriage);
                    FileOpenDlg(hwnd,&text_model,&view_model,&log);
                    view_model.scrollParam.position.horizontal=0;
                    view_model.scrollParam.position.vertical = 0;

                    SetScrollPos(hwnd, SB_HORZ, view_model.scrollParam.position.horizontal, TRUE);
                    SetScrollPos(hwnd, SB_VERT, view_model.scrollParam.position.vertical, TRUE);
                    SetCaretPos(carriage.position.horizontal * view_model.fontParam.charWidth, carriage.position.vertical * view_model.fontParam.charHeight);

                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);



                    break;
                case IDM_MENU_TEXT_USUAL:
                    EnableMenuItem(hmenu, IDM_MENU_TEXT_USUAL, MF_GRAYED);
                    EnableMenuItem(hmenu, IDM_MENU_TEXT_WRAP, MF_ENABLED);

                    FillUsualMode(&view_model,&text_model,&carriage);
                    view_model.scrollParam.maxPos.horizontal = max(0, (text_model.max_length*view_model.fontParam.charWidth - view_model.wndParam.size.horizontal) / view_model.fontParam.charWidth);
                    view_model.scrollParam.position.horizontal = 0;
                    view_model.scrollParam.maxPos.vertical = max(0, view_model.numStrs -1 - view_model.wndParam.size.vertical / view_model.fontParam.charHeight);


                    SetScrollRange(hwnd, SB_HORZ, 0, view_model.scrollParam.maxPos.horizontal, FALSE);
                    SetScrollPos(hwnd, SB_HORZ, view_model.scrollParam.position.horizontal, TRUE);
                    SetScrollRange(hwnd, SB_VERT, 0, view_model.scrollParam.maxPos.vertical, FALSE);
                    SetCaretPos(carriage.position.horizontal * view_model.fontParam.charWidth, carriage.position.vertical * view_model.fontParam.charHeight);

                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                case  IDM_MENU_TEXT_WRAP:
                    EnableMenuItem(hmenu, IDM_MENU_TEXT_USUAL, MF_ENABLED);
                    EnableMenuItem(hmenu, IDM_MENU_TEXT_WRAP, MF_GRAYED);

                    CountStringsWrap(&text_model,&view_model);
                    view_model.numStrs=text_model.num_strings_wrap;
                    FillStringsWrap(&view_model,&text_model,&carriage,view_model.numStrs);
                    view_model.scrollParam.maxPos.vertical = max(0, view_model.numStrs -1 - view_model.wndParam.size.vertical / view_model.fontParam.charHeight);

                    SetScrollRange(hwnd, SB_VERT, 0, view_model.scrollParam.maxPos.vertical, FALSE);
                    SetScrollRange(hwnd, SB_HORZ, 0, view_model.scrollParam.maxPos.horizontal, FALSE);
                    SetScrollPos(hwnd, SB_HORZ, view_model.scrollParam.position.horizontal, TRUE);
                    SetCaretPos(carriage.position.horizontal * view_model.fontParam.charWidth, carriage.position.vertical * view_model.fontParam.charHeight);

                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;

                case IDM_MENU_QUIT:
                    SendMessage(hwnd, WM_DESTROY, 0, 0L);
                    return 0;
                }
        }
        case WM_CREATE:
        {
            hdc = GetDC(hwnd);
            hmenu = GetMenu(hwnd);
            InitLog(&log);


            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            GetTextMetrics(hdc, &tm);

            ReleaseDC(hwnd, hdc);

            GetClientRect(hwnd, &rect);



            CREATESTRUCT *ex = (CREATESTRUCT *)lParam;
            if (ex && ex->lpCreateParams)
            {
                FileOpen((char*)ex->lpCreateParams,hwnd,&text_model,&log);
                InitViewModel(&view_model,&text_model, tm);
                if(log.status!=SUCCESS)
                {
                    carriage.position.horizontal=0;
                    carriage.position.vertical=0;

                    EnableMenuItem(hmenu, IDM_MENU_TEXT_USUAL, MF_GRAYED);
                    EnableMenuItem(hmenu, IDM_MENU_TEXT_WRAP, MF_GRAYED);
                }


            }
            break;
        }
        case WM_SIZE:
            view_model.wndParam.size.vertical = HIWORD(lParam);
            view_model.wndParam.size.horizontal = LOWORD(lParam);

            if (view_model.mode==WRAP)
             {
                CountStringsWrap(&text_model,&view_model);
                view_model.numStrs=text_model.num_strings_wrap;
                view_model.scrollParam.position.vertical=findNewScrollVertPos(view_model.wndParam.size.horizontal/ view_model.fontParam.charWidth-1, prevBufSize, &text_model);
                FillStringsWrap(&view_model,&text_model,&carriage,view_model.numStrs);
             }

            view_model.scrollParam.maxPos.vertical = max(0, view_model.numStrs - view_model.wndParam.size.vertical / view_model.fontParam.charHeight);
            if(view_model.scrollParam.maxPos.vertical<0)
                view_model.scrollParam.maxPos.vertical=0;


            view_model.scrollParam.position.vertical = min(view_model.scrollParam.position.vertical, view_model.scrollParam.maxPos.vertical);

            SetScrollRange(hwnd, SB_VERT, 0, view_model.scrollParam.maxPos.vertical, FALSE);
            SetScrollPos(hwnd, SB_VERT, view_model.scrollParam.position.vertical, TRUE);

            if (view_model.mode==USUAL)
            {

                view_model.scrollParam.maxPos.horizontal = max(0, (text_model.max_length*view_model.fontParam.charWidth - view_model.wndParam.size.horizontal) / view_model.fontParam.charWidth);
                view_model.scrollParam.position.horizontal = min(view_model.scrollParam.position.horizontal, view_model.scrollParam.maxPos.horizontal);
                SetScrollRange(hwnd, SB_HORZ, 0, view_model.scrollParam.maxPos.horizontal, FALSE);
                SetScrollPos(hwnd, SB_HORZ, view_model.scrollParam.position.horizontal, TRUE);
            }



            InvalidateRect(hwnd, NULL, TRUE);
            SendMessage(hwnd, WM_VSCROLL, NO_SCROLL, 0L);
            SendMessage(hwnd, WM_HSCROLL, NO_SCROLL, 0L);
            UpdateWindow(hwnd);

            return 0;
        case WM_VSCROLL:
            switch (LOWORD(wParam))
            {
            case SB_TOP:
                view_model.scrollParam.difference.vertical = -view_model.scrollParam.position.vertical;
                break;
            case SB_BOTTOM:
                view_model.scrollParam.difference.vertical = view_model.scrollParam.maxPos.vertical - view_model.scrollParam.position.vertical;
                break;
            case SB_LINEUP:
                view_model.scrollParam.difference.vertical = -1;
                break;
            case SB_LINEDOWN:
                view_model.scrollParam.difference.vertical = 1;
                break;
            case SB_PAGEUP:
                view_model.scrollParam.difference.vertical = min(-1, -view_model.wndParam.size.vertical / view_model.fontParam.charHeight);
                break;
            case SB_PAGEDOWN:
                view_model.scrollParam.difference.vertical = max(1, view_model.wndParam.size.vertical / view_model.fontParam.charHeight);
                break;
            case SB_THUMBTRACK:
                ZeroMemory(&si, sizeof(si));
                si.cbSize = sizeof(si);
                si.fMask = SIF_TRACKPOS;
                if (!GetScrollInfo(hwnd, SB_VERT, &si))
                    break;

                view_model.scrollParam.difference.vertical = si.nTrackPos - view_model.scrollParam.position.vertical;
                break;
            default:
                view_model.scrollParam.difference.vertical = 0;
                break;
            }

            view_model.scrollParam.difference.vertical = max(-view_model.scrollParam.position.vertical, min(view_model.scrollParam.difference.vertical, view_model.scrollParam.maxPos.vertical - view_model.scrollParam.position.vertical));

            if (view_model.scrollParam.difference.vertical != 0)
            {
                view_model.scrollParam.position.vertical += view_model.scrollParam.difference.vertical;
                carriage.position.vertical-=view_model.scrollParam.difference.vertical;
                ScrollWindow(hwnd, 0, -view_model.fontParam.charHeight * view_model.scrollParam.difference.vertical, NULL, NULL);
                SetScrollPos(hwnd, SB_VERT, view_model.scrollParam.position.vertical, TRUE);
                prevBufSize=findBufSize(view_model.wndParam.size.horizontal/ view_model.fontParam.charWidth-1, view_model.scrollParam.position.vertical, &text_model);
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
            }
            return 0;

        case WM_HSCROLL:
            if (view_model.mode==WRAP)
                return 0;

            switch(LOWORD(wParam))
                {
                case SB_LINEUP:
                    view_model.scrollParam.difference.horizontal = -1;
                    break;
                case SB_LINEDOWN:
                    view_model.scrollParam.difference.horizontal = 1;
                    break;
                case SB_PAGEUP:
                    view_model.scrollParam.difference.horizontal = -8;
                    break;
                case SB_PAGEDOWN:
                   view_model.scrollParam.difference.horizontal = 8;
                    break;
                case SB_THUMBTRACK:
                    ZeroMemory(&si, sizeof(si));
                    si.cbSize = sizeof(si);
                    si.fMask = SIF_TRACKPOS;
                    if (!GetScrollInfo(hwnd, SB_HORZ, &si))
                        break;

                    view_model.scrollParam.difference.horizontal = si.nTrackPos - view_model.scrollParam.position.horizontal;
                    break;
                default:
                    view_model.scrollParam.difference.horizontal = 0;
                    break;
                }

            view_model.scrollParam.difference.horizontal = max(-view_model.scrollParam.position.horizontal, min(view_model.scrollParam.difference.horizontal, view_model.scrollParam.maxPos.horizontal - view_model.scrollParam.position.horizontal));
            if (view_model.scrollParam.difference.horizontal != 0)
            {
                view_model.scrollParam.position.horizontal += view_model.scrollParam.difference.horizontal;
                carriage.position.horizontal-=view_model.scrollParam.difference.horizontal;
                ScrollWindow(hwnd, -view_model.fontParam.charWidth * view_model.scrollParam.difference.horizontal, 0, NULL, NULL);
                SetScrollPos(hwnd, SB_HORZ, view_model.scrollParam.position.horizontal, TRUE);
            }
            return 0;
        case WM_PAINT:
        {
            hdc = BeginPaint(hwnd,&ps);
            int i,x,y;
            i=x=y=0;
            int iPaint = max(0, view_model.scrollParam.position.vertical + ps.rcPaint.top / view_model.fontParam.charHeight - 1);
            int iPaintEnd = min(view_model.numStrs, view_model.scrollParam.position.vertical + ps.rcPaint.bottom / view_model.fontParam.charHeight);
            if (view_model.mode==USUAL)
            {
                for (i = iPaint; i < iPaintEnd; i++)
                {
                    x = view_model.fontParam.charWidth * (1 - view_model.scrollParam.position.horizontal)-view_model.fontParam.charWidth;
                    y = view_model.fontParam.charHeight * (1 - view_model.scrollParam.position.vertical + i)-view_model.fontParam.charHeight;
                    if (text_model.strings[i+1] - text_model.strings[i] == 0)
                        continue;
                    TextOut(hdc, x, y, text_model.buff + text_model.strings[i], text_model.strings[i+1] - text_model.strings[i] -1);
                }
            }
            else if(view_model.mode==WRAP)
            {
                size_t i,x = 0,y = 0,wndSize,pointer,strLenght;
                char *textForPrint = NULL;
                wndSize=view_model.wndParam.size.horizontal/ view_model.fontParam.charWidth-1;
                pointer = findBufSize(wndSize, view_model.scrollParam.position.vertical, &text_model);
                for(i =  view_model.scrollParam.position.vertical; i < iPaintEnd; i++)
                {
                    strLenght = 1;
                    textForPrint = (char*)text_model.buff + pointer;

                    while(text_model.buff[pointer] != '\n')
                    {
                            if(strLenght > wndSize || text_model.buff[pointer] == '\0')
                                break;


                            strLenght++;
                            pointer++;
                    }
                    TextOut(hdc, x, y, textForPrint, strLenght);
                    y = view_model.fontParam.charHeight * (1 - view_model.scrollParam.position.vertical + i);
                    pointer++;
                }
            }
            EndPaint(hwnd,&ps);
            break;
        }

        case WM_SETFOCUS:
        {
            CreateCaret(hwnd, (HBITMAP)NULL, view_model.fontParam.charWidth, view_model.fontParam.charHeight);
            SetCaretPos(carriage.position.horizontal * view_model.fontParam.charWidth, carriage.position.vertical * view_model.fontParam.charHeight);
            ShowCaret(hwnd);
            break;
        }

        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_DOWN:
                    carriageDOWN(hwnd,&carriage,&view_model,&text_model);
                    break;
                case VK_UP:
                    carriageUP(hwnd,&carriage,&view_model,&text_model);
                    break;
                case VK_LEFT:
                    carriageLEFT(hwnd,&carriage,&view_model,&text_model);
                    break;
                case VK_RIGHT:
                    carriageRIGHT(hwnd,&carriage,&view_model,&text_model);
                    break;
                case VK_F1:
                    SendMessage(hwnd, WM_COMMAND, IDM_MENU_TEXT_USUAL, 0L);
                    break;
                case VK_F2:
                    SendMessage(hwnd, WM_COMMAND, IDM_MENU_TEXT_WRAP, 0L);
                    break;
                case VK_ESCAPE:
                    SendMessage(hwnd, WM_COMMAND, IDM_MENU_QUIT, 0L);

            }

            SetCaretPos(carriage.position.horizontal * view_model.fontParam.charWidth, carriage.position.vertical * view_model.fontParam.charHeight);
            SetScrollPos(hwnd, SB_HORZ, view_model.scrollParam.position.horizontal, TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            return 0;
        case WM_MOUSEWHEEL:
            if (GET_WHEEL_DELTA_WPARAM(wParam) >= 0)
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
            else
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
            return 0;


        case WM_DESTROY:

            freeTextModel(&text_model);
            freeViewModel(&view_model);
            CloseLog(&log);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            return 0;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

