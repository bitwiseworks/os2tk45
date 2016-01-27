

/*=================================================
Contains defines needed for api level interface with 
pgl. 
===================================================*/

#ifndef PGL_DEFINE
#define PGL_DEFINE


#if !defined (NT)
#ifndef OS2
#define OS2
#endif
#endif /* NT */

#ifdef __cplusplus
extern "C" {
#endif

#define INCL_DOSPROCESS
#define INCL_32
#define INCL_DEV
#define INCL_WIN
#define INCL_DOS
#define INCL_GRE_PALETTE
#define INCL_GPI
#define INCL_PM
#if defined (NT)
#include <windows.h>
#include <windef.h>
#include <wingdi.h>
#include <mapi.h>
#include "os22nt.h"
#else
#include <os2.h>
#endif /* NT */

#include <gl.h>

typedef struct visualconfig {
    unsigned long  vid;  /*Visual ID*/
    BOOL   rgba;
    int redSize, greenSize, blueSize, alphaSize;
    ULONG redMask, greenMask, blueMask;
    int accumRedSize, accumGreenSize, accumBlueSize, accumAlphaSize;
    BOOL doubleBuffer;
    BOOL  stereo;
    int bufferSize;
    int depthSize;
    int stencilSize;
    int auxBuffers;
    int level;
    PVOID reserved;
    struct visualconfig *next;
    } VISUALCONFIG, *PVISUALCONFIG;

typedef LHANDLE HGC;
	

#if defined (NT)
extern PVISUALCONFIG APIENTRY pglChooseConfig(HINSTANCE hInstance, int *attriblist);
extern PVISUALCONFIG* APIENTRY pglQueryConfigs(HINSTANCE hInstance);
extern HGC APIENTRY pglCreateContext(HINSTANCE hInstance, PVISUALCONFIG pVisualConfig, HGC Sharelist, BOOL IsDirect);
extern BOOL APIENTRY pglMakeCurrent(HINSTANCE hInstance,HGC hgc, HWND hwnd);
extern BOOL APIENTRY pglDestroyContext(HINSTANCE hInstance, HGC hgc);
extern BOOL APIENTRY pglCopyContext(HINSTANCE hInstance, HGC hgc_src, HGC hgc_dst, GLuint attrib_mask);
extern int APIENTRY pglSelectColorIndexPalette(HINSTANCE hInstance, HPALETTE hpal, HGC hgc);
extern LONG APIENTRY pglIsIndirect(HINSTANCE hInstance, HGC hgc);
extern HGC  APIENTRY pglGetCurrentContext(HINSTANCE hInstance);
extern HWND APIENTRY pglGetCurrentWindow(HINSTANCE hInstance);
extern HDC  APIENTRY pglWaitGL(HINSTANCE hInstance);
extern void APIENTRY pglWaitPM(HINSTANCE hInstance);
extern void APIENTRY pglSwapBuffers(HINSTANCE hInstance, HWND hwnd);
extern LONG APIENTRY pglQueryCapability(HINSTANCE hInstance);
extern void APIENTRY pglQueryVersion(HINSTANCE hInstance, int *major, int *minor);
extern BOOL APIENTRY pglUsePMBitmapFont(HINSTANCE hInstance, LONG id, int first, int count, int listbase);
extern BOOL APIENTRY pglGrabFrontBitmap(HINSTANCE hInstance,HDC *phps,HBITMAP *phbitmap);
extern BOOL APIENTRY pglReleaseFrontBitmap(HINSTANCE hInstance);
extern BOOL APIENTRY pglUseFont(HINSTANCE,HDC,LOGFONT *,long,int,int,int);
#else /* OS2 */
extern PVISUALCONFIG APIENTRY pglChooseConfig(HAB hab, int *attriblist);
extern PVISUALCONFIG* APIENTRY pglQueryConfigs(HAB hab);
extern HGC APIENTRY pglCreateContext(HAB hab, PVISUALCONFIG pVisualConfig, HGC Sharelist, BOOL IsDirect);
extern BOOL APIENTRY pglMakeCurrent(HAB hab,HGC hgc, HWND hwnd);
extern BOOL APIENTRY pglDestroyContext(HAB hab, HGC hgc);
extern BOOL APIENTRY pglCopyContext(HAB hab, HGC hgc_src, HGC hgc_dst, GLuint attrib_mask);
extern int APIENTRY pglSelectColorIndexPalette(HAB hab, HPAL hpal, HGC hgc);
extern LONG APIENTRY pglIsIndirect(HAB hab, HGC hgc);
extern HGC  APIENTRY pglGetCurrentContext(HAB hab);
extern HWND APIENTRY pglGetCurrentWindow(HAB hab);
extern HPS  APIENTRY pglWaitGL(HAB hab);
extern void APIENTRY pglWaitPM(HAB hab);
extern void APIENTRY pglSwapBuffers(HAB hab, HWND hwnd);
extern LONG APIENTRY pglQueryCapability(HAB hab);
extern void APIENTRY pglQueryVersion(HAB hab, int *major, int *minor);
extern BOOL APIENTRY pglUsePMBitmapFont(HAB hab, LONG id, int first, int count, int listbase);
extern BOOL APIENTRY pglGrabFrontBitmap(HAB hab,HPS *phps,HBITMAP *phbitmap);
extern BOOL APIENTRY pglReleaseFrontBitmap(HAB hab);
extern BOOL APIENTRY pglUseFont(HAB,HPS,FATTRS *,long,int,int,int);
#endif /* NT */

#define  None			0x0

#define  PGL_USE_GL   		1
#define  PGL_BUFFER_SIZE   	2
#define  PGL_LEVEL   		3
#define  PGL_RGBA   		4
#define  PGL_DOUBLEBUFFER   	5
#define  PGL_STEREO   		6
#define  PGL_AUX_BUFFERS   	7
#define  PGL_RED_SIZE   	8
#define  PGL_GREEN_SIZE   	9
#define  PGL_BLUE_SIZE   	10
#define  PGL_ALPHA_SIZE   	11
#define  PGL_DEPTH_SIZE   	12
#define  PGL_STENCIL_SIZE   	13
#define  PGL_ACCUM_RED_SIZE   	14
#define  PGL_ACCUM_GREEN_SIZE   15
#define  PGL_ACCUM_BLUE_SIZE   	16
#define  PGL_ACCUM_ALPHA_SIZE   17
#define  PGL_SINGLEBUFFER   	18

#ifdef __cplusplus
}
#endif

#endif
