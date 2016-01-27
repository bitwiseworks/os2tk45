/*************************************************************************
 * Copyright (C) International Business Machines Corp., 1995.
 *
 * ToyBox is a basic Win32 application that uses BITBLT to display a
 * large number of simple bitmaps on the display screen and give them
 * motion. It moves the objects both around the client portion of the
 * screen, and makes the objects appear to change shape and rotate.
 *
 * This program displays a menu with five choices:
 *
 *      Main               - Display a submenu with the following five items:
 *        Start            - Start movement on the screen.
 *        Stop             - Stop movement on the screen.
 *        Maximum Toys     - Display maximum number of objects.
 *        Minimum Toys     - Remove All objects from the screen.
 *      Quit               - Terminate this program.
 *
 *      Add Toy            - Display an additional object, up to a preset limit.
 *
 *      Delete Toy         - Remove an object from the screen.
 *
 *  Step Toys              - If the objects are not in motion, move them one
 *                           step per press of the control
 *
 *  Help              - Display a submenu with two subitems:
 *  General Help      - Explain the basic program operation to the user.
 *  About             - Display program and author information.
 *
 *
 ************************************************************************/

/*************************************************************************
 * include files of common stuff that we need to use.
 ************************************************************************/

#include "toybox.h"

/*************************************************************************
 * These are the function prototypes for functions that we call.
 ************************************************************************/

LRESULT CALLBACK WndProc(HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK AboutProc(HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam);
void NewObject(int ObjIndex);
void MoveObject(HWND InhWnd, int DoMoveObject);
int iRand_Num(int lower_limit, int upper_limit);
int iRand_Range(int lower_limit, int upper_limit, int low_excld, int up_excld);
void SetWinText(HWND InhWnd);
void LoadBMP(HANDLE InInstance);
void AnimateObjects(HDC hToDC);

/*************************************************************************
 * Generic variables needed by this program.
 ************************************************************************/

char TitleText[]="Toy Box Demo - [Toys ";   /* default title        */
int iScreenX;                               /* client X in pixels   */
int iScreenY;;                              /* client Y in pixles   */
int Screen_OK;                              /* screen status        */
int ObjCount;                               /* # of active objects  */
int ObjActive=0;                            /* no active objects    */
int MaxBmpX=0;                              /* Max in X direction   */
int MaxBmpY=0;                              /* Max in Y direction   */
HWND hAbout;                                /* about windows hwnd   */
RECT ChildDrawRect;                         /* child drawing rect   */
HDC hToDC;                                  /* DC we display on     */
HBITMAP hObjBMP[MAX_OBJ_TYPE][MAX_OBJ_FRAME];   /* bitmap for all   */
HDC PixDC[MAX_OBJ_TYPE][MAX_OBJ_FRAME];         /* preselected DCs  */
char AboutText[]="\n\nToy Box Bit Blit Demo\nVersion 1.01"
                 "\nCopyright 1995\nIBM Corporation\nby Daniel Mendrala\n";

/*************************************************************************
 * A structure to hold information about each object on the screen.
 ************************************************************************/

typedef struct tagObjArrayType
 { int ObjX;                                /* Objects's X                  */
   int ObjY;                                /* Objects's Y                  */
   int ObjWX;                               /* Width of X                   */
   int ObjWY;                               /* Width of Y                   */
   int ObjType;                             /* what Object ?                */
   int ObjFrame;                            /* picture frame                */
   int ObjDX;                               /* Object Delta X               */
   int ObjDY;                               /* Object Delta Y               */
   int ObjStatus;                           /* Object Status                */
   int SpinCnt;                             /* curr spin cycle count        */
   int SpinWait;                            /* # cycles before spin         */
 } ObjArrayType;

 ObjArrayType ObjArray[MAX_OBJ+1];          /* 1 based, all objects */
 ObjArrayType RollObj[MAX_OBJ_TYPE+1];      /* 1 based, one of each */

/*************************************************************************
 * A structure to hold info about each type of object. Picture data.
 ************************************************************************/

typedef struct tagPixType
 {
  int X;                                    /* bitmap size in X             */
  int Y;                                    /* bitmap size in Y             */
  int Frame;                                /* # frames animation           */
  int Speed;                                /* Wait time for frames         */
  int MDX;                                  /* Max Delta X                  */
  int MDY;                                  /* Max Delta Y                  */
 } PixType;

 PixType Pix[MAX_OBJ_TYPE];                 /* all possible types           */

/*************************************************************************
 *
 * The main program sets up all of the information needed to register the
 * window class. It then creates the window, shows it then enters the
 * message loop that processes all of the messages until the user
 * exits the program.
 *
 ************************************************************************/

int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPreInst,
                                        LPSTR lpszCmdLine,int nCmdShow) {

HWND hWnd;
MSG Msg;
WNDCLASS wcApp;

if (!hPreInst)
{
 wcApp.style=                   CS_HREDRAW|CS_VREDRAW;
 wcApp.hInstance=               hInstance;
 wcApp.lpfnWndProc=     WndProc;
 wcApp.hCursor=                 LoadCursor(NULL,IDC_ARROW);
 wcApp.hIcon=                   LoadIcon(hInstance,MAKEINTRESOURCE(ToyIcon));
 wcApp.lpszMenuName=    ToyMenu;
 wcApp.lpszClassName=   "ToyClass";
 wcApp.hbrBackground=   GetStockObject(BLACK_BRUSH);
 wcApp.cbClsExtra=              0;
 wcApp.cbWndExtra=              0;

 if (!RegisterClass(&wcApp))
   return FALSE;

 wcApp.style=                   CS_HREDRAW|CS_VREDRAW;
 wcApp.hInstance=               hInstance;
 wcApp.lpfnWndProc=     AboutProc;;
 wcApp.hCursor=                 LoadCursor(NULL,IDC_ARROW);
 wcApp.hIcon=                   LoadIcon(hInstance,MAKEINTRESOURCE(ToyIcon));
 wcApp.lpszMenuName=    NULL;
 wcApp.lpszClassName=   "AboutClass";
 wcApp.hbrBackground=   GetStockObject(BLACK_BRUSH);
 wcApp.cbClsExtra=              0;
 wcApp.cbWndExtra=              0;

 if (!RegisterClass(&wcApp))
   return FALSE;

}

hWnd = CreateWindow("ToyClass",
                    "Toy Box Demo",
                    WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    NULL,
                    NULL,
                    hInstance,
                    NULL);

ShowWindow(hWnd,nCmdShow);

/*************************************************************************
 *
 * Now we peek to see if a message is in the queue. If their is one, then
 * we dispatch the message, otherwise if the 'Start' option has selected
 * we move all of the objects on the screen and then sleep for a short
 * amount of time. The repeats until the user quits the program.
 *
 ************************************************************************/

while (TRUE)
 {
  if (PeekMessage (&Msg,NULL,0,0,PM_REMOVE))
  {
   if (Msg.message==WM_QUIT)
    return Msg.wParam;
   else
   {
    TranslateMessage(&Msg);
        DispatchMessage(&Msg);
   }
  }                                         /* end if message wait          */
  else if (ObjActive)
   {
    MoveObject(hWnd,TRUE);
    Sleep(10);                              /* sleep 10 MS                  */
   }
 }                                          /* end While                    */
}                                           /* end WinMain                  */

/*************************************************************************
 *
 * This is the window procedure for the main window. The following
 * messages are processed by this procedure:
 *
 * WM_CREATE               Get the destination DC, initial setup, load bitmaps
 * WM_SIZE                 Save the new X and Y client sizes and repaint client
 * WM_MOVE                 Repaint client
 * WM_DESTROY              Post a quit message
 * WM_PAINT                Just re-draw the objects in the client window
 * WM_COMMAND
 *         IDM_START       Toggle checkmarks and active the objects movement
 *         IDM_STOP        Toggle checkmarks and deactive the object movement
 *         IDM_DELOBJ      If any objects exist, remove the last one and redraw
 *         IDM_ADDOBJ      If below limit, add a new object and redraw
 *         IDM_MINOBJ      While any objects exist, mark for delete and redraw
 *         IDM_MAXOBJ      While below limit, add a new object, then redraw
 *         IDM_STEP        Move the objects one step
 *         IDM_HELP        Display some minimal help
 *         IDM_ABOUT       Display title, version and author information in
 *                                   an animated dialog box
 *         IDM_QUIT        For all frames, delete DC and delete BMP
 *
 ************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam)
{
 HDC hdc;
 PAINTSTRUCT ps;
 HMENU hmenu;
 HANDLE hInstance;

 hInstance = (HANDLE) GetWindowLong (hWnd,GWL_HINSTANCE);
 hmenu=GetMenu(hWnd);

 switch (messg) {

  case WM_CREATE:
   {
    hToDC=GetDC(hWnd);
        ObjCount=0;
        ObjActive=FALSE;
        CheckMenuItem(hmenu,IDM_STOP,MF_CHECKED|MF_BYCOMMAND);
        Screen_OK=FALSE;                    /* assume bad screen */
        SetWinText(hWnd);
        LoadBMP(hInstance);
    /*DrawText(hToDC,AboutText,-1,&ChildDrawRect,DT_CENTER|DT_CALCRECT);*/
    ChildDrawRect.left=0;
    ChildDrawRect.top=0;
    ChildDrawRect.right=144+MaxBmpX*2;
        ChildDrawRect.bottom=128+MaxBmpY*2;
   }
  break;                                    /* WM_CREATE                    */

  case WM_SIZE:
   {
        iScreenX=LOWORD(lParam);            /* save screen X                */
        iScreenY=HIWORD(lParam);            /* save screen Y                */
        InvalidateRect(hWnd,NULL,TRUE);
   }
  break;                                    /* WM_SIZE                      */

  case WM_MOVE:
   {
    InvalidateRect(hWnd,NULL,TRUE);
   }
  break;                                    /* WM_MOVE                      */

  case WM_DESTROY:
   {
        PostQuitMessage(0);
   }
  break;                                    /* WM_DESTROY                   */

  case WM_PAINT:
   {
        hdc = BeginPaint(hWnd,&ps);
        Screen_OK=FALSE;                    /* fix up redraws               */
        MoveObject(hWnd,FALSE);
        EndPaint(hWnd,&ps);
   }
  break;                                    /* WM_PAINT                     */

  case WM_COMMAND:
   switch (LOWORD(wParam))
    {
     case IDM_START:
          {
           CheckMenuItem(hmenu,IDM_START,MF_CHECKED|MF_BYCOMMAND);
           CheckMenuItem(hmenu,IDM_STOP,MF_UNCHECKED|MF_BYCOMMAND);
           ObjActive=TRUE;
          }
         break;                             /* IDM_START                    */

         case IDM_STOP:
          {
           CheckMenuItem(hmenu,IDM_START,MF_UNCHECKED|MF_BYCOMMAND);
           CheckMenuItem(hmenu,IDM_STOP,MF_CHECKED|MF_BYCOMMAND);
           ObjActive=FALSE;
          }
         break;

         case IDM_DELOBJ:
          {
           if (ObjCount>0)
            {
             ObjArray[ObjCount].ObjStatus=BIT_DELOBJ;  /* tag for delete    */
                 MoveObject(hWnd,FALSE);
                 SetWinText(hWnd);
            }                               /* if any objects exist         */
          }
         break;                             /* IDM_DELOBJ                   */

         case IDM_ADDOBJ:
          {
           if (ObjCount+1<=MAX_OBJ)
            {
                 ObjCount++;
                 NewObject(ObjCount);       /* make the new object          */
                 MoveObject(hWnd,FALSE);
                 SetWinText(hWnd);
                }                           /* if below add limit           */
          }
         break;                             /* IDM_ADDOBJ                   */

         case IDM_MINOBJ:
          {
           int TempCtr;
           for (TempCtr=1;TempCtr<=ObjCount;TempCtr++)
            {
         ObjArray[TempCtr].ObjStatus=BIT_DELOBJ;  /* tag for delete         */
            }                                     /* if object exist        */
           MoveObject(hWnd,FALSE);                /* force redraw           */
           SetWinText(hWnd);
          }
         break;                             /* IDM_MINOBJ                   */

         case IDM_MAXOBJ:
          {
           while (ObjCount+1<=MAX_OBJ)
           {
            ObjCount++;
            NewObject(ObjCount);            /* make the object              */
           }
           MoveObject(hWnd,FALSE);          /* force redraw                 */
           SetWinText(hWnd);
          }
         break;                             /* IDM_ADDOBJ                   */

    case IDM_STEP:
         {
          MoveObject(hWnd,TRUE);
         }
        break;                              /* IDM_STEP                     */

        case IDM_HELP:
         {
          MessageBox(hWnd,"Click on 'Main' then 'Start', then 'Add Toy' to "
                     "add additional toys to the screen or 'Delete Toy' "
                     "to remove toys.",
                     "Help for Toy Box Bit Blit Demo",
                      MB_ICONINFORMATION | MB_OK);
         }
        break;                              /* IDM_HELP                      */

        case IDM_QUIT:
         {
          int Type;
          int Frame;
           for (Type=0;Type<MAX_OBJ_TYPE;Type++)
            {
                 for (Frame=0;Frame<Pix[Type].Frame;Frame++)
                  {
                   DeleteDC(PixDC[Type][Frame]);
                   DeleteObject(hObjBMP[Type][Frame]);
                  }
            }
           DestroyWindow(hWnd);
         }                                  /* IDM_QUIT                     */
        break;

        case IDM_ABOUT:
         {
          if (!hAbout)                      /* only allow one               */
           {
        hAbout= CreateWindow("AboutClass",
                 "About Toy Box",
                 WS_POPUP|WS_VISIBLE|WS_CAPTION|WS_BORDER,
                 CW_USEDEFAULT,
                 CW_USEDEFAULT,
                 ChildDrawRect.right,       /* so text fits                 */
                 ChildDrawRect.bottom,      /* for text                     */
                 hWnd,
                 NULL,
                 hInstance,
                 NULL);
       CheckMenuItem(hmenu,IDM_ABOUT,MF_CHECKED|MF_BYCOMMAND);
          }
          else
          {
           DestroyWindow(hAbout);
       CheckMenuItem(hmenu,IDM_ABOUT,MF_UNCHECKED|MF_BYCOMMAND);
          }
         }
        break;                              /* IDM_ABOUT                    */

        default:
        break;
   }                                        /* WM_COMMAND                   */
   break;
   default:
   return(DefWindowProc(hWnd,messg,wParam,lParam));
  }                                         /* end Switch                   */
 return(0L);
}                                           /* end WndProc                  */

/*************************************************************************
 * This moves all of the objects displayed on the screen. First it erases
 * any object already on the screen, then if the objects are to be moved
 * it moves them and makes sure that they do not overdraw the edge of the
 * screen, then it draws them back on the screen. Finally, if any of them
 * are marked for delete, it deletes them.
 *
 * Note: If DoMoveObject flag is FALSE, we just re-draw the objects,
 *       otherwise we move and redraw the objects.
 ************************************************************************/
void MoveObject(HWND InhWnd, int DoMoveObject)
{
 int TempCtr;
 int Tx,Ty;
 int TFrame;
 int TType;
 int tWX,tWY;
 int DelObjCount=0;

 if (Screen_OK==FALSE)                      /* if bad screen, erase */
  {
   SelectObject(hToDC,GetStockObject(BLACK_BRUSH));
   Rectangle(hToDC,0,0,iScreenX,iScreenY);
  }
 for (TempCtr=1;TempCtr<=ObjCount;TempCtr++)
  {
   Tx=ObjArray[TempCtr].ObjX;
   Ty=ObjArray[TempCtr].ObjY;
   tWX=ObjArray[TempCtr].ObjWX;
   tWY=ObjArray[TempCtr].ObjWY;
   TFrame=ObjArray[TempCtr].ObjFrame;
   TType=ObjArray[TempCtr].ObjType;
   if (ObjArray[TempCtr].ObjStatus==BIT_NEWOBJ)
    {
     ObjArray[TempCtr].ObjStatus=BIT_ACTOBJ; /* make object active      */
    }
   else
        {
         if (Screen_OK==TRUE)               /* erase if scrren OK       */
         BitBlt(hToDC,Tx,Ty,tWX,tWY,PixDC[TType][TFrame],0,0,SRCINVERT);
        }
   if (DoMoveObject==TRUE)                  /* if move mode, move em    */
        {
         Tx+=ObjArray[TempCtr].ObjDX;
         Ty+=ObjArray[TempCtr].ObjDY;
         ObjArray[TempCtr].SpinCnt++;       /* do frame animation       */
         if (ObjArray[TempCtr].SpinCnt>ObjArray[TempCtr].SpinWait)
         {
          ObjArray[TempCtr].SpinCnt=1;
          TFrame++;
          if (TFrame>=Pix[TType].Frame)
          {
           TFrame=0;
          }
          ObjArray[TempCtr].ObjFrame=TFrame;
         }
         if ((Tx+tWX)>iScreenX)             /* off screen right ?       */
          {
           Tx=iScreenX-tWX;
           ObjArray[TempCtr].ObjDX=-iRand_Num(1,Pix[TType].MDX);
          }                                 /* if Tx+Delta > Screen     */
         else
         if (Tx<0)                          /* of screen left ?         */
          {
       Tx=0;
           ObjArray[TempCtr].ObjDX=iRand_Num(1,Pix[TType].MDX);
          }                                 /* Tx < Screen              */
         if ((Ty+tWY)>iScreenY)             /* off screen bottom        */
          {
           Ty=iScreenY-tWY;
           ObjArray[TempCtr].ObjDY=-iRand_Num(1,Pix[TType].MDY);
          }                                 /* if Ty+Delta > Screen     */
         else
         if (Ty<0)                          /* off top of screen        */
          {
       Ty=0;
           ObjArray[TempCtr].ObjDY=iRand_Num(1,Pix[TType].MDY);
          }                                 /* Tx < Screen              */
         ObjArray[TempCtr].ObjX=Tx;
         ObjArray[TempCtr].ObjY=Ty;
         }                                  /* if DoMoveObject          */
         if (ObjArray[TempCtr].ObjStatus==BIT_DELOBJ)
          {
           DelObjCount++;                          /* we delete later    */
           ObjArray[TempCtr].ObjStatus=BIT_NONOBJ; /* mark unused        */
          }                                        /* if deleting object */
         else
          {
           BitBlt(hToDC,Tx,Ty,tWX,tWY,PixDC[TType][TFrame],0,0,SRCINVERT);
          }                                 /* if we can see object         */
   }                                        /* end TempCtr                  */
 ObjCount-=DelObjCount;                     /* remove the objects           */
 Screen_OK=TRUE;                            /* Objects are now OK           */
}                                           /* end MoveObject               */

/*************************************************************************
 *
 * This routine creats a new object and places it somewhere on the
 * client area. The object gets a random delta in X and Y directions
 * and a random type and starting frame number.
 * We don't care about the object possibly being off the screen, since it
 * will be fixed up when it is moved.
 *
 ************************************************************************/
void NewObject(int ObjIndex)
{
 int TempType;
 TempType=iRand_Num(1,MAX_OBJ_TYPE)-1;
 ObjArray[ObjIndex].ObjX=iRand_Num(1,iScreenX);
 ObjArray[ObjIndex].ObjY=iRand_Num(1,iScreenY);
 ObjArray[ObjIndex].ObjType=TempType;
 ObjArray[ObjIndex].ObjFrame=iRand_Num(1,Pix[TempType].Frame)-1;
 ObjArray[ObjIndex].SpinCnt=1;
 ObjArray[ObjIndex].SpinWait=Pix[TempType].Speed;
 ObjArray[ObjIndex].ObjWX=Pix[TempType].X;
 ObjArray[ObjIndex].ObjWY=Pix[TempType].Y;
 ObjArray[ObjIndex].ObjDX=iRand_Range(0,Pix[TempType].MDX*2,
    Pix[TempType].MDX,Pix[TempType].MDX)-Pix[TempType].MDX;
 ObjArray[ObjIndex].ObjDY=iRand_Range(0,Pix[TempType].MDY*2,
    Pix[TempType].MDY,Pix[TempType].MDY)-Pix[TempType].MDY;
 ObjArray[ObjIndex].ObjStatus=BIT_NEWOBJ;
}                                           /* end NewObject                 */

/*************************************************************************
 *
 * This routine sets the text displayed in the title bar of the InhWnd
 * equal to the predefined string TitleText and the number of objects
 * currently on the screen.
 *
 ************************************************************************/
void SetWinText(HWND InhWnd)
{
 char TempBuff[32];                         /* temporary                    */
 char NewWinText[128];                      /* new window title             */

 strcpy(NewWinText,TitleText);              /* get default text             */
 wsprintf(TempBuff,"%i",ObjCount);          /* format object count          */
 strcat(NewWinText,TempBuff);               /* add it to title              */
 strcat(NewWinText,"]");                    /* add ending character         */
 SetWindowText(InhWnd,NewWinText);          /* set the text                 */
}                                           /* SetWinText                   */

/*************************************************************************
 *
 * This routine generates a random number that is between (and including)
 * the lower_limit and the upper_limit.
 *
 * For example, to generate a number inclusive to +5 and +15:
 *
 *  x=iRand_Num(5,15);
 *
 ************************************************************************/
int iRand_Num(int lower_limit, int upper_limit)
{
 int i;
 i=(rand()%(upper_limit-lower_limit+1))+lower_limit;
 return (i);
}                                           /* end iRand_Num                */

/*************************************************************************
 *
 * This routine generates a random number that is between (and including)
 * the lower_limit and the upper_limit, and does NOT contain any number
 * between (and including) the low_excld and the up_excld.
 *
 * For example, to generate a number between -5 and +5 and not allow zero:
 *
 *  x=iRand_Range(-5,5,0,0);
 *
 ************************************************************************/
int iRand_Range(int lower_limit, int upper_limit, int low_excld, int up_excld)
{
 int i;
 retry:
 i=(rand()%(upper_limit-lower_limit+1))+lower_limit;
 if (i>=low_excld && i<=up_excld)           /* is it between limits         */
  { goto retry;}                            /* nope, try again              */
 return (i);
}                                           /* end iRand_Range              */

/*************************************************************************
 *
 * This routine just loads in all of the bitmaps used for this program
 * and sets all of the information needed for each group of objects
 * (the Pix[].xxx data fields).
 *
 * The really important step is done at the end of this routine, which is
 * to create a DC for each frame of each object group and then select
 * into the DC the bitmap. This step, done during initial program startup
 * really speeds up the display of the bitmaps. The CreateCompatibleDC
 * and the SelectObject into the DC is done once, not like would be needed
 * if this was not done.
 *
 ************************************************************************/
void LoadBMP(HANDLE InInstance)
{
 int Type;                                   /* type of object              */
 int Frame;                                  /* animation frame             */

/*************************************************************************
 * Load in the first group of object bitmaps and then set relevant data.
 ************************************************************************/

  hObjBMP[0][0]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallR1));
  hObjBMP[0][1]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallR2));
  hObjBMP[0][2]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallR3));
  hObjBMP[0][3]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallR4));
  hObjBMP[0][4]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallR5));
  hObjBMP[0][5]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallR6));
  Pix[0].X=23;                              /* Bitmap X                     */
  Pix[0].Y=23;                              /* Bitmap Y                     */
  Pix[0].Frame=6;                           /* # of animate frames          */
  Pix[0].Speed=2;                           /* wait time to animate         */
  Pix[0].MDX=4;                             /* Max Delta X                  */
  Pix[0].MDY=4;                             /* Max Delta Y                  */

/*************************************************************************
 * Load in the second group of object bitmaps and then set relevant data.
 ************************************************************************/

  hObjBMP[1][0]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallB1));
  hObjBMP[1][1]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallB2));
  hObjBMP[1][2]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallB3));
  hObjBMP[1][3]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallB4));
  Pix[1].X=15;                              /* Bitmap X                     */
  Pix[1].Y=15;                              /* Bitmap Y                     */
  Pix[1].Frame=4;                           /* # of animate frames          */
  Pix[1].Speed=3;                           /* wait time to animate         */
  Pix[1].MDX=3;                             /* Max Delta X                  */
  Pix[1].MDY=3;                             /* Max Delta Y                  */

/*************************************************************************
 * Load in the third group of object bitmaps and then set relevant data.
 ************************************************************************/

  hObjBMP[2][0]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallY1));
  hObjBMP[2][1]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallY2));
  hObjBMP[2][2]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallY3));
  hObjBMP[2][3]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallY4));
  hObjBMP[2][4]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallY5));
  hObjBMP[2][5]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallY6));
  Pix[2].X=15;                              /* Bitmap X                     */
  Pix[2].Y=15;                              /* Bitmap Y                     */
  Pix[2].Frame=6;                           /* # of animate frames          */
  Pix[2].Speed=5;                           /* wait time to animate         */
  Pix[2].MDX=3;                             /* Max Delta X                  */
  Pix[2].MDY=3;                             /* Max Delta Y                  */

/*************************************************************************
 * Load in the fourth group of object bitmaps and then set relevant data.
 ************************************************************************/

  hObjBMP[3][0]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallG1));
  hObjBMP[3][1]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallG2));
  hObjBMP[3][2]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallG3));
  hObjBMP[3][3]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallG4));
  hObjBMP[3][4]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallG5));
  hObjBMP[3][5]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallG6));
  Pix[3].X=27;                              /* Bitmap X                     */
  Pix[3].Y=13;                              /* Bitmap Y                     */
  Pix[3].Frame=6;                           /* # of animate frames          */
  Pix[3].Speed=5;                           /* wait time to animate         */
  Pix[3].MDX=4;                             /* Max Delta X                  */
  Pix[3].MDY=2;                             /* Max Delta Y                  */

/*************************************************************************
 * Load in fifth group of object bitmaps and then set relevant data.
 ************************************************************************/

  hObjBMP[4][0]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallL1));
  hObjBMP[4][1]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallL2));
  hObjBMP[4][2]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallL3));
  hObjBMP[4][3]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallL4));
  Pix[4].X=16;                              /* Bitmap X                     */
  Pix[4].Y=8;                               /* Bitmap Y                     */
  Pix[4].Frame=4;                           /* # of animate frames          */
  Pix[4].Speed=4;                           /* wait time to animate         */
  Pix[4].MDX=3;                             /* Max Delta X                  */
  Pix[4].MDY=3;                             /* Max Delta Y                  */

/*************************************************************************
 * Load in the sixth group of object bitmaps and then set relevant data.
 ************************************************************************/

  hObjBMP[5][0]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallP1));
  hObjBMP[5][1]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallP2));
  hObjBMP[5][2]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallP3));
  hObjBMP[5][3]=LoadBitmap (InInstance,MAKEINTRESOURCE(BallP4));
  Pix[5].X=8;                               /* Bitmap X                     */
  Pix[5].Y=16;                              /* Bitmap Y                     */
  Pix[5].Frame=4;                           /* # of animate frames          */
  Pix[5].Speed=4;                           /* wait time to animate         */
  Pix[5].MDX=2;                             /* Max Delta X                  */
  Pix[5].MDY=3;                             /* Max Delta Y                  */

/*************************************************************************
 * Now we loop thru all object groups and for each frame of the animation
 * we create a Device Context (DC) and into that DC we select the current
 * animation and save it for faster access into the PixDC array.
 ************************************************************************/

for (Type=0;Type<MAX_OBJ_TYPE;Type++)           /* do for all groups    */
 {
 for (Frame=0;Frame<Pix[Type].Frame;Frame++)    /* for all valid frames */
  {
   PixDC[Type][Frame]=CreateCompatibleDC(hToDC);
   SelectObject(PixDC[Type][Frame],hObjBMP[Type][Frame]);
  }
 MaxBmpX=((Pix[Type].X>MaxBmpX)?Pix[Type].X:MaxBmpX);
 MaxBmpY=((Pix[Type].Y>MaxBmpY)?Pix[Type].Y:MaxBmpY);
 RollObj[Type].ObjWX=Pix[Type].X;
 RollObj[Type].ObjWY=Pix[Type].Y;
 RollObj[Type].ObjType=Type;
 RollObj[Type].ObjFrame=1;
 RollObj[Type].SpinCnt=1;
 RollObj[Type].SpinWait=Pix[Type].Speed;
 }
}                                               /* end LoadBMP          */

/*************************************************************************
 *
 * This routine does the special processing needed to display the ABOUT
 * Toy Box animated about box. The box displays a centered multi-line
 * about box with the six defined 'Toys' located three across the top
 * and three across the bottom, centered (relative to the largest one)
 * and animated.
 *
 *      WM_CREATE               Create a timer for the animation, and get a DC
 *      WM_MOVE                 Just force a re-draw
 *      WM_SIZE                 Compute positions of animation and force re-draw
 *      WM_PAINT                Erase the screen and draw the text onto the window
 *      WM_TIMER                Update the animation of the balls
 *      WM_QUIT         Delete the timer and reset hAbout to null
 *
 ************************************************************************/
LRESULT CALLBACK AboutProc(HWND hChild,UINT messg,WPARAM wParam,LPARAM lParam)
{
 static HDC TheDC;
 PAINTSTRUCT ps;
 static int ChildTimer;
 static int ChildX,ChildY;

 switch (messg) {

  case WM_CREATE:
   {
    ChildTimer=SetTimer(hChild,2,50,NULL);
    TheDC=GetDC(hChild);
   }                                        /* WM_CREATE                    */
  break;

  case WM_MOVE:
   {
        InvalidateRect(hChild,NULL,TRUE);
   }
  break;

  case WM_SIZE:
   {
    int TempSpace;
        ChildX=LOWORD(lParam);             /* save screen X                */
        ChildY=HIWORD(lParam);             /* save screen Y                */
        TempSpace=(ChildX-(MAX_OBJ_TYPE/2)*MaxBmpX)/(MAX_OBJ_TYPE/2);
         RollObj[0].ObjX=6+((MaxBmpX-RollObj[0].ObjWX)/2);
         RollObj[0].ObjY=6+((MaxBmpY-RollObj[0].ObjWY)/2);

         RollObj[1].ObjX=(ChildX-MaxBmpX)/2+((MaxBmpX-RollObj[1].ObjWX)/2);
         RollObj[1].ObjY=6+((MaxBmpY-RollObj[1].ObjWY)/2);

         RollObj[2].ObjX=ChildX-MaxBmpX-6+((MaxBmpX-RollObj[2].ObjWX)/2);
         RollObj[2].ObjY=6+((MaxBmpY-RollObj[2].ObjWY)/2);

         RollObj[3].ObjX=6+((MaxBmpX-RollObj[3].ObjWX)/2);
         RollObj[3].ObjY=ChildY-MaxBmpY-6+((MaxBmpY-RollObj[3].ObjWY)/2);

         RollObj[4].ObjX=(ChildX-MaxBmpX)/2+((MaxBmpX-RollObj[4].ObjWX)/2);
         RollObj[4].ObjY=ChildY-MaxBmpY-6+((MaxBmpY-RollObj[4].ObjWY)/2);

         RollObj[5].ObjX=ChildX-MaxBmpX-6+((MaxBmpX-RollObj[5].ObjWX)/2);
         RollObj[5].ObjY=ChildY-MaxBmpY-6+((MaxBmpY-RollObj[5].ObjWY)/2);

        InvalidateRect(hChild,NULL,TRUE);
   }
  break;                                    /* WM_SIZE                      */

  case WM_PAINT:
   {
    BeginPaint(hChild,&ps);
    SetTextColor(TheDC,RGB(255,0,255));
        SetBkColor(TheDC,RGB(0,0,0));
        PatBlt(TheDC,0,0,ChildX,ChildY,BLACKNESS);
        DrawText(TheDC,AboutText,-1,&ChildDrawRect,DT_CENTER);
        EndPaint(hChild,&ps);
   }
  break;                                    /* WM_PAINT                     */

  case WM_TIMER:
   {
    AnimateObjects(TheDC);
   }
  break;                                    /* WM_TIMER                     */

  case WM_DESTROY:
   {
    if (ChildTimer!=0)
     KillTimer(hChild,ChildTimer);
        ReleaseDC(hChild,TheDC);
        hAbout=NULL;                        /* allow to make again          */
   }
  break;                                    /* WM_DESTROY                   */

  default:
  return(DefWindowProc(hChild,messg,wParam,lParam));
  }                                         /* end Switch                   */
 return(0L);
}                                           /* end WndProc                  */

/*************************************************************************
 *
 * This routine takes each object that is displayed in the about
 * box and animates it. For each object, it PatBlts the object to
 * black, then updates the object frame and BitBlts the object back
 * on the screen.
 *
 ************************************************************************/
void AnimateObjects(HDC hToDC)
{
 int TempCtr;
 int Tx,Ty;
 int TFrame;
 int TType;
 int tWX,tWY;

 for (TempCtr=0;TempCtr<MAX_OBJ_TYPE;TempCtr++)
  {
   Tx=RollObj[TempCtr].ObjX;
   Ty=RollObj[TempCtr].ObjY;
   tWX=RollObj[TempCtr].ObjWX;
   tWY=RollObj[TempCtr].ObjWY;
   TFrame=RollObj[TempCtr].ObjFrame;
   TType=RollObj[TempCtr].ObjType;
   PatBlt(hToDC,Tx,Ty,tWX,tWY,BLACKNESS);
   RollObj[TempCtr].SpinCnt++;              /* do frame animation   */
         if (RollObj[TempCtr].SpinCnt>RollObj[TempCtr].SpinWait)
         {
          RollObj[TempCtr].SpinCnt=1;
          TFrame++;
          if (TFrame>=Pix[TType].Frame)
          {
           TFrame=0;
          }
          RollObj[TempCtr].ObjFrame=TFrame;
         }
           BitBlt(hToDC,Tx,Ty,tWX,tWY,PixDC[TType][TFrame],0,0,SRCCOPY);
   }                                        /* end TempCtr                  */
}                                           /* end AnimateObjects           */
