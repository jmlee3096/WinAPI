#pragma once

#include "resource.h"
#include "math.h"
#include <stdio.h>

#define PI							3.1416
#define DRONE1_SPEED				1.0
#define DRONE2_SPEED				0.7
#define DRONE_MODE					2

class MyDrone
{
public:
	MyDrone();
	MyDrone(int mode, double speed);

	int drone_id_;
	int cluster_id_;
	int mode_;

	int x_;
	int y_;

	double radian_;
	double degree_;
	double speed_;
};

MyDrone::MyDrone() {
	drone_id_ = 1;
	cluster_id_ = 1;
	mode_ = 1;

	x_ = 0;
	y_ = 0;

	degree_ = 0;
	radian_ = 0;
	speed_ = 0.7;
};

MyDrone::MyDrone(int mode, double speed) {
	drone_id_ = 1;
	cluster_id_ = 1;
	mode_ = mode;

	x_ = 0;
	y_ = 0;

	degree_ = 0;
	radian_ = 0;
	speed_ = speed;
};

void MoveDrone(HDC hdc, RECT rcClient, HINSTANCE g_Instance, int* x3, int* y3, double* radian3, double speed, double* deg3) {

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(memDC, hbmBuffer);

	FillRect(hdcBuffer, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

	*radian3 = *deg3 * PI / 180;

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

void MoveDroneMode1(HDC hdc, RECT rcClient, HINSTANCE g_Instance, MyDrone *drone1, MyDrone *drone2) {

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBuffer);

	//	Radian값으로 변환
	drone1->radian_ = drone1->degree_ * PI / 180;
	drone2->radian_ = drone2->degree_ * PI / 180;

	drone1->x_ = (100 * sin(drone1->radian_)) + 200;
	drone1->y_ = (100 * cos(drone1->radian_)) + 100;

	drone2->x_ = (100 * sin(drone2->radian_)) + 500;
	drone2->y_ = (100 * cos(drone2->radian_)) + 100;

	drone1->degree_ += drone1->speed_;
	drone2->degree_ += drone2->speed_;

	HBITMAP myBitmap = LoadBitmap(g_Instance, MAKEINTRESOURCE(IDB_DRONESMALL));

	SelectObject(hdcMem, myBitmap);

	//	흰색으로 바탕깔기('원래'있던거 지워줌)
	FillRect(hdcBuffer, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

	//	바탕에 드론그리기
	BitBlt(hdcBuffer, drone1->x_, drone1->y_, 100, 100, hdcMem, 0, 0, SRCAND);
	BitBlt(hdcBuffer, drone2->x_, drone2->y_, 100, 100, hdcMem, 0, 0, SRCAND);

	//	완성된 그림 그려줌
	BitBlt(hdc, 0, 100, rcClient.right, rcClient.bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);

	DeleteDC(hdcMem);
	DeleteDC(hdcBuffer);

	DeleteObject(myBitmap);
	DeleteObject(hbmBuffer);
}

void MoveDroneMode2(HDC hdc, RECT rcClient, HINSTANCE g_Instance, MyDrone *drone1, MyDrone *drone2) {

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBuffer);

	drone1->radian_ = drone1->degree_ * PI / 180;
	drone2->radian_ = drone2->degree_ * PI / 180;

	drone1->x_ = (100 * sin(drone1->radian_)) + 200;
	drone1->y_ = (100 * cos(drone1->radian_)) + 100;

	drone2->x_ = (100 * sin(drone2->radian_)) + 500;
	drone2->y_ = (100 * cos(drone2->radian_)) + 100;

	if(drone1->degree_ <90)
		drone1->degree_ += drone1->speed_;
	if(drone2->degree_ <270)
		drone2->degree_ += drone2->speed_;

	HBITMAP myBitmap = LoadBitmap(g_Instance, MAKEINTRESOURCE(IDB_DRONESMALL));

	SelectObject(hdcMem, myBitmap);

	FillRect(hdcBuffer, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

	BitBlt(hdcBuffer, drone1->x_, drone1->y_, 100, 100, hdcMem, 0, 0, SRCAND);
	BitBlt(hdcBuffer, drone2->x_, drone2->y_, 100, 100, hdcMem, 0, 0, SRCAND);
	BitBlt(hdc, 0, 100, rcClient.right, rcClient.bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);

	DeleteDC(hdcMem);
	DeleteDC(hdcBuffer);

	DeleteObject(myBitmap);
	DeleteObject(hbmBuffer);
}

void MoveDroneMode3(HDC hdc, RECT rcClient, HINSTANCE g_Instance, MyDrone *drone1, MyDrone *drone2) {

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBuffer);

	drone1->radian_ = drone1->degree_ * PI / 180;
	drone2->radian_ = drone2->degree_ * PI / 180;

	drone1->x_ = (100 * sin(drone1->radian_)) + 200;
	drone1->y_ = (100 * cos(drone1->radian_)) + 100;

	drone2->x_ = (100 * sin(drone2->radian_)) + 500;
	drone2->y_ = (100 * cos(drone2->radian_)) + 100;

	drone1->degree_ += drone1->speed_;
	drone2->degree_ += drone2->speed_;

	HBITMAP myBitmap = LoadBitmap(g_Instance, MAKEINTRESOURCE(IDB_DRONESMALL));

	SelectObject(hdcMem, myBitmap);

	FillRect(hdcBuffer, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

	BitBlt(hdcBuffer, drone1->x_, drone1->y_, 100, 100, hdcMem, 0, 0, SRCAND);
	BitBlt(hdcBuffer, drone2->x_, drone2->y_, 100, 100, hdcMem, 0, 0, SRCAND);
	BitBlt(hdc, 0, 100, rcClient.right, rcClient.bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);

	DeleteDC(hdcMem);
	DeleteDC(hdcBuffer);

	DeleteObject(myBitmap);
	DeleteObject(hbmBuffer);
}

void MoveDroneMode4(HDC hdc, RECT rcClient, HINSTANCE g_Instance, MyDrone *drone1, MyDrone *drone2) {

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBuffer);

	drone1->radian_ = drone1->degree_ * PI / 180;
	drone2->radian_ = drone2->degree_ * PI / 180;

	drone1->x_ = (100 * sin(drone1->radian_)) + 200;
	drone1->y_ = (100 * cos(drone1->radian_)) + 100;

	drone2->x_ = (100 * sin(drone2->radian_)) + 500;
	drone2->y_ = (100 * cos(drone2->radian_)) + 100;

	drone1->degree_ += drone1->speed_;
	drone2->degree_ += drone2->speed_;

	HBITMAP myBitmap = LoadBitmap(g_Instance, MAKEINTRESOURCE(IDB_DRONESMALL));

	SelectObject(hdcMem, myBitmap);

	FillRect(hdcBuffer, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

	BitBlt(hdcBuffer, drone1->x_, drone1->y_, 100, 100, hdcMem, 0, 0, SRCAND);
	BitBlt(hdcBuffer, drone2->x_, drone2->y_, 100, 100, hdcMem, 0, 0, SRCAND);
	BitBlt(hdc, 0, 100, rcClient.right, rcClient.bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);

	DeleteDC(hdcMem);
	DeleteDC(hdcBuffer);

	DeleteObject(myBitmap);
	DeleteObject(hbmBuffer);
}
