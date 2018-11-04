#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "main.h"


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedurePartie (HWND, UINT, WPARAM, LPARAM);


/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("MoussaPenduApp");
TCHAR szClassName2[ ] = _T("MoussaPenduApp2");

HINSTANCE hInst;
HINSTANCE hInst2;
int cmdShow;
HWND hwndMain;
HWND hwndPartie;

static unsigned int level = 1;
static unsigned int nbrEssais = 0;
static bool isGagner = false;

static char motSecret[10];
static char motIndice[200];
static char indiceText[200];
static char motEnter[10];

static char motValidation[100];


void DrawValues(HDC hdc, int points, int level);
void OnSomeActionToRefreshValues();

bool mainFormClosed, formPartieClosed, endProgram;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    //HWND hwnd;               /* This is the handle for our window */
    FreeConsole();

    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW+4);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    hInst = hThisInstance;

    /* The class is registered, let's create the program*/
    hwndMain = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("PENDU V1 BETA Moussa"),       /* Title Text */
                (WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX ), /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               800,                 /* The programs width */
               400,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );


    /***FORM 2****/
    //HWND hwndPartie;               /* This is the handle for our window */
    cmdShow = nCmdShow;


    //MSG messages2;            /* Here messages2 to the application are saved */
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
    wincl2.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl2))
        return 0;

    hInst2 = hThisInstance;

    /* The class is registered, let's create the program*/
    hwndPartie = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName2,         /* Classname */
           _T("PENDU V1 BETA Moussa"),       /* Title Text */
           (WS_EX_CLIENTEDGE  & WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX ), /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           376,                 /* The programs width */
           420,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    ShowWindow (hwndPartie, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (!endProgram)
    {
        if(GetMessage (&messages, NULL, 0, 0))
        {
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&messages);
            /* Send message to WindowProcedure */
            DispatchMessage(&messages);

            if (mainFormClosed == true && formPartieClosed == true) {
                endProgram = true;
            }
        }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

static HWND hMotEntrer;
static HWND hBtnValider;
static HWND hMotSecret;
static HWND hIndice;

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            //PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            mainFormClosed = true;
            return 0;
            break;
        case WM_SIZE:

            break;
        case WM_CREATE:
            CreateWindow("STATIC", "Mot Secret:",
                                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                                      400, 100, 100, 20,
                                      hwnd, (HMENU)150, hInst, 0);
            hMotEntrer = CreateWindow("EDIT", "",
                                      WS_CHILD | WS_VISIBLE | ES_LEFT | ES_WANTRETURN, // | ES_PASSWORD
                                      480, 100, 250, 20,
                                      hwnd, (HMENU)EDIT_MOT_ENTRER, hInst, 0);
            SendMessage(hMotEntrer, EM_SETLIMITTEXT, MAX_LENGHT_MOT, EDIT_MOT_ENTRER);

            hBtnValider = CreateWindow("BUTTON", "VALIDER",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                500, 130,
                100, 20,
                hwnd, (HMENU) BTN_VALIDER,
                hInst, NULL );

            CreateWindow("BUTTON", "RECOMMENCER",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                620, 330,
                150, 20,
                hwnd, (HMENU) BTN_RECOMMENCER,
                hInst, NULL );
        break;
        case WM_PAINT:
            {
                hdc = BeginPaint(hwnd, &ps);

                HPEN hPenOld;
                HPEN hLinePen;
                COLORREF qLineColor;
                qLineColor = RGB(255, 0, 0);
                hLinePen = CreatePen(PS_SOLID, 7, qLineColor);
                hPenOld = (HPEN)SelectObject(hdc, hLinePen);
                char tmp[100];
                strcpy(tmp,  "");

                /**TEXT A AFFICHER**/
                char charNbrEssais[5], charNiv[5];
                sprintf(charNbrEssais, "%d", (7 - nbrEssais));
                sprintf(charNiv, "%d", level);

                strcpy(tmp,  "Niveau: ");
                strcat(tmp, charNiv);
                strcpy(tmp,  "");
                TextOut(hdc, 10, 10, tmp, strlen(tmp));

                strcpy(tmp,  "ESSAIS RESTANT: ");
                strcat(tmp, charNbrEssais);
                TextOut(hdc, 10, 28, tmp, strlen(tmp));

                strcpy(tmp,  "");
                strcat(tmp, indiceText);
                TextOut(hdc, 10, 43, tmp, strlen(tmp));
                if(level == 1)
                {
                    strcpy(tmp,  "INDICE: ");
                    strcat(tmp, motIndice);
                    TextOut(hdc, 10, 58, tmp, strlen(tmp));
                }

                /**DESSIN**/
                if(nbrEssais < 7 && !isGagner)
                {
                    /**POTENCE**/
                    MoveToEx(hdc, 50, 350, NULL);
                    LineTo(hdc, 400, 350);

                    MoveToEx(hdc, 100, 350, NULL);
                    LineTo(hdc, 100, 100);

                    MoveToEx(hdc, 100, 150, NULL);
                    LineTo(hdc, 150, 100);

                    MoveToEx(hdc, 100, 100, NULL);
                    LineTo(hdc, 300, 100);

                    MoveToEx(hdc, 300, 100, NULL);
                    LineTo(hdc, 300, 150);

                    /**TETE**/
                    if(nbrEssais >= 1) {
                        Arc(hdc, 275, 150, 330, 200, 0, 0, 0, 0);
                    }

                    /**CORPS**/
                    if(nbrEssais >= 2) {
                        MoveToEx(hdc, 300, 200, NULL);
                        LineTo(hdc, 300, 270);
                    }

                    /**BRAS**/
                    if(nbrEssais >= 3) {
                        MoveToEx(hdc, 300, 225, NULL);
                        LineTo(hdc, 250, 200);
                    }
                    if(nbrEssais >= 4) {
                        MoveToEx(hdc, 300, 225, NULL);
                        LineTo(hdc, 350, 200);
                    }

                    /**JAMBE**/
                    if(nbrEssais >= 5) {
                        MoveToEx(hdc, 300, 270, NULL);
                        LineTo(hdc, 250, 330);
                    }
                    if(nbrEssais >= 6) {
                        MoveToEx(hdc, 300, 270, NULL);
                        LineTo(hdc, 350, 330);
                    }
                }

                if (isGagner) {
                    /**GAGNER**/
                    Arc(hdc, 200, 100, 300, 200, 0, 0, 0, 0);

                    //YEUX
                    Arc(hdc, 225, 120, 235, 130, 0, 0, 0, 0);
                    Arc(hdc, 275, 120, 265, 130, 0, 0, 0, 0);

                    //BOUCHE
                    MoveToEx(hdc, 245, 170, NULL);
                    LineTo(hdc, 235, 160);
                    MoveToEx(hdc, 245, 170, NULL);
                    LineTo(hdc, 255, 170);
                    MoveToEx(hdc, 255, 170, NULL);
                    LineTo(hdc, 265, 160);
                } else if (nbrEssais == 7) {

                    /**PERDUS**/
                    Arc(hdc, 200, 100, 300, 200, 0, 0, 0, 0);

                    //YEUX
                    Arc(hdc, 225, 120, 235, 130, 0, 0, 0, 0);
                    Arc(hdc, 275, 120, 265, 130, 0, 0, 0, 0);

                    //BOUCHE
                    MoveToEx(hdc, 235, 170, NULL);
                    LineTo(hdc, 265, 170);
                }

                SelectObject(hdc, hPenOld);
                DeleteObject(hLinePen);
            }
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case BTN_VALIDER:
                    testMot();
                break;
                case BTN_RECOMMENCER:
                    recommencerPartie();
                break;
            }
        break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedurePartie (HWND hwndPartie, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    static HWND rdi1;

    switch (message)                  /* handle the messages2 */
    {
        case WM_DESTROY:
            //PostQuitMessage (0);
            formPartieClosed = true;
            return 0;
            break;

        case WM_INITDIALOG:
            SendMessage(rdi1, BM_SETCHECK, BST_CHECKED, 1);
		break;
        case WM_PAINT:
            hdc = BeginPaint(hwndPartie, &ps);

            TextOut(hdc, 110, 15, _T(" Nouvelle Partie "), 17);
        break;
        case WM_CREATE:
            {

                CreateWindowEx(WS_EX_WINDOWEDGE,
                        "BUTTON",
                        "Jouer à deux:",
                        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                        5,50,
                        360,250,
                        hwndPartie,
                        (HMENU)BTN_JOUER_A_DEUX,
                        hInst, NULL);


                CreateWindowEx(WS_EX_WINDOWEDGE,
                        "BUTTON",
                        "Selectionner un niveau:",
                        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                        10,80,
                        350,100,
                        hwndPartie,
                        (HMENU)200,
                        hInst, NULL);
                rdi1 = CreateWindowEx(WS_EX_WINDOWEDGE,
                                    "BUTTON",
                                    "Facile",
                                    WS_VISIBLE | WS_CHILD|BS_AUTORADIOBUTTON|WS_GROUP,
                                    15,100,
                                    300,20,
                                    hwndPartie,
                                    (HMENU)RDI_NIV1,
                                    hInst, NULL);
                CreateWindowEx(WS_EX_WINDOWEDGE,
                                    "BUTTON",
                                    "Moyen",
                                    WS_VISIBLE | WS_CHILD|BS_AUTORADIOBUTTON,
                                    15,120,
                                    300,20,
                                    hwndPartie,
                                    (HMENU)RDI_NIV2,
                                    hInst, NULL);
                CreateWindowEx(WS_EX_WINDOWEDGE,
                                    "BUTTON",
                                    "Difficile",
                                    WS_VISIBLE | WS_CHILD|BS_AUTORADIOBUTTON,
                                    15,140,
                                    300,20,
                                    hwndPartie,
                                    (HMENU)RDI_NIV3,
                                    hInst, NULL);
                //SendMessage(rdi1, WM_SETFONT, (WPARAM) wParam, TRUE);

                CreateWindow("STATIC", "Mot Secret:",
                                          WS_CHILD | WS_VISIBLE | ES_LEFT,
                                          10, 200, 100, 20,
                                          hwndPartie, (HMENU)150, hInst, 0);
                hMotSecret = CreateWindow("EDIT", "",
                                          WS_CHILD | WS_VISIBLE | ES_LEFT | ES_WANTRETURN | ES_PASSWORD, // |
                                          105, 200, 250, 20,
                                          hwndPartie, (HMENU)EDIT_MOT_SECRET, hInst, 0);
                CreateWindow("STATIC", "Indice du mot:",
                                          WS_CHILD | WS_VISIBLE | ES_LEFT,
                                          10, 225, 100, 20,
                                          hwndPartie, (HMENU)152, hInst, 0);
                hIndice = CreateWindow("EDIT", "",
                                          WS_CHILD | WS_VISIBLE  | ES_LEFT | ES_WANTRETURN,
                                          105, 225, 250, 20,
                                          hwndPartie, (HMENU)EDIT_PHRASE_INDICE, hInst, 0);

                SendMessage(hMotSecret, EM_SETLIMITTEXT, MAX_LENGHT_MOT, EDIT_MOT_ENTRER);
                SendMessage(hIndice, EM_SETLIMITTEXT, MAX_LENGHT_PHRASE_INDICE, EDIT_PHRASE_INDICE);

                CreateWindow("BUTTON", "COMMENCER",
                    WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                    50, 265,
                    250, 20,
                    hwndPartie, (HMENU) BTN_JOUER_A_DEUX,
                    hInst, NULL );

                CreateWindowEx(WS_EX_WINDOWEDGE,
                        "BUTTON",
                        "Jouer Seul:",
                        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                        5,300,
                        360,50,
                        hwndPartie,
                        (HMENU)200,
                        hInst, NULL);
                CreateWindow("BUTTON", "JOUER SEUL",
                    WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                    50, 325,
                    250, 20,
                    hwndPartie, (HMENU) BTN_JOUER_SOLO,
                    hInst, NULL );


                CreateWindow("BUTTON", "QUITTER",
                    WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                    50, 360,
                    250, 20,
                    hwndPartie, (HMENU) BTN_QUITTER,
                    hInst, NULL );
            }
        break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case BTN_JOUER_A_DEUX:
                    confPartieADeux();
                break;
                case BTN_JOUER_SOLO:
                    confPartieSolo();
                break;
                case RDI_NIV1:
                case RDI_NIV2:
                    EnableWindow(hIndice, true);
                break;
                case RDI_NIV3:
                    EnableWindow(hIndice,false);
                break;
                case BTN_QUITTER:
                    exit(0);
                break;
            }
        break;
        default:                      /* for messages2 that we don't deal with */
            return DefWindowProc (hwndPartie, message, wParam, lParam);
    }

    return 0;
}

void strToLower(char str[])
{
    int i = 0;
    for(i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}
void my_strip(char *data)
{
    unsigned long i = 0; /* Scanning index */
    unsigned long x = 0; /* Write back index */
    char c;

    /*
     * Store every next character in `c` and make sure it is not '\0'
     * because '\0' indicates the end of string, and we don't want
     * to read past the end not to trigger undefined behavior.
     * Then increment "scanning" index so that next time we read the
     * next character.
     */
    while ((c = data[i++]) != '\0') {
        /* Check if character is either alphabetic or numeric. */
        if (isalnum(c)) {
            /*
             * OK, this is what we need. Write it back.
             * Note that `x` will always be either the same as `i`
             * or less. After writing, increment `x` so that next
             * time we do not overwrite the previous result.
             */
            data[x++] = c;
        }
        /* else — this is something we don't need — so we don't increment the
           `x` while `i` is incremented. */
    }
    /* After all is done, ensure we terminate the string with '\0'. */
    data[x] = '\0';
}

void confPartieADeux()
{
    int niv = 0;
    if(IsDlgButtonChecked(hwndPartie, RDI_NIV1)){
        niv = 1;
    }
    else if(IsDlgButtonChecked(hwndPartie, RDI_NIV2)){
        niv = 2;
    }
    else if(IsDlgButtonChecked(hwndPartie, RDI_NIV3)){
        niv = 3;
    }

    char buff[1024];
    GetWindowText(hMotSecret, buff, 1024);
    strncpy(motSecret, buff, sizeof(motSecret));
    my_strip(motSecret);

    GetWindowText(hIndice, buff, 1024);
    strncpy(motIndice, buff, sizeof(motIndice));
    //my_strip(motIndice);

    if(!(niv > 0 && niv < 4))
    {
        MessageBox (hwndPartie, "Veuillez sélectionner un niveau de jeux.",  "ERREUR", 0);
    }
    else
    {
        if(strlen(motSecret) > MAX_LENGHT_MOT)
        {
            MessageBox (hwndPartie, "Le mot secret est trop long.", "ERREUR", 0);
        }
        else
        {
            if(strlen(motIndice) > MAX_LENGHT_PHRASE_INDICE)
            {
                MessageBox (hwndPartie, "L'indice est trop long.", "ERREUR", 0);
            }

            level = niv;
            nbrEssais = 0;
            isGagner = 0;

            strcpy(indiceText, "");
            EnableWindow(hMotEntrer, true);
            EnableWindow(hBtnValider, true);

            ShowWindow (hwndPartie, SW_HIDE);
            ShowWindow (hwndMain, SW_SHOWNOACTIVATE);
            InvalidateRect(hwndMain, 0, TRUE);
        }
    }
}

void confPartieSolo()
{
    char words[10][8] = {
        "vache",
        "roche",
        "agent",
        "aigle",
        "avant",
        "verre",
        "girafe",
        "singe",
    };
    char wordsIndice[10][50] = {
        "Le Lait",
        "Une sorte de pierre",
        "Une personne speciale",
        "Un oiseau trés grand",
        "En arriere et ?",
        "Un conteneur",
        "Un animal long",
        "On dit que l'homme descend du ?",
    };

    srand(time(NULL));
    int c = rand()%(0-7) + 0; //Entre 0 et 7

    strncpy(motSecret, words[c], sizeof(motSecret));
    strncpy(motIndice, wordsIndice[c], sizeof(motIndice));

    level = 1;
    nbrEssais = 0;
    isGagner = 0;

    strcpy(indiceText, "");
    EnableWindow(hMotEntrer, true);
    EnableWindow(hBtnValider, true);

    ShowWindow (hwndPartie, SW_HIDE);
    ShowWindow (hwndMain, SW_SHOW);
    InvalidateRect(hwndMain, 0, TRUE);
}

void recommencerPartie()
{
    if(!isGagner && nbrEssais < 7) {
        const int result = MessageBox(NULL, "Vous ête sur le point de recommencer une partie ?", "Recommencer",  MB_YESNOCANCEL);

        switch (result)
        {
            case IDYES:
                SetWindowText( GetDlgItem( hwndMain, EDIT_MOT_ENTRER ), "" );
                ShowWindow (hwndMain, SW_HIDE);
                ShowWindow (hwndPartie, cmdShow);
                SetActiveWindow(hwndPartie);
                break;
            case IDNO:
                break;
            case IDCANCEL:
                break;
        }
    } else {
        SetWindowText( GetDlgItem( hwndMain, EDIT_MOT_ENTRER ), "" );
        ShowWindow (hwndMain, SW_HIDE);
        ShowWindow(hwndPartie, SW_SHOW);
        SetActiveWindow(hwndPartie);
    }
}

void testMot()
{
    char buff[128];

    strcpy(buff, "");
    strcpy(motEnter, "");

    GetWindowText(hMotEntrer, buff, 128);
    strncpy(motEnter, buff, sizeof(motEnter));

    my_strip(motEnter);

    strToLower(motEnter);
    int length = strlen(motSecret);
    strncpy(motValidation, motSecret, sizeof(motValidation));
    char motTmp[100] = "";
    strncpy(motTmp, motEnter, sizeof(motTmp));
    char memExistant[50] = "";;

    int c = 0;
    int m = 0;
    int f = 0;

    int i = 0;
    for(i = 0; i < length; i++)
    {
        if(motValidation[i] == motEnter[i])
        {
            c++;
            motTmp[i] = '*';
        }
        else
        {
            motValidation[i] = '*';
            f++;
        }
    }

    /*A la recherche de caractere existant mais mal place*/
    for(i = 0; i < length; i++)
    {
        if(motValidation[i] != motEnter[i])
        {
            int a = 0;
            for(a = 0; a < length; a++)
            {
                int deja = 0;
                int b = 0;
                int strlenMem = strlen(memExistant);
                /*On verifie si il na pas ete deja comptabilisé*/
                for(b = 0; b < strlenMem; b++)
                {
                    if(memExistant[b] == motEnter[i])
                    {
                        deja = 1; //On active le flag car il a ete deja comptablise
                        break;
                    }
                }
                /*Si il est existant dans le secret et nom pas deja comptabilisé*/
                if ((motTmp[a] == motSecret[i]) && deja == 0)
                {
                    m++;
                    f--;
                    memExistant[strlen(memExistant) + 1] = motEnter[i];
                }
            }
        }
    }

    nbrEssais++;

    if(nbrEssais == 7){

        strcpy(indiceText, "  Vous avez perdu !!! Le mot etait: \"");
        strcat(indiceText, motSecret);
        strcat(indiceText, "\"");

        EnableWindow(hMotEntrer, false);
        EnableWindow(hBtnValider, false);

        InvalidateRect(hwndMain, 0, TRUE);
        return;
    } else if(c == length){
        isGagner = 1;
        SetWindowText( GetDlgItem( hwndMain, EDIT_MOT_ENTRER ), "" );

        strcpy(indiceText, "Vous avez gagner BRAVOOOOOO !!!!! c'etait bien \"");
        strcat(indiceText, motSecret);
        strcat(indiceText, "\"");

        EnableWindow(hMotEntrer, false);
        EnableWindow(hBtnValider, false);

        InvalidateRect(hwndMain, 0, TRUE);
        return;
    }

    char indice[200];
    char ca[5],ma[5],fa[5];
    sprintf(ca, "%d", c);
    sprintf(ma, "%d", m);
    sprintf(fa, "%d", (((f-m) < 0) ? 0 : (f-m))); //Expression tenaire si le resultat de la soustraction est inferieur a 0 on garde 0

    strcat(indice, ca);
    strcat(indice, " Caractère Bien Placer | ");
    strcat(indice, ma);
    strcat(indice, " Caractère Mal Placer | ");
    strcat(indice, fa);
    strcat(indice, " Caractère Inutile");
    if(level <= 2)
    {
        strcat(indice, " | --> \"");
        strcat(indice, motValidation);
        strcat(indice, "\"");
    }

    strcpy(indiceText, indice);

    InvalidateRect(hwndMain, 0, TRUE);
}
