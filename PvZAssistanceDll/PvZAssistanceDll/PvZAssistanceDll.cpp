// PvZAssistanceDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PvZAssistanceDll.h"
#include "PvZAssistanceDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CPvZAssistanceDllApp

BEGIN_MESSAGE_MAP(CPvZAssistanceDllApp, CWinApp)
END_MESSAGE_MAP()


// CPvZAssistanceDllApp construction

CPvZAssistanceDllApp::CPvZAssistanceDllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPvZAssistanceDllApp object

CPvZAssistanceDllApp theApp;

//Customized windows proc function.
//Intercept shortcut input: DELETE key, and invoke the assistance window.
LRESULT  MyWindowProc(HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{

  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_DELETE)
    {
      TRACE(_T("wParam == VK_DELETE"));

      if (theApp.m_pDlg == NULL)
      {
        theApp.m_pDlg = new CPvZAssistanceDialog;
        theApp.m_pDlg->Create(IDD_DIALOG1);
        theApp.m_pDlg->ShowWindow(SW_SHOWNORMAL);
      }
      else
      {
        if (theApp.m_pDlg->IsWindowEnabled())
        {
          theApp.m_pDlg->ShowWindow(SW_HIDE);
        }
        else
        {
          theApp.m_pDlg->ShowWindow(SW_SHOWNORMAL);
        }
      }

    }
  }

  return CallWindowProc((PFNWindowProc)theApp.m_pfnWindowProc, hwnd, uMsg,
    wParam, lParam);
}


// CPvZAssistanceDllApp initialization
BOOL CPvZAssistanceDllApp::InitInstance()
{
	CWinApp::InitInstance();

  m_hMainWnd = ::FindWindow(NULL, _T("植物大战僵尸中文版"));

  m_pDlg = NULL;

  if (m_hMainWnd == NULL)
  {
    TRACE(_T("找不到游戏窗口\r\n"));
    TRACE(_T("Can not find game window.\r\n"));

    return FALSE;
  }

  //劫持PvZ的窗口信息函数
  //hijack PvZ's window proc function
  m_pfnWindowProc = (PFNWindowProc)SetWindowLong(m_hMainWnd, GWL_WNDPROC,
    (LONG)MyWindowProc);

  TRACE(_T("m_pfnWindowProc: %p"), m_pfnWindowProc);

  //获取PvZ的进程ID
  //Get PvZ's process ID
  DWORD dwPId = 0;
  GetWindowThreadProcessId(m_hMainWnd, &dwPId);
  m_hProcess = INVALID_HANDLE_VALUE;
  if (dwPId != 0)
  {
    m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPId);
    if (m_hProcess == INVALID_HANDLE_VALUE)
    {
      TRACE(_T("找不到游戏进程\r\n"));
      TRACE(_T("Can not find game process.\r\n"));
    }
  }

  //获取战场环境
  //Get battle field environment
  DWORD dwReadedByte = 0;
  BOOL bRet = FALSE;
  ReadProcessMemory(m_hProcess, (LPCVOID)0x6A9EC0, &m_dwBaseAddress, 
    sizeof(INT), &dwReadedByte);
  DWORD dwBattleFieldAddr = m_dwBaseAddress + 0x768;
  ReadProcessMemory(m_hProcess, (LPCVOID)dwBattleFieldAddr, &m_dwBattleEnvir, 
    sizeof(INT), &dwReadedByte);

  //自动拾取阳光
  BYTE btAutoPicking = 1;
  DWORD dwAutoPickingConditionAddr = 0x0043158e;
  WriteProcessMemory(m_hProcess, (LPWORD)dwAutoPickingConditionAddr,
    &btAutoPicking, sizeof(BYTE), NULL);

	return TRUE;
}


