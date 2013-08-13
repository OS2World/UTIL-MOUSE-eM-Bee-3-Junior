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

#include <os2.h>
#include <string.h>
#include <stdlib.h>
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
HMODULE hmSelf;
/// Handle of window list
HWND hwndUserDlg;
POINTS ptsMouse;

GLOBAL globValues;
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

       hwndSwitch = NULLHANDLE;
       ScreenSize.usWidth = ScreenSize.usHeight = 0;
       Atomics.fOK = FALSE;
       return 1;
   } else {
      __ctordtorTerm();
      _CRT_term();
      return 1;
   }
};


//-----------------------------------------------------------------------------
/// Purpose: find windowlist
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
      if( !strcmp( pchN , WINDOWLISTNAME ) )
         break;
   }
   WinEndEnumWindows( heWins );

   return hwndList;
};

//-----------------------------------------------------------------------------
// Purpose: Animate the switchlist
// In     : HWND of WindowList and a pointer to a POINTL struct containg the
//          mouse placement
// Out    : Nil
// What   : "Explode" windowlist from mouse's position. The size will be 1/4 of
//          screen's width and 1/3 of the height
//-----------------------------------------------------------------------------
void AnimateShow( HWND hwndL )
{
   register int i;
   POINTL   ptlMouse;

   if( !ScreenSize.usWidth ) {
      ScreenSize.usWidth  = WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );
      ScreenSize.usHeight = WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN );
      // and now set the max size for the window list
      ScreenSize.usWLWidth  = ScreenSize.usWidth/4;
      ScreenSize.usWLHeight = ScreenSize.usHeight/3;
      ScreenSize.usWL_WBits = ScreenSize.usWLWidth/5;
      ScreenSize.usWL_HBits = ScreenSize.usWLHeight/5;
   } // We have the screen dims

   WinQueryPointerPos(  HWND_DESKTOP,
                        &ptlMouse );

   WinSetWindowPos(  hwndL,
                     HWND_TOP,
                     ptlMouse.x ,
                     ptlMouse.y ,
                     1,
                     1,
                     SWP_MOVE | SWP_HIDE | SWP_SIZE );

   if( fDoAnimateWinList == TRUE ) {
    // Now animate in 5 steps
      for( i = 1; i < 6; i++ ) {
         WinSetWindowPos(  hwndL,
                           HWND_TOP,
                           ( ULONG )ptlMouse.x - ( ScreenSize.usWL_WBits*i )/2,
                           ( ULONG )ptlMouse.y - ( ScreenSize.usWL_HBits*i )/2,
                           ( ULONG )( ScreenSize.usWL_WBits*i ),
                           ( ULONG )( ScreenSize.usWL_HBits*i ),
                           SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE );
    }
   }

   WinSetActiveWindow( HWND_DESKTOP, hwndL );
};


BOOL EXPENTRY eMBee3Hook( HAB habA, PQMSG pqmsgM, ULONG fs )
//-----------------------------------------------------------------------------
// Description: the eMBee "Work horse"
// Purpose    : Examine input queue and act upon WM_BUTTON3CLICK messages
//-----------------------------------------------------------------------------
{
   char     str[ 257 ],
            strN[ 30 ];
   ULONG    ulStyle;
   BOOL     fRc;
   POINTS   psMousePos;
   USHORT   usHitTest,
            usKeybCtrlCode;
   HWND     hwndCnr;

   if( pqmsgM->msg == WM_BUTTON3CLICK ) {
      if( !hwndSwitch )
         hwndSwitch = FindSwitch( );

      WinQueryClassName( pqmsgM->hwnd, 29, ( unsigned char * )strN );


      if( !strcmp( &strN[ 1 ] , "37" ) ) {        		// If 'clicked window = container
      	ptsMouse = *(POINTS*)(VOID*)VOIDFROMMP( pqmsgM->mp1 );

      	WinPostMsg(	hwndUserDlg,
      					globValues.wmu_DoubleClick,
      					MPFROMLONG( pqmsgM->hwnd ),
      					MPFROMLONG( &ptsMouse ) );
         return TRUE;                          // .. msg has been processed
       } else {                                   // else: show tasklist
         return TRUE;
      }
      return TRUE;      // Processed
   } else
     return FALSE;        // Not processed
};

