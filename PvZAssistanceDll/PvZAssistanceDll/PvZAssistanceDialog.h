#pragma once


// CPvZAssistanceDialog dialog

class CPvZAssistanceDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPvZAssistanceDialog)

public:
	CPvZAssistanceDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPvZAssistanceDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:
  void Plant(LPVOID pThis, unsigned int uPosX, unsigned int uPosY,
    unsigned int uPlantType);

};
