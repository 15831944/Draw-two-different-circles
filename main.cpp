#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
int num =0;
POINT center1,center2,tangent ;
double R,R1,R2;

void Swap(int& x,int& y)
{
    int tmp=x;
    x=y;
    y=tmp;
}

int Round(double x)
{
    return (int)(x+0.5);
}
void DrawLine(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
{
    int dx=xe-xs;
    int dy=ye-ys;
    if(abs(dy)<=abs(dx))
    {
        double slope=(double)dy/dx;
        if(xs>xe)
        {
            Swap(xs,xe);
            Swap(ys,ye);
        }
        for(int x=xs; x<=xe; x++)
        {
            int y=Round(ys+(x-xs)*slope);
            SetPixel(hdc,x,y,color);
        }
    }
    else
    {
        double islope=(double)dx/dy;
        if(ys>ye)
        {
            Swap(xs,xe);
            Swap(ys,ye);
        }
        for(int y=ys; y<=ye; y++)
        {
            int x=Round(xs+(y-ys)*islope);
            SetPixel(hdc,x,y,color);
        }
    }
}
void Draw8Points (HDC hdc, int xc, int yc, int x, int y , COLORREF c)
{
    SetPixel(hdc,xc+x,yc+y,c);
    SetPixel(hdc,xc-x,yc+y,c);
    SetPixel(hdc,xc+x,yc-y,c);
    SetPixel(hdc,xc-x,yc-y,c);

    SetPixel(hdc,xc+y,yc+x,c);
    SetPixel(hdc,xc+y,yc-x,c);
    SetPixel(hdc,xc-y,yc+x,c);
    SetPixel(hdc,xc-y,yc-x,c);
}

void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF c, bool isFirstCircle)
{

    int x=0;
    int y=R;
    int d= 1-R;
    int d1=3;
    int d2=5-(2*R);
    Draw8Points (hdc,  xc,  yc,  x,  y , c);
    while(x<y)
    {

        if(d<0)
        {

            d+=d1;
            d2+=2;
        }
        else
        {
            d+=d2;
            d2+=4;
            y--;
        }
        d1+=2;
        x++;
        Draw8Points(hdc, xc,yc,x,y,c);
        if (isFirstCircle)
        {
            DrawLine(hdc ,xc+x ,yc+y ,xc-x,yc-y, c);
        }
    }

}
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
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
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
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
    HDC hdc=GetDC(hwnd);
    // DrawCircle( hdc, 60, 60, 50, RGB(44,56,9));


    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONDOWN:
        if (num==0)
        {
            center1.x = LOWORD(lParam);
            center1.y = HIWORD(lParam);

            // DrawCircle( hdc, center1.x, center1.y, 80, RGB(0,0,255));
            num++;
        }
        else if(num==1)
        {
            center2.x = LOWORD(lParam);
            center2.y = HIWORD(lParam);
            // DrawCircle( hdc, center2.x, center2.y, 80, RGB(0,0,255));
            num++;
        }

        break;
    case WM_RBUTTONDOWN:
        tangent.x = LOWORD(lParam);
        tangent.y = HIWORD(lParam);
        R  = sqrt(pow((center1.y - center2.y),2)+ pow((center1.x - center2.x ),2)) ;
        R1 = sqrt(pow((center1.y - tangent.y),2)+ pow((center1.x - tangent.x),2)) ;
        R2 = R - R1;
        DrawCircle( hdc, center1.x, center1.y, R1, RGB(0,0,0),true);
        DrawCircle( hdc, center2.x, center2.y, R2, RGB(0,0,0),false);
        num = 0;

        break;

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
