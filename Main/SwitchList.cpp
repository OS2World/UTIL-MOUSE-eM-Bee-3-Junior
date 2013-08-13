#define INCL_WINWINDOWMGR
#define INCL_WINSYS
#define INCL_WINPOINTERS
#define INCL_WINFRAMEMGR
#include <os2.h>
#include "ErrorDlg.h"

HWND hwndWinList;

/** Calculate new position of window list
	@param lWidth Window list's width
	@param lHeight Window list's heigth
	@param pptlMouse Mouse position
	@param lWinX Output parameter: Window list's new x pos.
	@param lWinY Output parameter: Window list's new y pos.
*/
void CalcWinListPos( LONG lWidth, LONG lHeight, POINTL* pptlMouse, LONG &lWinX, LONG &lWinY )
{
	RECTL rclScreen;
	
	WinQueryWindowRect( HWND_DESKTOP, &rclScreen );
	
	if( pptlMouse->x < lWidth/2 )
	// Too far left
	{
		lWinX = 0;
		// check to see that it doesn't get off screen
		if( lHeight > pptlMouse->y )
			lWinY = 0;
		else
			lWinY = pptlMouse->y - lHeight + WinQuerySysValue( HWND_DESKTOP, SV_CYTITLEBAR );
	} else
		if( ( rclScreen.xRight - pptlMouse->x ) < lWidth/2 )
		// Too far right
		{
			lWinX = rclScreen.xRight - lWidth;
			// check to see that it doesn't get off screen
			if( lHeight > pptlMouse->y )
				lWinY = 0;		
			else
				lWinY = pptlMouse->y - lHeight + WinQuerySysValue( HWND_DESKTOP, SV_CYTITLEBAR );
		} else
			if( lHeight > pptlMouse->y )
			// Too far down
			{
				lWinY = 0;		
				lWinX = pptlMouse->x - lWidth/2;
			} else {
				lWinX = pptlMouse->x - lWidth/2;
				lWinY = pptlMouse->y - lHeight + WinQuerySysValue( HWND_DESKTOP, SV_CYTITLEBAR );
			}
};

HWND FindSwitch( )
{
   HENUM heWins;
   HWND hwndList;
   char pchN[ 30 ];

   heWins = WinBeginEnumWindows( HWND_DESKTOP );

   while( ( hwndList = WinGetNextWindow( heWins ) ) != NULLHANDLE ) {
      WinQueryClassName( ( long unsigned int )WinWindowFromID( hwndList, FID_CLIENT ),
                         29,
                         ( unsigned char * )pchN );
      if( !strcmp( pchN , "WindowList" ) )
         break;
   }
   WinEndEnumWindows( heWins );

   return hwndList;
};

void AnimateShow( SWP* pswpW )
{
   register int i;
	USHORT	usStep = pswpW->cy/5;

  WinSetWindowPos(  hwndWinList,
                    HWND_BOTTOM,
                    pswpW->x ,
                    pswpW->y + pswpW->cy - WinQuerySysValue( HWND_DESKTOP, SV_CYTITLEBAR )-10,
                    pswpW->cx,
                    1,
                    SWP_MOVE | SWP_HIDE | SWP_SIZE );

   // Now animate in 5 steps
	for( i = 1; i < 6; i++ ) {
		WinSetWindowPos(  hwndWinList,
								HWND_TOP,
                        pswpW->x,
                        pswpW->y + ( pswpW->cy - usStep * i ),
                        pswpW->cx,
                        usStep * i,
                        SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE );
   }

   WinSetActiveWindow( HWND_DESKTOP, hwndWinList );
};

/// Show window list
void ShowWinList( BOOL fAnimate ) {
	SWP		swpWinList;
	ULONG		ulWidth, ulHeight;
	POINTL   ptlMouse;
	
	if( hwndWinList == NULLHANDLE )
		hwndWinList = FindSwitch();
	// First query switchlist's current size
	WinQueryWindowPos( hwndWinList, &swpWinList );
	
	// Query the mouse position
	WinQueryPointerPos( HWND_DESKTOP, &ptlMouse );
	
	// Calculate new pos.
	CalcWinListPos( swpWinList.cx, swpWinList.cy, &ptlMouse , swpWinList.x, swpWinList.y );

	if( fAnimate ) {
		AnimateShow( &swpWinList );
	} else {	
		// Activate window
		WinSetWindowPos(	hwndWinList,
								HWND_TOP,
								swpWinList.x,
								swpWinList.y,
								swpWinList.cx,
								swpWinList.cy,
								swpWinList.fl );
		WinSetActiveWindow( HWND_DESKTOP, hwndWinList );	
	}
};


