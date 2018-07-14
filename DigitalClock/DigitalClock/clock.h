#pragma once

#include <afxwin.h>

enum ID_TIMER {
	REPAINT,
	SETTOPMOST,
};

// メインウィンドウクラス
class CMainWin : public CFrameWnd {
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

// アプリケーションクラス
class CApp : public CWinApp {
public:
	BOOL InitInstance();
};

