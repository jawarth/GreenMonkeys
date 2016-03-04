#include "Visual.h"

Visual::Visual():pop(pop){
	hWnd = NULL;
	hInstance = NULL;
	hDc = NULL;
}

Visual::Visual(std::vector<std::shared_ptr<Animal>> &incPop) : pop(std::move(incPop)){
	hWnd = NULL;
	hInstance = NULL;
	hDc = NULL;
}

void Visual::visualSetup(){
	LPCSTR myClass = "Green Monkeys";
	LPCSTR myTitle = "Natural Selection Simulator";
	

	WNDCLASS wndclass = { CS_HREDRAW | CS_VREDRAW, (WNDPROC)StaticWndProc, 0, 0, 
		hInstance, LoadIcon(NULL,IDI_WINLOGO), LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) GetStockObject(WHITE_BRUSH), myTitle, myClass};

	if (RegisterClass(&wndclass))
	{
		hWnd = CreateWindowEx(0, myClass, myTitle,
			WS_BORDER | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
			800, 600, NULL, NULL, hInstance, this);
		
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
		//SetTimer(hWnd, 1, 3, NULL);
		SetTimer(hWnd, 2, 5000, NULL);
		SetTimer(hWnd, 3, 150, NULL);
		/*SetTimer(hWnd, 4, 10000, NULL);*/
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

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		case 2:
			/*Possible timer to update animals position and such. Mostly for testing right now*/
			animalPosUpdate();
			/*InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);*/
			break;
		case 3:
			animalIncUpdate();
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		/*case 4:
			GA g = GA();
			g.combination(pop, pop.size(), 0);
			g.ranking(pop);
			g.selection(pop);
			break;*/
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		/*Window here*/

		//code to plot points of animal locations called here
		if (firstTime)
			initialPopPlot(hdc, hWnd);
		else
			paintAnimals(hdc, hWnd);

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
	GAUtils gau = GAUtils();
	for (const auto& animal : pop){
		int randx = gau.randIntGen(600);
		int randy = gau.randIntGen(500);
			
		if (gau.randIntGen(10) >= 5)
			animal->setPosXOffset(randx);
		else
			animal->setPosXOffset(randx*-1);

		if (gau.randIntGen(10) >= 5)
			animal->setPosYOffset(randy);
		else
			animal->setPosYOffset(randy*-1);
	}
	
}


void Visual::initialPopPlot(HDC hdc, HWND hWnd) {
	GAUtils gau = GAUtils();
	startX = gau.randIntGen(100, 300);
	startY = gau.randIntGen(100, 300);
	int x = 1;
	int y = pop.size();
	int z = 0;
	y /= 9;

	while (y != 0){
		for (int i = 0; i < 9; i++){
			if (i == 0){
				TextOut(hdc, startX, startY, ".", 1);
				pop[i + z]->setXPosition(startX);
				pop[i + z]->setYPosition(startY);
			}
			else if (i == 1){
				TextOut(hdc, startX + x, startY, ".", 1);
				pop[i + z]->setXPosition(startX+x);
				pop[i + z]->setYPosition(startY);
			}
			else if (i == 2){
				TextOut(hdc, startX + x, startY + x, ".", 1);
				pop[i + z]->setXPosition(startX + x);
				pop[i + z]->setYPosition(startY + x);
			}
			else if (i == 3){
				TextOut(hdc, startX, startY + x, ".", 1);
				pop[i + z]->setXPosition(startX);
				pop[i + z]->setYPosition(startY + x);
			}
			else if (i == 4){
				TextOut(hdc, startX - x, startY + x, ".", 1);
				pop[i + z]->setXPosition(startX - x);
				pop[i + z]->setYPosition(startY + x);
			}
			else if (i == 5){
				TextOut(hdc, startX - x, startY, ".", 1);
				pop[i + z]->setXPosition(startX - x);
				pop[i + z]->setYPosition(startY);
			}
			else if (i == 6){
				TextOut(hdc, startX - x, startY - x, ".", 1);
				pop[i + z]->setXPosition(startX - x);
				pop[i + z]->setYPosition(startY - x);
			}
			else if (i == 7){
				TextOut(hdc, startX, startY - x, ".", 1);
				pop[i + z]->setXPosition(startX);
				pop[i + z]->setYPosition(startY - x);
			}
			else if (i == 8){
				TextOut(hdc, startX + x, startY - x, ".", 1);
				pop[i + z]->setXPosition(startX + x);
				pop[i + z]->setYPosition(startY - x);
			}
		}
		y--;
		x += 1;
		z += 9;
	}
	firstTime = false;
	animalPosUpdate();
}


void Visual::paintAnimals(HDC hdc, HWND hWnd){

	int xpos, ypos;
	for (const auto& animal : pop){
		
		xpos = animal->getXPos();
		ypos = animal->getYPos();

		if ((xpos > 51) && (ypos > 51) && (ypos < 501) && (xpos < 701))
			TextOut(hdc,xpos, ypos, ".", 1);
		else if ((xpos < 51) && (ypos > 51))
			TextOut(hdc, 50, ypos, ".", 1);
		else if ((ypos < 51) && (xpos > 51))
			TextOut(hdc, xpos, 50, ".", 1);
		else if ((ypos < 51) && (xpos < 51))
			TextOut(hdc, 51, 51, ".", 1);
		else if ((ypos > 501) && (xpos < 701))
			TextOut(hdc, xpos, 500, ".", 1);
		else if ((ypos < 501) && (xpos > 701))
			TextOut(hdc, 700, ypos, ".", 1);

		else
			TextOut(hdc, 699, 499, ".", 1);
		
	}
}

void Visual::animalIncUpdate(){
	for (const auto& animal : pop){
		if (animal->getPosXOffset() > 0){
			animal->setPosXOffset(animal->getPosXOffset() - 1);
			animal->setXPosition(animal->getXPos() + 1);
		}
		else{
			animal->setPosXOffset(animal->getPosXOffset() + 1);
			animal->setXPosition(animal->getXPos() - 1);
		}

		if (animal->getPosYOffset() > 0){
			animal->setPosYOffset(animal->getPosYOffset() - 1);
			animal->setYPosition(animal->getYPos ()+ 1);
		}
		else{
			animal->setPosYOffset(animal->getPosYOffset() + 1);
			animal->setYPosition(animal->getYPos() - 1);
		}
	}
}