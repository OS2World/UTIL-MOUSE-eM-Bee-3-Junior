//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define INCL_WINDIALOGS
#include <os2.h>
#include <string.h>
#include "Interface.h"
#include "ErrorDlg.h"

MRESULT EXPENTRY ErrorDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mp1, MPARAM mp2 )
{
   ErrStruct *perr;
   switch( ulMsg ) {
   case WM_INITDLG:
         perr = ( ErrStruct * )PVOIDFROMMP( mp2 );
         WinSetWindowText( WinWindowFromID( hwndDlg, IDT_TITLE ),
                           perr->title ) ;
         WinSetWindowText( WinWindowFromID( hwndDlg, IDT_MESSAGE  ),
                           perr->msg ) ;
      break;
   default: 
      return WinDefDlgProc( hwndDlg, ulMsg, mp1, mp2 );
   }
   return MRFROMSHORT( TRUE );
};

VOID ErrorBox( const char *pcchMsg, const char *pcchTitle )
{
   ErrStruct   err;
   
   strcpy( err.msg, pcchMsg     );
   strcpy( err.title, pcchTitle );
   err.ulSz = sizeof( ErrStruct );

   WinDlgBox(  HWND_DESKTOP,
               HWND_DESKTOP,
               ErrorDlgProc,
               NULLHANDLE,
               IDLG_ERROR,
               (PVOID)&err );
};

