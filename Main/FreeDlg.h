#ifndef __FREEDLG_H
#define __FREEDLG_H

#define APP_NAME		"eMBee jr."
#define KEY_ANIM		"Do animate winlist"
#define KEY_WIN		"Windos position"

MRESULT EXPENTRY FreeDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpOne, MPARAM mpTwo );
MRESULT EXPENTRY SplashProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpOne, MPARAM mpTwo );

void Paint( HWND hwnd, POINTL* pptlWhere, ULONG ulWidth, ULONG ulHeight, HBITMAP hbmImg1, HBITMAP hbmMask1, RECTL* prclI );

#endif
