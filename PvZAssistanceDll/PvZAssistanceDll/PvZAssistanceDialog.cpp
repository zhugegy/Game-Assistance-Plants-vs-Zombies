// PvZAssistanceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PvZAssistanceDll.h"
#include "PvZAssistanceDialog.h"
#include "afxdialogex.h"


// CPvZAssistanceDialog dialog

IMPLEMENT_DYNAMIC(CPvZAssistanceDialog, CDialogEx)

CPvZAssistanceDialog::CPvZAssistanceDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CPvZAssistanceDialog::~CPvZAssistanceDialog()
{
}

void CPvZAssistanceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPvZAssistanceDialog, CDialogEx)
  ON_WM_LBUTTONUP()
  ON_WM_PAINT()
  ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CPvZAssistanceDialog message handlers

void CPvZAssistanceDialog::Plant(LPVOID pThis,
  unsigned int uPosX,
  unsigned int uPosY,
  unsigned int uPlantType)
{
  // TODO: Add your control notification handler code here
  __asm
  {
    pushad
    push - 1
    push uPlantType
    mov eax, uPosY
    push uPosX
    push pThis
    mov  ecx, 0x0040D120
    call ecx
    popad
  }

}

extern CPvZAssistanceDllApp theApp;

void CPvZAssistanceDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

// debug
//   CString strPos;
//   strPos.Format(_T("%d %d"), point.x, point.y);
//   AfxMessageBox(strPos);
  unsigned int uPosX = 0;
  unsigned int uPosY = 0;


  //只在映射区有效
  //only if the mouse is pointing at the mapping zone
  if (point.x >= 10 && point.y >= 90)
  {
    uPosX = (point.x - 10) / 80;
    uPosY = (point.y - 90) / 85;
  }

  Plant((LPVOID)theApp.m_dwBattleEnvir, uPosX, uPosY, 0x0);

  CDialogEx::OnLButtonUp(nFlags, point);
}