#include "CEngine.h"

// Комментарий для компоновщика
#pragma comment(lib,"d2d1.lib")

Engine::Engine(HINSTANCE h)
{
	hWnd = NULL;
	this->hInst = h;

	this->m_pDirect2dFactory = NULL;
	this->pRenderTarget = NULL;
	this->m_pPathGeometry = NULL;
	this->pBrush = NULL;
	this->pStroke = NULL;
	this->pSink = NULL;


	this->btnA = { 10,10,20,20 };
}
Engine::~Engine()
{
	if (this->m_pDirect2dFactory)
	{
		this->m_pDirect2dFactory->Release();
		this->m_pDirect2dFactory = NULL;
	}
	if (this->pBrush)
	{
		this->pBrush->Release();
		this->pBrush = NULL;
	}
	if (this->pRenderTarget)
	{
		this->pRenderTarget->Release();
		this->pRenderTarget = NULL;
	}

	if (this->pStroke)
	{
		this->pStroke->Release();
		this->pStroke = nullptr;
	}
	if (this->m_pPathGeometry)
	{
		this->m_pPathGeometry->Release();
		this->m_pPathGeometry = nullptr;
	}
	if (this->pSink)
	{
		this->pSink->Release();
		this->pSink = nullptr;
	}
}

HRESULT Engine::Initialize()
{
	HRESULT hr = S_OK;

	// Initialize device-indpendent resources, such as the Direct2D factory
	hr = this->CreateDeviceIndependentResources();
	if (!SUCCEEDED(hr))return hr;
	else {

		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)this->WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = this->hInst;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DDemoApp";

		if (!RegisterClassEx(&wcex)) return S_FALSE;
	}
	// Create the window.
	//FLOAT dpiX, dpiY;
	//m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
	this->hWnd = CreateWindow(L"D2DDemoApp", L"app D2D", WS_OVERLAPPED | WS_SYSMENU, 0, 0, 500, 500, NULL, NULL, this->hInst, this);
	if (!this->hWnd) return S_FALSE;
	SetTimer(hWnd, TIMER1, 10, NULL);
	CreateWindow(L"Button", L"but", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		this->btnA.l, this->btnA.t, this->btnA.w, this->btnA.h,
		this->hWnd, (HMENU)ID_BUTTON_A, this->hInst, NULL);

	if (SUCCEEDED(hr))
	{
		ShowWindow(this->hWnd, SW_SHOWNORMAL);
		UpdateWindow(this->hWnd);
		this->RunMessageLoop();
	}
	return hr;
}
void Engine::RunMessageLoop()
{
	MSG msg;
	msg.message = WM_NULL; //initialize variable msg
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			this->Render();
		}
	}

}
LRESULT Engine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Engine* pThis;
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<Engine*>(lpcs->lpCreateParams);
		// Put the value in a safe place for future use
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		// Recover the "this" pointer from where our WM_NCCREATE handler stashed it.
		pThis = reinterpret_cast<Engine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}
	if (pThis) pThis->Procedure(hWnd, message, wParam, lParam);

	return (DefWindowProc(hWnd, message, wParam, lParam));
}
LRESULT Engine::Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)
	{
		PostQuitMessage(NULL);
		return 0;
	}
	if (message == WM_LBUTTONDOWN)
	{
		// Достаем координаты щелчка
		float Xpos, Ypos;
		Xpos = static_cast<float>LOWORD(lParam);
		Ypos = static_cast<float>HIWORD(lParam);
	}
	if (message == WM_MOUSEMOVE)
	{
		// Достаем координаты щелчка
		float Xpos, Ypos;
		Xpos = static_cast<float>LOWORD(lParam);
		Ypos = static_cast<float>HIWORD(lParam);
	}
	if (message == WM_COMMAND)
	{
		if (wParam == ID_BUTTON_A)
		{
			this->btnA.pushed = !this->btnA.pushed;
		}
	}
	if (message == WM_TIMER)
	{
		if (wParam == TIMER1)
		{
			// do something1
			//this->pLogig->RotateStar();
		}
	}
	return S_OK;
}
HRESULT Engine::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	if (FAILED(hr)) return hr;

	// Create Stroke. Its independent too
	D2D1_STROKE_STYLE_PROPERTIES strokeStyleProperties = D2D1::StrokeStyleProperties(
		D2D1_CAP_STYLE_ROUND,  // The start cap.
		D2D1_CAP_STYLE_ROUND,  // The end cap.
		D2D1_CAP_STYLE_ROUND, // The dash cap.
		D2D1_LINE_JOIN_ROUND, // The line join.
		10.0f, // The miter limit.
		D2D1_DASH_STYLE_DOT, // The dash style D2D1_DASH_STYLE_DOT
		0.0f // The dash offset.
	);
	hr = this->m_pDirect2dFactory->CreateStrokeStyle(strokeStyleProperties, NULL, 0, &pStroke);
	if (SUCCEEDED(hr)) hr = this->m_pDirect2dFactory->CreatePathGeometry(&m_pPathGeometry);
	return hr;
}
HRESULT Engine::CreateTarget()
{
	if (this->pRenderTarget) return S_FALSE; // Âûõîäèì, åñëè ñöåíà óæå ñîçäàíà

	// Ïîëó÷àåì ðàçìåðû îêíà
	RECT rc;
	GetClientRect(this->hWnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// Create a Direct2D render target
	HRESULT hr = m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(this->hWnd, size),
		&this->pRenderTarget
	);
	if (SUCCEEDED(hr)) this->pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	if (SUCCEEDED(hr) && !pBrush) hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &this->pBrush);



	return hr;

}
HRESULT Engine::Render()
{
	HRESULT hr = this->CreateTarget();
	if (FAILED(hr)) return hr;
	if (!m_pPathGeometry) return S_FALSE;

	this->pRenderTarget->BeginDraw();
	this->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	this->pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
	// Draw here

	hr = m_pPathGeometry->Open(&pSink);
	// Push geometry to sink
	if (SUCCEEDED(hr))
	{
		pSink->BeginFigure(D2D1::Point2F(100, 100), D2D1_FIGURE_BEGIN_FILLED);
		pSink->AddLine(D2D1::Point2F(200, 100));
		pSink->AddLine(D2D1::Point2F(200, 150));
		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		hr = pSink->Close();		
		if (FAILED(hr)) return hr;		
	}	
	this->pRenderTarget->FillGeometry(m_pPathGeometry, this->pBrush);  // Draw geometry

	// GUI Drawings	
	if (true)
	{
		D2D1_RECT_F btnRect = { this->btnA.l,this->btnA.t,this->btnA.l + this->btnA.w,this->btnA.t + this->btnA.h };
		if (this->btnA.pushed) { this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::AliceBlue)); }
		else { this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray)); }
		this->pRenderTarget->FillRectangle(btnRect, this->pBrush);
		this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
		this->pRenderTarget->DrawRectangle(btnRect, this->pBrush, 2.0f, NULL);
	}

	// up to this end
	hr = this->pRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return S_OK;
}
void Engine::DiscardDeviceResources()
{
	if (this->m_pDirect2dFactory)
	{
		this->m_pDirect2dFactory->Release();
		this->m_pDirect2dFactory = NULL;
	}
	if (this->pBrush)
	{
		this->pBrush->Release();
		this->pBrush = NULL;
	}
	if (this->pRenderTarget)
	{
		this->pRenderTarget->Release();
		this->pRenderTarget = NULL;
	}
	if (this->m_pPathGeometry)
	{
		this->m_pPathGeometry->Release();
		this->m_pPathGeometry = nullptr;
	}
	if (this->pSink)
	{
		this->pSink->Release();
		this->pSink = nullptr;
	}
}
