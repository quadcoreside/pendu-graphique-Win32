#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedurePartie (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName2[ ] = _T("MoussaPenduApp2");

HINSTANCE hInst2;

int FormPartie(HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwndPartie;               /* This is the handle for our window */

    MSG messages2;            /* Here messages2 to the application are saved */
    WNDCLASSEX wincl2;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl2.hInstance = hThisInstance;
    wincl2.lpszClassName = szClassName2;
    wincl2.lpfnWndProc = WindowProcedurePartie;      /* This function is called by windows */
    wincl2.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl2.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl2.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl2.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl2.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl2.lpszMenuName = NULL;                 /* No menu */
    wincl2.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl2.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl2.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl2))
        return 0;

    hInst2 = hThisInstance;

    /* The class is registered, let's create the program*/
    hwndPartie = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName2,         /* Classname */
           _T("PENDU By Moussa"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           450,                 /* The programs width */
           200,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwndPartie, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages2, NULL, 0, 0))
    {
        /* Translate virtual-key messages2 into character messages2 */
        TranslateMessage(&messages2);
        /* Send message to WindowProcedurePartie */
        DispatchMessage(&messages2);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages2.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedurePartie (HWND hwndPartie, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    static HWND hButton;

    switch (message)                  /* handle the messages2 */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_SIZE:

            break;
        case WM_CREATE:

            hButton = CreateWindow( "button", "Label",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                500, 300,
                50, 20,
                hwndPartie, (HMENU) 100,
                hInst2, NULL );
        break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case 100:
                {
                    MessageBox (hwndPartie, "Button", "Btn cliquer", 0);
                }
                break;
            }
        break;
        default:                      /* for messages2 that we don't deal with */
            return DefWindowProc (hwndPartie, message, wParam, lParam);
    }

    return 0;
}
