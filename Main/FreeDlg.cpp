//----------------------------- FreeDlg.cpp -----------------------------------
// Contents: Window procedure for "main" dialog box
//-----------------------------------------------------------------------------
#define INCL_DOSPROCESS
#define INCL_DEV
#define INCL_GPIBITMAPS
#define INCL_WININPUT
#define INCL_WINMESSAGEMGR
#define INCL_WINBUTTONS
#define INCL_WINWINDOWMGR
#define INCL_WINHOOKS
#define INCL_WINDIALOGS
#define INCL_WINATOM
#define INCL_WINSHELLDATA
#define INCL_WINFRAMEMGR

#include <os2.h>
#include <string.h>
#include <stdlib.h>

#include "interface.h"
#include "..\common.h"
#include "ErrorDlg.h"
#include "SwitchList.h"
#include "FreeDlg.h"
#include "open_folder.h"
#include "thread.h"

extern HWND hwndWinList;
THREADINFO	tiText;
extern HINI hiProfile;
PGLOBAL		globVals ;

const int iAwake_x = 393;
const int iAwake_y = 257;
const int iSleep_x = 443;
const int iSleep_y = 215;

VOID Toggle( HWND hwndDlg, BOOL fToggle )
{
   char     pchMsg[ 101 ];
   RECTL    rclAThird;

   WinSendDlgItemMsg(  hwndDlg,                 // .. set checkbox accordingly
                       IDPB_ENABLE,
                       BM_SETCHECK,
                       MPFROMSHORT( fToggle ),
                       MPFROMLONG( 0L ) );

   WinEnableControl(  hwndDlg,                 // Enable/disable radios
                      RB_NORMALWINDOWLIST,
                      fToggle );

   WinEnableControl(  hwndDlg,
                      RB_ANIMATEWINDOWLIST,
                      fToggle );

   WinQueryWindowRect( WinWindowFromID( hwndDlg, IDB_NULL), &rclAThird );   // Force a WM_PAINT to come..

   WinInvalidateRect( hwndDlg,
                      &rclAThird,
                      TRUE );
};


MRESULT EXPENTRY FreeDlgProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpOne, MPARAM mpTwo )
//-----------------------------------------------------------------------------
// Description: Window procedure for "main"
//-----------------------------------------------------------------------------
{
	MRESULT				mrReply;
	static BOOL			fAnim, fMin;
	static HBITMAP		hbmAwake, hbmAwakeMask;
	static HBITMAP		hbmSleep, hbmSleepMask;

	char	pchFailBuf[ 23 ];
	char	pszArgs[] = { "view.exe\0mb3jr.inf Help" };
	char	pszView[] = { "view.exe" };
	RESULTCODES	rc;

	switch( ulMsg ) {
	case WM_INITDLG:
		if( !eMBeeInitHook( hwndDlg ) )
		{
			ErrorBox( 	"Couldn't hook DLL to system!\n\nProgram has no purpose without hook\n\nExiting..",
							"FreeDlgProc: ulMsg = WM_INITDLG" );
			WinPostMsg( hwndDlg,
							WM_CLOSE,
							MPFROMLONG( 0L ),
							MPFROMLONG( 0L ) );
		} else {
			globVals = GetGloabls();

			hwndWinList = NULLHANDLE;
			fAnim = FALSE;
			
			// Set up thread
			tiText.ulSz = sizeof( THREADINFO );
			tiText.fKillThread = FALSE;
			tiText.fSleeping = FALSE;	
			tiText.hwndText = WinWindowFromID( hwndDlg, TXT_STATUS );
			tiText.hwndOwner = hwndDlg;


			HPS hps;
			hps = WinGetPS( hwndDlg );
			
			hbmAwake = GpiLoadBitmap(	hps,
												NULLHANDLE,
												IDB_JR_AWAKE,
												iAwake_x , iAwake_y );

			hbmAwakeMask = GpiLoadBitmap( 	hps,
														NULLHANDLE,
														IDB_JR_AWAKE_MASK,
														iAwake_x , iAwake_y );
			hbmSleep = GpiLoadBitmap(	hps,
												NULLHANDLE,
												IDB_JR_ASLEEP,
												iSleep_x , iSleep_y );

			hbmSleepMask = GpiLoadBitmap(	hps,
													NULLHANDLE,
													IDB_JR_ASLEEP_MASK,
													iSleep_x , iSleep_y );

			WinReleasePS( hps );

			WinPostMsg(	hwndDlg,
							MBM_CREATEDONE,
							0L,
							0L );
		} // is/else
	break;// WM_INITDLG

	case MBM_CREATEDONE:
	{
		if( _beginthread( TextThread, NULL, 0x4000, (PVOID)&tiText ) == -1 )
		{
			ErrorBox( "Unable to create signaling thread!", "FreeDlg.cpp: MBM_CREATEDONE" );
			tiText.fEngaged = TRUE;
		}
		
		WinSendDlgItemMsg(	hwndDlg,
									IDPB_ENABLE,
									BM_SETCHECK,
									MPFROMSHORT( TRUE ),
									MPFROMLONG( 0 ) );

      WinEnableControl(  hwndDlg,                 // Enable/disable radios
                         RB_NORMALWINDOWLIST,
                         globVals->fActivated );

      WinEnableControl(  hwndDlg,
  	                      RB_ANIMATEWINDOWLIST,
     	                   globVals->fActivated );
		   	
		fAnim = (BOOL)PrfQueryProfileInt(	hiProfile,
														APP_NAME,
														KEY_ANIM,

														FALSE );

      WinSendDlgItemMsg(   hwndDlg,
  	                        fAnim ? RB_ANIMATEWINDOWLIST : RB_NORMALWINDOWLIST,
     	                     BM_SETCHECK,
        	                  MPFROMSHORT( TRUE ),
           	               MPFROMLONG( 0L ) );
	
	} // MBM_CREATEDONE
	break;

   case WM_CONTROL:
      switch( SHORT1FROMMP( mpOne ) )
      {
      	case IDPB_ENABLE:
         	if( SHORT2FROMMP( mpOne ) == BN_CLICKED )
         	{       // if checkbox was clicked
	         	if( globVals->fActivated )
   	      	{
      	   		if( !eMBeeHookOff() )                               // .. toogle hook and
      	   		{
         				ErrorBox( "Unable to un-hook system!\n\nSystem could be unstable at this point.\nReboot recomended.",
         							 "FreeDlgProc():line 132" );
         			} else {
         				tiText.fSleeping = TRUE;
       				}			
         		} else {
         			if( !eMBeeInitHook( hwndDlg ) )
         			{
         				ErrorBox( "Unable to hook system!\n\nSystem could be unstable at this point.\nReboot recomended.",
         							 "FreeDlgProc():line 136" );
         			} else {
         				tiText.fSleeping = FALSE;
         			}
         		}
         		Toggle( hwndDlg, globVals->fActivated );          // .. update dialog
         	} // if
         	break; // BN_CLICKED

      	case RB_ANIMATEWINDOWLIST:
				fAnim = TRUE;
			break; // RB_ANIMATEWINDOWLIST

      	case RB_NORMALWINDOWLIST:
      		fAnim = FALSE;
      	break; // RB_NORMALWINDOWLIST
		} // WM_CONTROL

   case WM_COMMAND:
   	switch( SHORT1FROMMP( mpOne ) )
   	{
   		case PB_EXIT:
   		WinPostMsg(	hwndDlg,
   						WM_CLOSE,
   						MPFROMLONG( 0L ),
   						MPFROMLONG( 0L ) );
   		break; // PB_EXIT

   		case PB_HELP:
   		{
   			APIRET	arRet;
   			arRet = DosExecPgm(	pchFailBuf,
	      								22,
	      								1,
	      								pszArgs,
	      								NULL,
				      					&rc,
				      					pszView );
      	}
			break; // PB_HELP
		} // switch
	break; // WM_COMMAND

	case WM_CLOSE:
	{
		USHORT usResp;
		usResp = WinMessageBox(	HWND_DESKTOP,
										hwndDlg,
										"Really quit?",
										"eM Bee 3: Junior!",
										511,
										MB_ICONQUESTION | MB_YESNO | MB_MOVEABLE );
		
		if( usResp == MBID_YES )
		{
			tiText.fKillThread = TRUE;
		
			while( tiText.fKillThread && tiText.fEngaged )
				DosSleep( 1 );
		
			WinPostMsg(	hwndDlg,
							WM_QUIT,
							MPFROMLONG( 0L ),
							MPFROMLONG( 0L ) );
		} else {
			return MRFROMSHORT( FALSE );
		}
	}
	break; // WM_CLOSE

	case WM_PAINT:
	{
		POINTL ptlWhere;
		RECTL	 rclInvalidArea;
		
		// First call default proc.
		WinDefDlgProc(   hwndDlg,
		                 ulMsg,
  		                 mpOne,
    	                 mpTwo );
		
		if( !tiText.fSleeping ) {
			ptlWhere.x = 10;
			ptlWhere.y = 10;
			rclInvalidArea.xLeft = ptlWhere.x;
			rclInvalidArea.xRight = ptlWhere.x + iSleep_x + 1;
			rclInvalidArea.yBottom = ptlWhere.y;
			rclInvalidArea.yTop = ptlWhere.y + iSleep_y + 1;
			
			Paint( hwndDlg, &ptlWhere, iAwake_x, iAwake_y, hbmAwake, hbmAwakeMask, &rclInvalidArea  );
		} else {
			ptlWhere.x = 10;
			ptlWhere.y = 10;
			rclInvalidArea.xLeft = ptlWhere.x;
			rclInvalidArea.xRight = ptlWhere.x + iAwake_x + 1;
			rclInvalidArea.yBottom = ptlWhere.y;
			rclInvalidArea.yTop = ptlWhere.y + iAwake_y + 1;
			
			Paint( hwndDlg, &ptlWhere, iSleep_x, iSleep_y, hbmSleep, hbmSleepMask  , &rclInvalidArea );
		}
	}
	break; // WM_PAINT
	
   case WM_SAVEAPPLICATION:
   {	
   	SWP swpDlg;
   	PrfWriteProfileString(	hiProfile,
   									APP_NAME, KEY_ANIM,
   									fAnim ? "1" : "0" );
   	
   	WinQueryWindowPos( hwndDlg, &swpDlg );
   	
   	BOOL fTest = ( swpDlg.fl & SWP_MINIMIZE );

   	PrfWriteProfileString(	hiProfile,
   									APP_NAME, KEY_WIN,
   									fTest ? "1" : "0" );
	}
	break; // WM_SAVEAPPLICATION

	case WM_DESTROY:
		GpiDeleteBitmap( hbmAwake );
		GpiDeleteBitmap( hbmAwakeMask );
		GpiDeleteBitmap( hbmSleep );
		GpiDeleteBitmap( hbmSleepMask );
		if( !eMBeeHookOff() )
			ErrorBox( "Unable to un-hook!", "WM_DESTROY" );;
	break; // WM_DESTROY

   default:
   	if( globVals )
   	{
   		if( ulMsg == globVals->wmu_DoubleClick )
   		{
   	       POINTS	ptsM;
   	       ptsM.x = SHORT1FROMMP( mpTwo );
   	       ptsM.y = SHORT2FROMMP( mpTwo );
   	
				if( ! OpenObject( LONGFROMMP( mpOne ), &ptsM ) )
					ErrorBox( "Couldn't open object!", "FreeDlgProc: globVals->wmu_doubleClick" );
				break;
      	} else { // ulMsg == globVals->wmu_DoubleClick
      		if( ulMsg ==  globVals->wmu_ShowWinList )
      		{
      		// user pressed MB3 on something else
      			ShowWinList( fAnim );
					break;
				} else { // ulMsg == globVals->wmu_showWinList
					return WinDefDlgProc(   hwndDlg,
													ulMsg,
													mpOne,													
													mpTwo );
				}
			}
		} else // globVals
			return WinDefDlgProc(   hwndDlg,
											ulMsg,
											mpOne,
											mpTwo );
   } // switch

   return MRFROMSHORT( TRUE );
};


MRESULT EXPENTRY SplashProc( HWND hwndDlg, ULONG ulMsg, MPARAM mpOne, MPARAM mpTwo )
{
	      return WinDefDlgProc(   hwndDlg,
  		                           ulMsg,
     		                        mpOne,
        		                     mpTwo );
};

