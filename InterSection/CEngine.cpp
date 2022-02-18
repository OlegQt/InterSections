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

	//code below should be in logic class
	this->Aparticle.pos = { 50.0f,100.0f };
	this->Aparticle.V = { 0.01f,0.01f };
	this->Aparticle.diameter = 10;
	this->Aparticle.mass = 10;

	this->Bparticle.pos = { 120.0f,100.0f };
	this->Bparticle.V = { -0.1f,0.0f };
	this->Bparticle.diameter = 20;
	this->Bparticle.mass = 20;
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
	this->hWnd = CreateWindow(L"D2DDemoApp", L"app D2D", WS_OVERLAPPED | WS_SYSMENU | WS_SIZEBOX, 0, 0, 500, 500, NULL, NULL, this->hInst, this);
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
	if (message == WM_SIZE)
	{
		//this->DiscardDeviceResources();
		this->ResizeTarget();


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
	if (this->pRenderTarget) return S_FALSE; // If render target is already created

	// Get windows size into RECT struct
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
void Engine::ResizeTarget()
{
	if (this->pRenderTarget)
	{

		// Get windows size into RECT struct
		RECT rc;
		GetClientRect(this->hWnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		// Resuze Direct2D render target
		this->pRenderTarget->Resize(size);
		InvalidateRect(this->hWnd, NULL, FALSE);
	}

}

HRESULT Engine::Render()
{
	this->VectorArray.clear();
	this->CalcilateLogic();
	HRESULT hr = this->CreateTarget();
	if (FAILED(hr)) return hr;
	if (!m_pPathGeometry) return S_FALSE;

	this->pRenderTarget->BeginDraw();
	this->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	this->pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));
	// Draw here
	this->RenderParticle(&this->Aparticle);
	this->RenderParticle(&this->Bparticle);



	for (int iter = 0; iter < VectorArray.size(); iter++)
	{
		this->RenderRline(&VectorArray.at(iter));
	}
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
void Engine::RenderParticle(element* pE)
{
	// pE - pointer to Element
	if (pE->mass == 0) this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkRed));
	this->pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(pE->pos.x, pE->pos.y), pE->diameter, pE->diameter), this->pBrush, 1.0f, NULL);
}
void Engine::RenderRline(rLine* pL)
{
	float LineWidth = 1.0f;
	if (pL->type == BOND)
	{
		this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Firebrick));
		LineWidth = 2.0f;
	}
	else if (pL->type == VELOCITY) this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkBlue));
	else if (pL->type == PROECTION) this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	this->pRenderTarget->DrawLine(
		D2D1::Point2F(pL->a.x, pL->a.y),
		D2D1::Point2F(pL->b.x, pL->b.y),
		this->pBrush, LineWidth, NULL);
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


void Engine::CalcilateLogic()
{

	rvector AB{ 0,0 };
	rvector V{ 0,0 };
	rvector Offset{ 0,0 };
	float betta = 0;
	rLine line;

	AB = CMatrix::SumVectors(Aparticle.pos, Bparticle.pos, -1);
	if (btnA.pushed)
	{

		if (CMatrix::GetVectorLength(AB) <= Aparticle.diameter + Bparticle.diameter + 0.0f)
		{

			betta = CMatrix::GetAngle(AB, { 1.0f,0.0f }); // Calculate angle between radius vector from A-point ro B-point and OX axis
			rvector Va = Aparticle.V; // Take the real velocity  vector in real OXY
			rvector Vb = Bparticle.V; // Take the real velocity vector in real OXY
			Va = CMatrix::Rotate(Va, betta); // Rotate velocity (x component is the proection the real velocity on the AB-vector)
			Vb = CMatrix::Rotate(Vb, betta); // Rotate velocity (x component is the proection the real velocity on the AB-vector)

			float DeltaMass = Bparticle.mass - Aparticle.mass;
			float SummMass = Bparticle.mass + Aparticle.mass;
			float Vax = (2 * Bparticle.mass * Vb.x - Va.x * DeltaMass) / SummMass; // Calculate Vx for A-particle after collusion
			float Vbx = (2 * Aparticle.mass * Va.x + Vb.x * DeltaMass) / SummMass; // Calculate Vx for B-particle after collusion
			Va.x = Vax;
			Vb.x = Vbx;

			Aparticle.V = CMatrix::Rotate(Va, -betta); // Rotate back to take new velocities after collusion
			Bparticle.V = CMatrix::Rotate(Vb, -betta); // Rotate back to take new velocities after collusion

			// If both particles stuck in each others
			// Calculate the overlay distance and moove each particl with its new speed on the half of this distancce
			
			float delta = CMatrix::GetVectorLength(AB) - (Aparticle.diameter + Bparticle.diameter); // Calculate the overlay
			if (delta < 1.0f) delta = 1.0f;
			float time = (0.5f * delta) / CMatrix::GetVectorLength(Aparticle.V); //Calculate time to moove from stuck

		
			Aparticle.pos.x = Aparticle.pos.x + (time * Aparticle.V.x); 
			Aparticle.pos.y = Aparticle.pos.y + (time * Aparticle.V.y);

			Bparticle.pos.x = Bparticle.pos.x + (time * Bparticle.V.x);
			Bparticle.pos.y = Bparticle.pos.y + (time * Bparticle.V.y);
		}
		else
		{
			this->Aparticle.pos.x += Aparticle.V.x;
			this->Aparticle.pos.y += Aparticle.V.y;

			this->Bparticle.pos.x += Bparticle.V.x;
			this->Bparticle.pos.y += Bparticle.V.y;
		}
	}



	line = { Bparticle.pos,Aparticle.pos ,BOND };
	this->VectorArray.push_back(line);


	V = Aparticle.V;
	Offset = Aparticle.pos;
	V = CMatrix::ScaleVector(V, 2.1f);
	V = CMatrix::SumVectors(V, Offset, 1);
	line = { Offset ,V,VELOCITY };
	this->VectorArray.push_back(line);

	V = Bparticle.V;
	Offset = Bparticle.pos;
	V = CMatrix::ScaleVector(V, 2.1f);
	V = CMatrix::SumVectors(V, Offset, 1);
	line = { Offset ,V,VELOCITY };
	this->VectorArray.push_back(line);

	//Draw proections A velosity
	//Offset (300,300)
	Offset = { 300.0f,300.0f };
	//Offset = Aparticle.pos;
	V = Aparticle.V;
	V = CMatrix::Rotate(V, betta);
	V = CMatrix::ScaleVector(V, 2.1f);
	V = CMatrix::SumVectors(V, Offset, 1);
	line = { Offset ,V,PROECTION };
	this->VectorArray.push_back(line);


	V = CMatrix::Rotate(AB, betta);
	V = CMatrix::SumVectors(V, Offset, 1);
	line = { Offset,V,PROECTION };
	this->VectorArray.push_back(line);

	//Draw proections A velosity
	//Offset (300,300)
	Offset = CMatrix::SumVectors(Offset, CMatrix::Rotate(AB, betta), 1);
	//Offset = CMatrix::SumVectors(Aparticle.pos, CMatrix::Rotate(AB, betta), 1);
	V = Bparticle.V;
	V = CMatrix::Rotate(V, betta);
	V = CMatrix::ScaleVector(V, 2.1f);
	V = CMatrix::SumVectors(V, Offset, 1);
	line = { Offset ,V,PROECTION };
	this->VectorArray.push_back(line);



}