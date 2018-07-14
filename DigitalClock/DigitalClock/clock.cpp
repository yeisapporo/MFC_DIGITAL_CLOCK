// ���v�v���O����

#include <afxwin.h>
#include <cstring>
#include <ctime>
#include "clock.h"

// �o�͕������ێ�����
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

// �A�v���P�[�V����������������
BOOL CApp::InitInstance() {
	m_pMainWnd = new CMainWin;
	// �ĕ`��p�^�C�}�[��ݒ肷��
	m_pMainWnd->SetTimer(ID_TIMER::REPAINT, 500, NULL);
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

// �A�v���P�[�V�����̃��b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// ���[�W�����ݒ�
afx_msg int CMainWin::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CRgn rgn;
	rgn.CreateRectRgn(10, 33, 170, 62);
	SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);

	// ��(0, 0, 0)�̗̈�𓧖��ɂ���
	LONG exStyle = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_COMPOSITED);
	this->SetLayeredWindowAttributes(RGB(0, 0, 0), 0, LWA_COLORKEY/*LWA_ALPHA*/);

	return 0;
}

// WM_PAINT���b�Z�[�W����������
afx_msg void CMainWin::OnPaint() {
	CPaintDC dc(this);

	dc.FillSolidRect(0, 0, 500, 500, RGB(0, 0, 0));

	// �t�H���g�ݒ�
//	CFont *pFont;
	CFont font;

	// �t�H���g���쐬����
	font.CreateFont(
		32,                   // �t�H���g�̍���(�傫��)�B
		0,                    // �t�H���g�̕��B���ʂO�B
		0,                    // �p�x�B�O�łn�j�B
		0,                    // �������p�x�B������O�B
		FW_EXTRABOLD,          // �����̑����B
		FALSE,                // �t�H���g���C�^���b�N�Ȃ�TRUE���w��B
		FALSE,                // �����������Ȃ�TRUE�B
		FALSE,                // ���������������Ȃ�TRUE�B
		ANSI_CHARSET,     // �t�H���g�̕����Z�b�g�B���̂܂܂łn�j�B
		OUT_DEFAULT_PRECIS,   // �o�͐��x�̐ݒ�B���̂܂܂łn�j�B
		CLIP_DEFAULT_PRECIS,  // �N���b�s���O���x�B���̂܂܂łn�j�B
		DEFAULT_QUALITY,        // �t�H���g�̏o�͕i���B���̂܂܂łn�j�B
		DEFAULT_PITCH,        // �t�H���g�̃s�b�`�ƃt�@�~�����w��B���̂܂܂łn�j�B
		_T("Ricty Diminished Discord") // �t�H���g�̃^�C�v�t�F�C�X���̎w��B����͌����܂�܁B
	);
	/*pFont = */dc.SelectObject(&font);        // �t�H���g��ݒ�B
	dc.SetTextColor(RGB(63, 127, 255));

	// �������ĕ\��
	dc.TextOutW(1, 1, str, _tcslen(str));
}

afx_msg void CMainWin::OnLButtonUp(UINT Flags, CPoint loc) {
	SetWindowPos(&CMainWin::wndBottom, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetTimer(ID_TIMER::SETTOPMOST, 5000, NULL);
	isTopMost = FALSE;

}

afx_msg void CMainWin::OnLButtonDown(UINT Flags, CPoint loc) {
	// �^�C�g���o�[�ȊO�̕������h���b�O���ăE�B���h�E���ړ�����B
	this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(loc.x, loc.y));
}

// WM_TIMER���b�Z�[�W����������
afx_msg void CMainWin::OnTimer(UINT ID) {
	if (ID == ID_TIMER::SETTOPMOST) {
		KillTimer(ID_TIMER::SETTOPMOST);
		isTopMost = TRUE;
	}
	else if (ID == ID_TIMER::REPAINT && isTopMost == TRUE) {
		// ��ɍőO�ʂƂ���
		SetWindowPos(&CMainWin::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		CTime curtime = CTime::GetCurrentTime();
		struct tm newtime;

		curtime.GetLocalTm(&newtime);
		wsprintf(str, _T("%02d:%02d:%02d"), newtime.tm_hour, newtime.tm_min, newtime.tm_sec);

		InvalidateRect(NULL, 0);
	}
}

// �A�v���P�[�V�������I������
afx_msg void CMainWin::OnDestroy() {
	KillTimer(ID_TIMER::REPAINT);
}

// �A�v���P�[�V�����̃C���X�^���X
CApp App;
