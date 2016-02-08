#include "Visual.h"

Visual::Visual():pop(pop){
	hWnd = NULL;
	hInstance = NULL;
	hDc = NULL;
}

Visual::Visual(std::vector<Animal> &incPop): pop(incPop){
	hWnd = NULL;
	hInstance = NULL;
	hDc = NULL;
}

void Visual::visualSetup(){
	LPCWSTR myClass = L"Green Monkeys";
	LPCWSTR myTitle = L"Natural Selection Simulator";
	

	WNDCLASS wndclass = { CS_HREDRAW | CS_VREDRAW, (WNDPROC)StaticWndProc, 0, 0, 
		hInstance, LoadIcon(NULL,IDI_WINLOGO), LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) GetStockObject(WHITE_BRUSH), myTitle, myClass};

	if (RegisterClass(&wndclass))
	{
		hWnd = CreateWindowEx(0, myClass, myTitle,
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, this);
		
		if (hWnd)
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);
			MSG msg; 
			while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
		}
	}
}

LRESULT CALLBACK Visual::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Visual* pParent;

	// Get pointer to window
	if (uMsg == WM_CREATE)
	{
		pParent = (Visual*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pParent);
		SetTimer(hWnd, 1, 20, NULL);
		SetTimer(hWnd, 2, 2000, NULL);
	}
	else
	{
		pParent = (Visual*)GetWindowLongPtr(hWnd, GWL_USERDATA);
		if (!pParent) return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	pParent->hWnd = hWnd;
	return pParent->WndProc(uMsg, wParam, lParam);
}


LRESULT Visual::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;


	switch (uMsg)
	{
	case WM_CREATE:
		//SetTimer(hWnd, 1, 20, NULL);
		//SetTimer(hWnd, 2, 2000, NULL);
		break;
	case WM_TIMER:
		switch (wParam){
		case 1:
			/*This will be used to update the window to give the illusion of realtime movement*/

			//InvalidateRect(hWnd, NULL, TRUE);
			//UpdateWindow(hWnd);
			return 0;
		case 2:
			/*Possible timer to update animals position and such. Mostly for testing right now*/
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			return 0;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		/*Window here*/

		//code to plot points of animal locations called here
	    paintAnimals(hdc, hWnd);
		Ellipse(hdc, 90, 90, 110, 110);
		
		x += 5;
		TextOut(hdc, x, x, L"X", 1);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return (UINT)0;
}

void Visual::animalPosUpdate(){
	
}


//ATTENTION: JAMES WRATH.. HOW WILL THIS CALL ACCESS THE POPULATION VECTOR?

//function must accept starting point for first individual.. random x and y?
//function must also accept population vector.. how will this call access it?
void Visual::initialPopPlot(HDC hdc, HWND hWnd) {

	//need some global data structure (vector) of every individual's current location
	//must reference this structure to see what positions are occupied
 
	//plot first amimal at p with 1 px ellipse

	//	for each animal in population

	//		while adjacent pixel x is not empty

	//			increment p.x

	//			while adjacent pixel y is not empty

	//				increment p.y

	//				plot ellipse(p), or TextOut an x at position p, whateve

}

void Visual::paintAnimals(HDC hdc, HWND hWnd){
	std::wstring stemp = std::wstring(pop[0].tag.begin(), pop[0].tag.end());
	LPCWSTR sw = stemp.c_str();
	TextOut(hdc, 50, 50, sw, pop[0].tag.size());
}

