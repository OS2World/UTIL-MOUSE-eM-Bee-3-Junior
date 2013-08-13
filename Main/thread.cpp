#include "thread.h"
#include "interface.h"

#define INCL_DOSPROCESS
#define INCL_WINWINDOWMGR

#include <os2.h>
#include <string.h>

/** Copies the appropriate number of 'z' into the string, padding as nececary.
*/
void MakeZZ( char* pszS, USHORT usIdx )
{
	if( usIdx >15 ) {
		usIdx = 15 - ( usIdx - 15 );
	}

	while( usIdx > 0 ) {
		*pszS = 'z';
		pszS++;
		usIdx--;
	}
	*pszS = '\0';
};

/** Text thread.
	Provides visual feedback to user.
*/
VOID _Optlink TextThread( PTHREADINFO ptiInf )
{
	HAB		habDummy;
	HMQ		hmqDummy;
	USHORT   usIdx=1;
	USHORT	usReadyIdx = 1;
	BOOL 		fBlink=TRUE;
	BOOL		fSleeping = FALSE;
	const char pszReady[] = { "eM Bee 3: Junior! is ready!!" };
   char		pszString[ 31 ];
	
	pszString[ 0 ] = '\0';

	habDummy = WinInitialize( 0 );
	hmqDummy = WinCreateMsgQueue( habDummy , 0 );
	
	WinPostMsg( ptiInf->hwndOwner,
					MBM_THREADOK,
					MPFROMP( &fSleeping ),
					0L);

	//---------- text looping, once every 150ms -------------
	while( !ptiInf->fKillThread ) {
		if( ptiInf->fSleeping )
		{
			fBlink = TRUE;
			usReadyIdx = 0;
		
			MakeZZ( &pszString[ 0 ], usIdx );
			
			if( usIdx < 30  )
				usIdx++;
			else
				usIdx = 1;

			WinSetWindowText( ptiInf->hwndText, pszString );
		} else {
			if( fBlink )
			{
				usIdx = 1;
				
				strncpy( pszString, pszReady, usReadyIdx );
				usReadyIdx++;
				pszString[ usReadyIdx ] = '\0';
				
				WinSetWindowText( ptiInf->hwndText, pszString );
		
				if( (usReadyIdx - 1)>= strlen( pszReady ) )
				{
					strcpy( pszString, " " );
					fBlink = FALSE;
				}
			} // if blink
		} // if sleeping
		DosSleep( 150 );
	} // for

	//---------- terminating --------------------------------	
	WinDestroyMsgQueue( hmqDummy );
	WinTerminate( habDummy );
	
	DosEnterCritSec();
	ptiInf->fKillThread = FALSE;
	
	return;
};
