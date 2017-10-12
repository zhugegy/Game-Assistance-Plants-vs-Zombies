// PvZAssistanceStarter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <windows.h>
#include <afx.h>  //for CString

#define DLL_PATH "PvZAssistanceDll.dll"  //our assistance dll's name
#define EXE_NAME "植物大战僵尸中文版"  //the captain of game window

/*
* 远程线程注入
* 1 获取目标进程hProcess。
* 2 向目标进程申请空间，并写入DLL路径。
* 3 通过CreateRemoteThread()，调用目标进程的LoadLibrary()，加载第二步的dll。
* 4 （可选）通过CreateRemoteThread()，调用目标进程的FreeLibrary()，释放第二步的dll。
*/

/*
* RemoteThreadInjection
* 1 Get target process's hProcess.
* 2 Alloc target process's memory space, and write in dll path.
* 3 Via CreateRemoteThread(), call target process's LoadLibrary(), loading our
    dll.
* 4 (optional) Via CreateRemoteThread(),call target process's FreeLibrary(), 
    releasing our dll.
*/

int main(int argc, char* argv[])
{
  //1 获取目标进程hProcess。
  //1 Get target process's hProcess.
  HWND hWnd = FindWindow(NULL, TEXT(EXE_NAME));
  DWORD dwPid = 0;
  DWORD dwTid = GetWindowThreadProcessId(hWnd, &dwPid);
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

  //2 向目标进程申请空间，并写入DLL路径。
  //2 Alloc target process's memory space, and write in dll path.
  LPVOID pAddr = VirtualAllocEx(hProcess,
    NULL,
    0x1000,
    MEM_COMMIT,
    PAGE_EXECUTE_READWRITE);

  if (pAddr == NULL)
  {
    return 0;
  }

  DWORD dwWritedBytes = 0;

  char szCurrentPath[MAXBYTE] = { 0 };

  DWORD dwRet = GetCurrentDirectory(MAXBYTE, (LPWSTR)szCurrentPath);

  CString szDllPath;

  szDllPath.Format(TEXT("%s%c%s"), szCurrentPath, '\\', TEXT(DLL_PATH));

  WriteProcessMemory(hProcess,
    (LPVOID)pAddr,
    szDllPath.GetBuffer(0),
    /*wcslen(pstr) + 2*/500,  //暂时设为500  //set to 500 for temporary
    &dwWritedBytes);

  //3 通过CreateRemoteThread(), 调用目标进程的LoadLibrary(), 加载第二步的dll。
  //3 Via CreateRemoteThread(), call target process's LoadLibrary(), loading our
  //  dll.
  HANDLE hRemoteThread = CreateRemoteThread(hProcess,
    NULL,
    0,
    (LPTHREAD_START_ROUTINE)LoadLibrary,
    (LPVOID)pAddr,
    0,
    NULL);

  WaitForSingleObject(hRemoteThread, INFINITE);

  DWORD dwExitCode = 0;
  BOOL bRet = GetExitCodeThread(hRemoteThread, &dwExitCode);

  //如果远程线程没有正常结束
  //if remote thread was not terminated normally
  if (!bRet)
  {
    return 0;
  }

  //4 （可选）通过CreateRemoteThread()，调用目标进程的FreeLibrary()，释放第二步的
  //   dll。
  //4 (optional) Via CreateRemoteThread(),call target process's FreeLibrary(), 
  //   releasing our dll.
//   HANDLE hRemoteThread2 = CreateRemoteThread(hProcess,
//     NULL,
//     0,
//     (LPTHREAD_START_ROUTINE)FreeLibrary,
//     (LPVOID)dwExitCode,
//     0,
//     NULL);

  return 0;
}