
// Test GLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test GL.h"
#include "Test GLDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const int  WIDTH_SCREEN = 640;			//if you change this const you must change X_COEF const
const int  HEIGHT_SCREEN = 480;			//if you change this const you must change Y_COEF const
const int  TIMER_TIME = 20;

//coefficients for converting coordinates from client space to GL space
const float X_COEF = 2.35f;				
const float Y_COEF = 1.65f;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestGLDlg dialog
CTestGLDlg::CTestGLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestGLDlg::IDD, pParent)
	, m_pDC(NULL)
	, m_IsLButtonDown(false)
	, m_MousePos (koval::PBDynamics::Vector2D(0.0f, 0.0f))
	, m_CapturedNode(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CTestGLDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CDialog::PreCreateWindow(cs);
}

void CTestGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestGLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CTestGLDlg message handlers

BOOL CTestGLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//{ Init OpenGL
	SetWindowPos(&wndTop, 0, 0, WIDTH_SCREEN, HEIGHT_SCREEN, SWP_NOMOVE);
	m_pDC = GetDC();
	Init();
	SetTimer(1, TIMER_TIME, NULL);
	//} Init OpenGL

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_pMesh = shared_ptr<koval::PBDynamics::Mesh>(new koval::PBDynamics::Mesh());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestGLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CTestGLDlg::OnTimer(UINT nIDEvent)
{
	DrawScene();
	CDialog::OnTimer(nIDEvent);
}

void CTestGLDlg::OnDestroy()
{
	CDialog::OnDestroy();

	ClearOpenGLContext();
}

koval::PBDynamics::Vector2D CTestGLDlg::client2GLCoord(CPoint pt)
{
	CRect rectClient;
	GetClientRect(&rectClient);

	koval::PBDynamics::Vector2D fpt;
	fpt.x = X_COEF * (static_cast<float>(pt.x) /rectClient.right - 0.5f);
	fpt.y = Y_COEF * (static_cast<float>(rectClient.bottom - pt.y) / rectClient.bottom - 0.5f);

	return fpt;
}

void CTestGLDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_IsLButtonDown)
	{
		m_MousePos = client2GLCoord(point);
		m_pMesh->updateNodePosition(m_CapturedNode, m_MousePos);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CTestGLDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_IsLButtonDown = true;
	m_MousePos = client2GLCoord(point);
	m_CapturedNode = m_pMesh->captureNode(m_MousePos);

	CWnd::OnLButtonDown(nFlags, point);
}

void CTestGLDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_IsLButtonDown = false;
	m_pMesh->releaseNode(m_CapturedNode);
	m_CapturedNode = 0;

	CWnd::OnLButtonUp(nFlags, point);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestGLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestGLDlg::Init()
{
	CRect rect;
	HGLRC hrc;
	if (!bSetupPixelFormat())
		return;
	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	ASSERT(hrc != NULL);
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	GetClientRect(&rect);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, static_cast<GLfloat>(rect.right) / 
		static_cast<GLfloat>(rect.bottom), 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_POINT_SMOOTH);						//rounded point
}

BOOL CTestGLDlg::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),		// size of this pfd
		1,									// version number
		PFD_DRAW_TO_WINDOW |				// support window
		PFD_SUPPORT_OPENGL |				// support OpenGL
		PFD_DOUBLEBUFFER,					// double buffered
		PFD_TYPE_RGBA,						// RGBA type
		24,									// 24-bit color depth
		0, 0, 0, 0, 0, 0,					// color bits ignored
		0,									// no alpha buffer
		0,									// shift bit ignored
		0,									// no accumulation buffer
		0, 0, 0, 0,							// accum bits ignored
		32,									// 32-bit z-buffer
		0,									// no stencil buffer
		0,									// no auxiliary buffer
		PFD_MAIN_PLANE,						// main layer
		0,									// reserved
		0, 0, 0								// layer masks ignored
	};

	int pixelformat;

	if ((pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0)
	{
		MessageBox(L"ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox(L"SetPixelFormat failed");
		return FALSE;
	}
	return TRUE;
}

void CTestGLDlg::DrawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.0f);

	using namespace koval::PBDynamics;

	glBegin(GL_LINES);
		for (Mesh::TDistanceConstraint::const_iterator itr = m_pMesh->constraintsItrBegin(); itr != m_pMesh->constraintsItrEnd(); ++itr)
		{
			const DistanceConstraint* pCon = (*itr);
			glVertex2f( pCon->getBeginPoint().x, pCon->getBeginPoint().y);
			glVertex2f( pCon->getEndPoint().x, pCon->getEndPoint().y);
		}
	glEnd();

	glPointSize (4.0f);
	glColor3f (1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
		for (Mesh::TMeshNodes::const_iterator itr = m_pMesh->nodesItrBegin(); itr != m_pMesh->nodesItrEnd(); ++itr)
		{
			Vector2D pt = (*itr)->getNewPosition();
			glVertex2f(pt.x, pt.y);
		}
	glEnd();

	glFinish();
	SwapBuffers(wglGetCurrentDC());

	m_pMesh->update(TIMER_TIME / 1000.0f);
}

void CTestGLDlg::ClearOpenGLContext()
{
	HGLRC hrc;
	KillTimer(1);
	hrc = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL, NULL);
	if (hrc)
		::wglDeleteContext(hrc);
}
