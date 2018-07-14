// 時計プログラム

#include <afxwin.h>
#include <cstring>
#include <ctime>
#include "clock.h"

// 出力文字列を保持する
TCHAR str[80] = _T("");
// Z-order
BOOL isTopMost = TRUE;

CMainWin::CMainWin() {
	RECT r;
	r.left =  -10;
	r.top =   -35;
	r.right = 207;
	r.bottom = 70;

	Create(NULL, _T("A Clock Program"), WS_OVERLAPPEDWINDOW, r);
}

// アプリケーションを初期化する
BOOL CApp::InitInstance() {
	m_pMainWnd = new CMainWin;
	// 再描画用タイマーを設定する
	m_pMainWnd->SetTimer(ID_TIMER::REPAINT, 500, NULL);
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

// アプリケーションのメッセージマップ
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// リージョン設定
afx_msg int CMainWin::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CRgn rgn;
	rgn.CreateRectRgn(10, 33, 170, 62);
	SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);

	// 黒(0, 0, 0)の領域を透明にする
	LONG exStyle = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_COMPOSITED);
	this->SetLayeredWindowAttributes(RGB(0, 0, 0), 0, LWA_COLORKEY/*LWA_ALPHA*/);

	return 0;
}

// WM_PAINTメッセージを処理する
afx_msg void CMainWin::OnPaint() {
	CPaintDC dc(this);

	dc.FillSolidRect(0, 0, 500, 500, RGB(0, 0, 0));

	// フォント設定
//	CFont *pFont;
	CFont font;

	// フォントを作成する
	font.CreateFont(
		32,                   // フォントの高さ(大きさ)。
		0,                    // フォントの幅。普通０。
		0,                    // 角度。０でＯＫ。
		0,                    // 同じく角度。これも０。
		FW_EXTRABOLD,          // 文字の太さ。
		FALSE,                // フォントがイタリックならTRUEを指定。
		FALSE,                // 下線を引くならTRUE。
		FALSE,                // 取り消し線を引くならTRUE。
		ANSI_CHARSET,     // フォントの文字セット。このままでＯＫ。
		OUT_DEFAULT_PRECIS,   // 出力精度の設定。このままでＯＫ。
		CLIP_DEFAULT_PRECIS,  // クリッピング精度。このままでＯＫ。
		DEFAULT_QUALITY,        // フォントの出力品質。このままでＯＫ。
		DEFAULT_PITCH,        // フォントのピッチとファミリを指定。このままでＯＫ。
		_T("Ricty Diminished Discord") // フォントのタイプフェイス名の指定。これは見たまんま。
	);
	/*pFont = */dc.SelectObject(&font);        // フォントを設定。
	dc.SetTextColor(RGB(63, 127, 255));

	// 文字を再表示
	dc.TextOutW(1, 1, str, _tcslen(str));
}

afx_msg void CMainWin::OnLButtonUp(UINT Flags, CPoint loc) {
	SetWindowPos(&CMainWin::wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetTimer(ID_TIMER::SETTOPMOST, 5000, NULL);
	isTopMost = FALSE;

}

afx_msg void CMainWin::OnLButtonDown(UINT Flags, CPoint loc) {
	// タイトルバー以外の部分をドラッグしてウィンドウを移動する。
	this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(loc.x, loc.y));
}

// WM_TIMERメッセージを処理する
afx_msg void CMainWin::OnTimer(UINT ID) {
	if (ID == ID_TIMER::SETTOPMOST) {
		KillTimer(ID_TIMER::SETTOPMOST);
		isTopMost = TRUE;
	}
	else if (ID == ID_TIMER::REPAINT && isTopMost == TRUE) {
		// 常に最前面とする
		SetWindowPos(&CMainWin::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		CTime curtime = CTime::GetCurrentTime();
		struct tm newtime;

		curtime.GetLocalTm(&newtime);
		wsprintf(str, _T("%02d:%02d:%02d"), newtime.tm_hour, newtime.tm_min, newtime.tm_sec);

		InvalidateRect(NULL, 0);
	}
}

// アプリケーションを終了する
afx_msg void CMainWin::OnDestroy() {
	KillTimer(ID_TIMER::REPAINT);
}

// アプリケーションのインスタンス
CApp App;
