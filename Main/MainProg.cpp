//-----------------------------------------------------------------------------
// This program does little more than Displaying a dialog, hooking the system
// with MB3FREE.DLL and wait untill OK is pushed, then unhook the system
//
// Contents:   VOID FreeLib()
//             VOID APIENTRY ExitTrap()
//             MRESULT EXPENTRY FreeDlgProc()
//             main()
//-----------------------------------------------------------------------------

#define INCL_WINSHELLDATA
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_WINWINDOWMGR
#define INCL_WINDESKTOP
#define INCL_WINERRORS
#define INCL_WINHOOKS
#define INCL_WINDIALOGS
#define INCL_WINATOM

#include <os2.h>
#include <string.h>
#include <stdlib.h>

#include "..\common.h"
#include "interface.h"
#include "MB3_Exits.h"
#include "FreeDlg.h"
#include "ErrorDlg.h"

HMODULE 		hmL3;
HINI			hiProfile;


VOID APIENTRY ExitTrap()
//-----------------------------------------------------------------------------
// Description: Added to this process's exitlist in case of a TRAP ...
//-----------------------------------------------------------------------------
{
   eMBeeHookOff();
   DosExitList( EXLST_EXIT, (PFNEXITLIST)ExitTrap );
};

/** Create a standard INI file for Junior!
*/
void CreateNewMB3Profile( HINI hiProfile )
{
	PrfWriteProfileString( hiProfile, APP_NAME, KEY_ANIM,	"1" );
	PrfWriteProfileString( hiProfile, APP_NAME, KEY_WIN, "0" );
};


main( int argc, char* argv[])
//-----------------------------------------------------------------------------
// Description: entrypoint for OS
//-----------------------------------------------------------------------------
{
   HAB         habAnchor;
   HMQ         hmqQueue;
   ERRORID     errID;
   CHAR        chS[ 100 ],
               pchErr[ 100 ];
   QMSG			qmsMsg;
   PFN         pfnRegL3;
   APIRET      RC;
   BOOL        fR;
	char 			pszProfile[ CCHMAXPATH + 1 ];
	SWP			swpDlg;
	HWND			hwndSplash;

   habAnchor = WinInitialize( 0 );
   hmqQueue = WinCreateMsgQueue( habAnchor,
                                 0 );

	// Load and display a 'splash window'
	hwndSplash = WinLoadDlg(	HWND_DESKTOP,
										HWND_DESKTOP,
										SplashProc,
										NULLHANDLE,
										DLG_STARTUP,
										NULL );

   RC = DosLoadModule( (UCHAR*)chS, 99L, (UCHAR*)"3DLINE", &hmL3 );
   RC = DosQueryProcAddr( hmL3, 1L,(UCHAR*)"Line3DRegister", &pfnRegL3 );
   fR = (BOOL)pfnRegL3( habAnchor );

   DosExitList( EXLST_ADD, ( PFNEXITLIST ) ExitTrap );         // Add hook detachment to exitlist

	_execname( pszProfile , CCHMAXPATH );
	char *p;
	p = &pszProfile[ strlen( pszProfile ) ]; // Point to the last char
	while( *p != '.' )
		p--;

	strcpy( ++p, "INI" );

	hiProfile = PrfOpenProfile( habAnchor, pszProfile );

	int iTemp;
	iTemp = PrfQueryProfileInt(	hiProfile,
											APP_NAME,
											KEY_ANIM,
										   -1 );

	// If iTemp equals -1, then the profile is a new one!
	if( iTemp == -1 )
	{
		char pszMsg[] ={ "A new program profile is created at location:\n" };
		char *pszStr;
		pszStr = new char[ strlen( pszMsg ) + strlen( pszProfile ) + 1 ];
		strcpy( pszStr, pszMsg );
		strcat( pszStr, pszProfile );
		
		WinMessageBox(	HWND_DESKTOP,
							hwndSplash,
							pszStr,
							"eM Bee 3: Junior!",
							2,
							MB_ENTER | MB_INFORMATION | MB_MOVEABLE );
							
		CreateNewMB3Profile( hiProfile );
		
		delete []pszStr;
	} else
	{
		fR = PrfQueryProfileInt(	hiProfile,
											APP_NAME,
											KEY_WIN,
											FALSE );
	} // if else
	if( argc > 1 )
	{
		if( strstr( argv[ 1 ] , "min" ) )
			fR = TRUE;
		else
			if( strstr( argv[ 1 ], "show" ) )
				fR = FALSE;
	}

	HWND hwndD= WinLoadDlg( HWND_DESKTOP,
                  			HWND_DESKTOP,
                  			FreeDlgProc,
                  			NULLHANDLE,
                  			IDDLG_MAIN,
                  			NULL );
                  			
	if( hwndD == DID_ERROR )
	{
   	errID = WinGetLastError( habAnchor );
   	strcpy( pchErr, "Couldn't create dialog, PMERR code: ");
   	_ultoa( errID, chS, 10 );
   	strcat( pchErr, chS );
   	ErrorBox( pchErr, "main()" );
   } else
   {
		WinSetWindowPos(	hwndD,
								HWND_TOP,
								0, 0,
								0, 0,
								fR ?  SWP_MINIMIZE | SWP_ZORDER : SWP_RESTORE | SWP_SHOW | SWP_ZORDER | SWP_ACTIVATE );
		// Remove 'splash window'
		WinDestroyWindow( hwndSplash );

		while( WinGetMsg(	habAnchor, &qmsMsg, 0, 0, 0 ) )
			WinDispatchMsg( habAnchor, &qmsMsg );
   } // if/else

   WinDestroyWindow( hwndD );

	PrfCloseProfile( hiProfile );
   WinDestroyMsgQueue( hmqQueue );
   WinTerminate( habAnchor );

   return MB3_NORMAL_EXIT;

};
