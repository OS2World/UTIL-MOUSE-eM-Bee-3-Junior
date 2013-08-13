//--------------------------------- ErrorDlg.h --------------------------------
//-----------------------------------------------------------------------------
#ifndef __ERRORDLG_H
#define __ERRORDLG_H

VOID ErrorBox( const char *pcchMsg, const char *pcchTitle );

#define MBM_CREATEDONE (WM_USER+1)

struct ErrStruct {
   ULONG ulSz;
   char  msg[ 150  ];
   char  title[ 30 ];
};
#endif
