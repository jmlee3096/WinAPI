#pragma once

#include "resource.h"
#include "math.h"
#include <stdio.h>

#define RADIUS						100
#define PI							3.1416
#define DEGREE180					180
#define CLUSTER_SIZE				359
#define CENTER_X					150			
#define CENTER_Y					80

class MyDrone
{
public:
	MyDrone();
	int DroneID;
	int ClusterID;
	double speed;
	int mode;
	int x;
	int y;
	double radian;
	double degree;
};

MyDrone::MyDrone() {
	int DroneID = 1;
	int ClusterID = 1;
	double speed = 0.7;
	int x = 0;
	int y = 0;

	double degree = 0;
	double radian = 0;
};

void MoveDrone(HDC hdc, RECT rcClient, HINSTANCE g_Instance, int* x3, int* y3, double* radian3, double speed, double* deg3) {
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(memDC, hbmBuffer);

	FillRect(hdcBuffer, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

	*radian3 = *deg3 * PI / 180;	//Radian값으로 변환

	*x3 = (100 * sin(*radian3)) + 200;
	*y3 = (100 * cos(*radian3)) + 100;

	*deg3 += speed;

	HBITMAP myBitmap = LoadBitmap(g_Instance, MAKEINTRESOURCE(IDB_DRONESMALL));

	SelectObject(memDC, myBitmap);

	BitBlt(hdcBuffer, *x3, *y3, 100, 100, memDC, 0, 0, SRCAND);

	BitBlt(hdc, 0, 100, rcClient.right, rcClient.bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(memDC, hbmOld);

	DeleteDC(memDC);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);

	DeleteObject(myBitmap);
	DeleteObject(hbmBuffer);
}