//-----------------------------------------------------------------------------
// This file includes the implementation for the DLL containing the hook for
// the freeware version of eM Bee 3 .
//
// Included features:
//    * If 'clicked window' is of class #37 ( container ) --> convert msg to
//      button 1 double click
//    * All other - show window list w/animation
//-----------------------------------------------------------------------------

#define INCL_WIN
#define INCL_WINWINDOWMGR
#define INCL_GPIPRIMITIVES
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_WINFRAMEMGR
#define INCL_WINATOM

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define DLL_SOURCE
#include "..\common.h"

/**@name DLL constructors: */
//@{
int  _CRT_init( void );         // C-Runtime Init
void _CRT_term( void );         // C-Runtime terminate
void __ctordtorInit( void );
void __ctordtorTerm( void );
//@}

/**@name Global variables in DLL */
//@{
/// The handle to MB3FREE.DLL
HMODULE 	hmSelf;
/// Handle of window list
HWND		hwndUserDlg;
HAB		habeMBee;
POINTS	ptsMouse;
HATOMTBL	hatomtblSystem;
GLOBAL	globValues;
BOOL		fActivated;
//@}

GLOBAL* EXPENTRY GetGloabls()
{
	return &globValues;
};


unsigned long _System _DLL_InitTerm( unsigned long hmModule, unsigned long ulFlag )
//----------------------------------------------------------------------------
// Purpose: Initialize the C runtime system
//----------------------------------------------------------------------------
{
   hmSelf = hmModule;

   if( ulFlag == 0 ) {
      if( _CRT_init() != 0 )
         return 0;

       __ctordtorInit();
       globValues.ulSz = sizeof( GLOBAL );
		 globValues.fActivated = FALSE;
       return 1;
   } else {
      __ctordtorTerm();
      _CRT_term();
      return 1;
   }
};


BOOL EXPENTRY eMBee3Hook( HAB habA, PQMSG pqmsgM, ULONG fs )
//-----------------------------------------------------------------------------
// Description: the eMBee "Work horse"
// Purpose    : Examine input queue and act upon WM_BUTTON3CLICK messages
//-----------------------------------------------------------------------------
{
   char     strN[ 30 ];
   BOOL		fR;

   if( pqmsgM->msg == WM_BUTTON3CLICK ) {
      WinQueryClassName( pqmsgM->hwnd, 29, ( unsigned char * )strN );

		ptsMouse.x = SHORT1FROMMP( pqmsgM->mp1 );
		ptsMouse.y = SHORT2FROMMP( pqmsgM->mp1 );

		if( strN[ 1 ] == '3' && strN[ 2 ] == '7'  ) {        		// If 'clicked window = container
      	fR = WinPostMsg(	hwndUserDlg,
      							globValues.wmu_DoubleClick,
									MPFROMLONG( pqmsgM->hwnd ),
									MPFROM2SHORT( ptsMouse.x, ptsMouse.y ) );
			return TRUE;      // Processed
       } else {                                   // else: show tasklist
				fR = WinPostMsg(	hwndUserDlg,
										globValues.wmu_ShowWinList,
										MPFROMLONG( 0L ),
										MPFROMLONG( 0L ) );

		      return TRUE;      // Processed
      }
   } else {
     return FALSE;        // Not processed
   }
};


//อออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
/** Initiate hook
	@param hwndClient Handle to the control window
	@return BOOL	Success indicator
*/
BOOL EXPENTRY eMBeeInitHook( HWND hwndClient )
{
	APIRET	arR;
	hwndUserDlg = hwndClient;
	
	if( !globValues.fActivated ) {
		hatomtblSystem  = WinQuerySystemAtomTable();
		
		srand( (UINT)hwndClient );
		
		sprintf( globValues.pszShowWinList, "eMBeeShowWinList%d", rand() );
		sprintf( globValues.pszDoubleClick, "eMBeeDoubleClick%d", rand() );
		
		globValues.wmu_ShowWinList = WinAddAtom( hatomtblSystem, globValues.pszShowWinList );
		globValues.wmu_DoubleClick = WinAddAtom( hatomtblSystem, globValues.pszDoubleClick );
		
		arR = DosQueryModuleHandle( "MB3DLL", &hmSelf );
		
		habeMBee = WinQueryAnchorBlock( hwndClient );
		
		globValues.fActivated = fActivated =  WinSetHook( habeMBee, NULLHANDLE, HK_INPUT, (PFN)eMBee3Hook, hmSelf );
	}
	return globValues.fActivated ;
};
//อออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
/** Remove hook
*/
BOOL EXPENTRY	eMBeeHookOff()
{
	if( fActivated ) {
		WinReleaseHook( habeMBee, NULLHANDLE, HK_INPUT, (PFN)eMBee3Hook, hmSelf );
		
		WinDeleteAtom( hatomtblSystem, globValues.wmu_ShowWinList );
		WinDeleteAtom( hatomtblSystem, globValues.wmu_DoubleClick );
		
		globValues.fActivated = fActivated = FALSE;
	}
	
	return TRUE;
};

