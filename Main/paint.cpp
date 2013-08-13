#define INCL_WINSYS
#define INCL_WINWINDOWMGR
#define INCL_GPIBITMAPS

#include <os2.h>
#include <fstream.h>
#include "interface.h"

Paint( HWND hwnd , POINTL* pptlWhere, ULONG ulWidth, ULONG ulHeight, HBITMAP hbmImg1, HBITMAP hbmMask1, RECTL* prclI )
{
	HPS 		hps;
	LONG		lHits;
	POINTL	ptlBox[ 4 ];


	ptlBox[ 0 ].x = pptlWhere->x;
	ptlBox[ 0 ].y = pptlWhere->y;
	ptlBox[ 1 ].x = ulWidth + ptlBox[ 0 ].x;
	ptlBox[ 1 ].y = ulHeight+ ptlBox[ 0 ].y;
	ptlBox[ 2 ].x = 0;
	ptlBox[ 2 ].y = 0;
	ptlBox[ 3 ].x = ulWidth;
	ptlBox[ 3 ].y = ulHeight;
	
	hps = WinGetPS( hwnd );
	
	WinFillRect( hps, prclI, SYSCLR_DIALOGBACKGROUND );
	
	lHits = GpiWCBitBlt(	hps,
								hbmMask1,
								4L,
								ptlBox,
								ROP_SRCAND, BBO_IGNORE );
						
	lHits = GpiWCBitBlt( hps,
								hbmImg1,
								4L,
								ptlBox,
								ROP_SRCPAINT, BBO_IGNORE );
   		
	WinReleasePS( hps );
};
