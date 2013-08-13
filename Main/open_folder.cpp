#define INCL_WINMESSAGEMGR
#define INCL_WININPUT
#define INCL_WINFRAMEMGR
#include <os2.h>
#include "open_folder.h"

/** Open folder in container.
	All messages may not be necesary, but this is what PMSPY says...
	@param hwndCnr	Handle of container
	@param pptsMouse Pointer to POINTS struct containing mouse position
	@return BOOL Sucess indicator
*/
BOOL	OpenObject( HWND hwndCnr, POINTS*	pptsMouse )
{
	BOOL fRet;

	fRet = WinPostMsg(	hwndCnr,
								WM_BUTTON1DOWN,
								MPFROM2SHORT( (*pptsMouse).x, (*pptsMouse).y ),  	// Mouse pos.
								MPFROM2SHORT( HT_NORMAL, KC_NONE ) );					// Hittest, keyb.code.

	fRet = WinSendMsg(	hwndCnr,
								WM_FOCUSCHANGE,
								MPFROMLONG( hwndCnr ),			// new focus
								MPFROM2SHORT( TRUE, 0 ) );		// recv. focus?,

	fRet = WinSendMsg(	hwndCnr,
								WM_ACTIVATE,
								MPFROMSHORT( TRUE ),  			// Activate ?
								MPFROMLONG( hwndCnr ) );		// Handle

	fRet = WinSendMsg(	hwndCnr,
								WM_SETSELECTION,
								MPFROMSHORT( TRUE ),				// Selected ?
								0 );									// reserved

	fRet = WinSendMsg(	hwndCnr,
								WM_SETFOCUS,
								MPFROMLONG( hwndCnr ),			// Handle
								MPFROMSHORT( TRUE ) );			// Recv. focus?
								
	fRet = WinPostMsg(	hwndCnr,
								WM_BUTTON1UP,
								MPFROM2SHORT( (*pptsMouse).x, (*pptsMouse).y ), // Mouse pos.
								MPFROM2SHORT( HT_NORMAL, KC_NONE ) );				// hittest, keyb.code

	fRet = WinPostMsg(	hwndCnr,
								WM_BUTTON1CLICK,
								MPFROM2SHORT( (*pptsMouse).x, (*pptsMouse).y ), // Mouse pos.
								MPFROM2SHORT( HT_NORMAL, KC_NONE ) );				// hittest, keyb.code

	fRet = WinPostMsg(	hwndCnr,
								WM_SINGLESELECT,
								MPFROM2SHORT( (*pptsMouse).x, (*pptsMouse).y ),	// Mouse pos.
								MPFROMSHORT( TRUE ) );									// Selected ?
	
	fRet = WinPostMsg(	hwndCnr,
								WM_BUTTON1DBLCLK,
								MPFROM2SHORT( (*pptsMouse).x, (*pptsMouse).y ), // Mouse pos.
								MPFROM2SHORT( HT_NORMAL, KC_NONE ) );				// hittest, keyb.code

	fRet = WinPostMsg(	hwndCnr,
								WM_OPEN,
								MPFROMSHORT( TRUE ),											// From mouse?
								MPFROM2SHORT( (*pptsMouse).x, (*pptsMouse).y ) );	// Mouse pos.
	fRet = WinPostMsg(	hwndCnr,
								WM_BUTTON1UP,
								MPFROM2SHORT( (*pptsMouse).x, (*pptsMouse).y ), // Mouse pos.
								MPFROM2SHORT( HT_NORMAL, KC_NONE ) );				// hittest, keyb.code
					
	return fRet;
};


