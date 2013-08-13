//
#ifndef _MB3_COMMON_H__
#define _MB3_COMMON_H__

#define INCL_WINATOM
#include <os2.h>

/** Encapsulate the atomic messages used.
	Messages posted from hook in DLL to window.
*/
typedef struct {
	ULONG	ulSz;
	ATOM	wmu_ShowWinList;
	CHAR	pszShowWinList[ 41 ];
	ATOM 	wmu_DoubleClick;
	CHAR	pszDoubleClick[ 41 ];
	BOOL	fActivated;
} GLOBAL, *PGLOBAL;

#ifdef __cplusplus
extern "C" {
#endif
    PGLOBAL EXPENTRY GetGloabls();
    BOOL EXPENTRY eMBeeInitHook( HWND hwndClient );
    BOOL EXPENTRY   eMBeeHookOff();
#ifdef __cplusplus
}
#endif


#endif
