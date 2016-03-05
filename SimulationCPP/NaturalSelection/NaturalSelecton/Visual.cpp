#include "Visual.h"

/*COMMENTS:
*  Need this inorder to create a pointer to this class in the static version of windowprodecure.3
*  Not going to use pop in this at all, so setting pop equal to its self is safe in this instance.
*/
Visual::Visual():pop(pop){
	hWnd = NULL;
	hInstance = NULL;
	hDc = NULL;
}

/*COMMENTS:
*  The constructor that will be called from outside this class. Using an initializer list more for style and avoiding possible complicated code.
*  In earlier development this did not use an initializer list and the program quickly ran into problems setting pop equal to incPop in the body of the function.
*  Not sure what caused this, but it was fixed with an initalizer list.
*/
Visual::Visual(std::vector<std::shared_ptr<Animal>> &incPop) : pop(std::move(incPop)){
	hWnd = NULL;
	hInstance = NULL;
	hDc = NULL;
}

/*COMMENTS:
*  Just basic window creation and registration with a basic msg handler loop. Nothing too fancy. 
*/
void Visual::visualSetup(){
	LPCSTR myClass = "Green Monkeys";
	LPCSTR myTitle = "Natural Selection Simulator";
	std::cout << pop.size();

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

/*COMMENTS:
*  This was a big pain to get working. Finally got it to work after a long time resraching.
*  So windows api only allows the main window procedure to be static. But since we are using an external data structure
*  this proved to be a problem. The way this problem was solved was by creating a pointer to the class and a pointer to the window.
*  The window pointer is then used to initalize the class pointer (class pointer still has access to the static window). With this we are able to call functions via the class pointer.
*  And since we are able to call our second window procedure, which is not static, and use our external data members. 
*  This whole process allows us to bypass the static window procedure. 
*  Again this was a big pain to get working.
*/
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
		SetTimer(hWnd, 3, 125, NULL);
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

/*COMMENTS:
*  Non-static version of the window procedure. 
*  May need to edit the timers a bit more. 
*  Also need to remove the first timer as it is not being used anymore.
*/
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

/*COMMENTS:
*  This will need to change or be re-written once we add in prey/a grid 2d array. 
*  Currently it is giving the animal a random direction to "roam" in.
*/
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

/*COMMENTS:
*  Generates the first position and starting place for each animal.
*  It spawns each animal in a box formation. Each with their own unique position. This is done so there is no outliers when spawning (i.e. they're all grouped up).
*  Note, when we add in prey/predator we may need to space them out a bit more to avoid a feeding frenzy and possible genocide.
*  Probably should create their position in the GA instead of here, but this works for now.
*  Also this is causing some errors when the pop is < 9 and some of the aniamls are not getting an initial position. Need to rework this loop
*/
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

/*COMMENTS:
*  Basic painting and collision detection done in one function.
*  Did this in one function becuase the collision is fairly small and fits right in with the paiting.
*/
void Visual::paintAnimals(HDC hdc, HWND hWnd){

	
	for (const auto& animal : pop){
		int xpos, ypos;
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

/*COMMENTS:
*  Possible future work: Instead of each animal moving 1 pixel at a time have them move at their own speed.
*  Would need to test this though. Could lead to a "skipping" effect on the screen.
*/
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