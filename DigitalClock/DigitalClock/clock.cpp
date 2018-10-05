// simple digital clock

#include <afxwin.h>
#include <cstring>
#include <ctime>
#include "clock.h"

// for mouse move.
#define INPUT_DATA_NUM (1)

// default window location.
#define INITIAL_LEFT (-10)
#define INITIAL_TOP (-35)
#define INITIAL_RIGHT (300)
#define INITIAL_BOTTOM (300)

// key for registry.
#define REGKEY "Location"

// instance of the application.
CApp App;

// constructor.
CMainWin::CMainWin() : str(), isTopMost(TRUE) {
	RECT r;
	RestoreLocation(&r.left, &r.top, &r.right, &r.bottom);
	Create(NULL, _T("INSOMNIA"), WS_OVERLAPPEDWINDOW, r);
}

// initialize the application.
BOOL CApp::InitInstance() {
	// company name for registry.
	SetRegistryKey(_T("MSW"));

	m_pMainWnd = new CMainWin;
	// set interval timer for repaint.
	m_pMainWnd->SetTimer(ID_TIMER::REPAINT, 500, NULL);
	// set interval timer for mouse move.
	m_pMainWnd->SetTimer(ID_TIMER::MOVEMOUSE, 30000, NULL);
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

	// restore location of the window from registry.
	RECT r;
	RestoreLocation(&r.left, &r.top, &r.right, &r.bottom);
	SetWindowPos(&CMainWin::wndTopMost, r.left, r.top, r.right, r.bottom, SWP_NOOWNERZORDER);

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

		// back-up the location of the window.
		RECT rect;
		this->GetWindowRect(&rect);
		BackupLocation(rect.left, rect.top, rect.right, rect.bottom);

		InvalidateRect(NULL, 0);
	}
	else if (ID == ID_TIMER::MOVEMOUSE) {
		// suppress going into stand-by mode.
		::SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_CONTINUOUS);
		// suppress launching screen-saver.
		::SetThreadExecutionState(ES_DISPLAY_REQUIRED);
		// move mouse to (dx:=0, dy:=0).
		INPUT data[INPUT_DATA_NUM];
		INPUT *p_data = data;
		memset(p_data, 0x00, sizeof(INPUT));
		p_data->type = INPUT_MOUSE;
		p_data->mi.dwFlags = MOUSEEVENTF_MOVE;
		::SendInput(INPUT_DATA_NUM, p_data, sizeof(INPUT));
		_RPT0(_CRT_WARN, "I'm suffering from insomnia. I never sleep.\n");
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

// backup the window location into registry.
void CMainWin::BackupLocation(long x1, long y1, long x2, long y2) {
	App.WriteProfileInt(_T(REGKEY), _T("x1"), (INT)x1);
	App.WriteProfileInt(_T(REGKEY), _T("y1"), (INT)y1);
	App.WriteProfileInt(_T(REGKEY), _T("x2"), (INT)x2);
	App.WriteProfileInt(_T(REGKEY), _T("y2"), (INT)y2);
}

// restore the windows location from registry.
void CMainWin::RestoreLocation(long *x1, long *y1, long *x2, long *y2) {
	*x1 = (INT)App.GetProfileInt(_T(REGKEY), _T("x1"), INITIAL_LEFT);
	*y1 = (INT)App.GetProfileInt(_T(REGKEY), _T("y1"), INITIAL_TOP);
	*x2 = (INT)App.GetProfileInt(_T(REGKEY), _T("x2"), INITIAL_RIGHT);
	*y2 = (INT)App.GetProfileInt(_T(REGKEY), _T("y2"), INITIAL_BOTTOM);
}


// terminate the application.
afx_msg void CMainWin::OnDestroy() {
	KillTimer(ID_TIMER::REPAINT);
	KillTimer(ID_TIMER::MOVEMOUSE);
}
