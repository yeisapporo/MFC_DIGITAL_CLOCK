// simple digital clock
//

#include <afxwin.h>
#include <cstring>
#include <ctime>
#include "clock.h"

// constructor.
CMainWin::CMainWin() {
	RECT r;
	r.left =  -10;
	r.top =   -35;
	r.right = 207;
	r.bottom = 70;

	Create(NULL, _T("digital clock"), WS_OVERLAPPEDWINDOW, r);
}

// initialize the application.
BOOL CApp::InitInstance() {
	m_pMainWnd = new CMainWin;
	// set interval timer for repaint.
	m_pMainWnd->SetTimer(ID_TIMER::REPAINT, 500, NULL);
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

// message map for the application.
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// region settings and window style.
afx_msg int CMainWin::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CRgn rgn;
	rgn.CreateRectRgn(10, 33, 170, 62);
	SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);

	// make the black(0, 0, 0) area transparent.
	LONG exStyle = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
	// WS_EX_COMPOSITED : use built-in double buffers.
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_COMPOSITED);
	this->SetLayeredWindowAttributes(RGB(0, 0, 0), 0, LWA_COLORKEY);

	return 0;
}

// process WM_PAINT message.
afx_msg void CMainWin::OnPaint() {
	CPaintDC dc(this);

	dc.FillSolidRect(0, 0, 500, 500, RGB(0, 0, 0));

	CFont font;
	SetFont(&dc, &font);
	
	dc.SetTextColor(RGB(63, 127, 255));

	// re-display the string(HH:MM:SS).
	dc.TextOutW(1, 1, str, _tcslen(str));
}

// change z-order to set the window bottommost.
afx_msg void CMainWin::OnLButtonUp(UINT Flags, CPoint loc) {
	SetWindowPos(&CMainWin::wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetTimer(ID_TIMER::SETTOPMOST, 5000, NULL);
	isTopMost = FALSE;
}

// move the window by dragging. 
afx_msg void CMainWin::OnLButtonDown(UINT Flags, CPoint loc) {
	this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(loc.x, loc.y));
}

// handle WM_TIMER message.
afx_msg void CMainWin::OnTimer(UINT ID) {
	if (ID == ID_TIMER::SETTOPMOST) {
		// raise the window topmost.
		KillTimer(ID_TIMER::SETTOPMOST);
		isTopMost = TRUE;
	}
	else if (ID == ID_TIMER::REPAINT && isTopMost == TRUE) {
		// always topmost.
		SetWindowPos(&CMainWin::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		CTime curtime = CTime::GetCurrentTime();
		struct tm t;

		curtime.GetLocalTm(&t);
		wsprintf(str, _T("%02d:%02d:%02d"), t.tm_hour, t.tm_min, t.tm_sec);

		InvalidateRect(NULL, 0);
	}
}

// set the font.
void CMainWin::SetFont(CPaintDC *pDC, CFont *pFont) {
	pFont->CreateFont(
		32,                   // height
		0,                    // width
		0,                    // angle
		0,                    // angle
		FW_EXTRABOLD,         // thickness
		FALSE,                // italic
		FALSE,                // underline
		FALSE,                // strike line
		ANSI_CHARSET,         // character set
		OUT_DEFAULT_PRECIS,   // output precision
		CLIP_DEFAULT_PRECIS,  // clipping precision
		DEFAULT_QUALITY,      // quality 
		DEFAULT_PITCH,        // pitch and family of font
		_T("Lucida Console")  // type face name
	);
	
	pDC->SelectObject(pFont);
}

// terminate the application.
afx_msg void CMainWin::OnDestroy() {
	KillTimer(ID_TIMER::REPAINT);
}

// instance of the application.
CApp App;
