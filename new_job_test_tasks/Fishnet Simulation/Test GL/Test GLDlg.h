
// Test GLDlg.h : header file
//

#pragma once
#include "PBDynamics/Mesh.h"

using std::tr1::shared_ptr;

// CTestGLDlg dialog
class CTestGLDlg : public CDialogEx
{
// Construction
public:
	CTestGLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTGL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	koval::PBDynamics::Vector2D client2GLCoord(CPoint pt);

private:
	void Init();
	BOOL bSetupPixelFormat();
	void DrawScene();
	void ClearOpenGLContext();

private:
	CDC* m_pDC;

	bool m_IsLButtonDown;
	koval::PBDynamics::Vector2D m_MousePos;
	size_t m_CapturedNode;

	shared_ptr<koval::PBDynamics::Mesh> m_pMesh;
};
