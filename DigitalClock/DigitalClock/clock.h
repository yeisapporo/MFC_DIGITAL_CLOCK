#pragma once
#include <afxwin.h>

enum ID_TIMER {
	REPAINT,
	SETTOPMOST,
};

// main window class
class CMainWin : public CFrameWnd {
private:
	// retain string for output.
	TCHAR str[80] = { '\0' };
	// Z-order.
	BOOL isTopMost = TRUE;
private:
	void SetFont(CPaintDC * pDC, CFont * pFont);
public:
	CMainWin();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT ID);
	afx_msg void OnLButtonUp(UINT Flags, CPoint loc);
	afx_msg void OnLButtonDown(UINT Flags, CPoint loc);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

// application class
class CApp : public CWinApp {
public:
	BOOL InitInstance();
};
