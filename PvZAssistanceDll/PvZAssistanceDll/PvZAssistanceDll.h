// PvZAssistanceDll.h : main header file for the PvZAssistanceDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "PvZAssistanceDialog.h"

// CPvZAssistanceDllApp
// See PvZAssistanceDll.cpp for the implementation of this class
//

typedef LRESULT(__stdcall *PFNWindowProc)(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam);   // second message parameter


class CPvZAssistanceDllApp : public CWinApp
{
//custom
public:
  PFNWindowProc m_pfnWindowProc;

  HWND m_hMainWnd;  //game's 
  HANDLE m_hProcess;  //game's 

  DWORD m_dwBaseAddress;  //Base Address in Pvz
  DWORD m_dwBattleEnvir;  //battlefield enviroment in PvZ

  CPvZAssistanceDialog* m_pDlg;  //assistance window

public:
	CPvZAssistanceDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
