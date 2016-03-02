#pragma once
#ifndef VISUAL_H
#define VISUAL_H
#include <windows.h>
#include <stdlib.h>
#include <string.h>
//#include <char.h>
#include <memory>
#include <iostream>
#include "GA.h"


class Visual {
public:
	Visual();
    Visual(std::vector<Animal> &incPop);
	void visualSetup();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	HWND hWnd;
	HINSTANCE hInstance;
	HDC hDc;

private:
	int startX, startY;
	bool firstTime = true;
	std::vector<Animal> &pop;
    void animalPosUpdate();
	void paintAnimals(HDC hdc, HWND hWnd);
	void initialPopPlot(HDC hdc, HWND hWnd);
	void animalIncUpdate();
};


#endif