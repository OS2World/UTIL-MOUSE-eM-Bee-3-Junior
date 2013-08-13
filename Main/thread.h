#ifndef __MB3_THRD
#define __MB3_THRD
#include <os2.h>
/** Communication with text thread.
*/
typedef struct _THREADINFO{	
	/// Size of struct
	ULONG	ulSz;
	/// Handle of text control
	HWND	hwndText;
	/// Is Junior sleeping?
	BOOL	fSleeping;
	/** Is thread to be killed?.
		if this is set to true, it will be set to false when the thread ends
	*/
	HWND	hwndOwner;
	BOOL	fKillThread;
	BOOL	fEngaged;
} THREADINFO, *PTHREADINFO;

#define MBM_THREADOK (WM_USER+2)

VOID _Optlink TextThread( PTHREADINFO );
#endif
