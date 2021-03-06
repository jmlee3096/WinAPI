#pragma once

#include "stdafx.h"
#include "resource.h"
#include <winuser.h>
#include <objidl.h>
#include <gdiplus.h>
#include <minwinbase.h>
#include <math.h>
#include <minwinbase.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define PI							3.1416
#define DEGREE180					180
#define CLUSTER_SIZE				359
#define RADIUS						100			
#define CENTER_X					150			
#define CENTER_Y					80

#define MAX_LINE_CLUSTER_COUNT		4	

const int DRONE_MOVE_DISTANCE = 5;

typedef struct _DRONEPOS {

	int width;
	int height;
	int x;
	int y;

	int dx;
	int dy;
	int currentPos;

}DRONEPOS;

Image* LoadPNG(HMODULE hModule, LPCWSTR ResourceName)
{
	Image* image = NULL;
	IStream* pStream = NULL;

	HRSRC hResource = FindResource(hModule, ResourceName, L"png");

	if (S_OK == CreateStreamOnHGlobal(NULL, TRUE, &pStream)) {

		PVOID pResourceData = LockResource(LoadResource(hModule, hResource));
		DWORD imageSize = SizeofResource(hModule, hResource);
		DWORD dwReadWrite = 0;

		pStream->Write(pResourceData, imageSize, &dwReadWrite);
		image = Image::FromStream(pStream);
		pStream->Release();

	}

	return image;
}

void ShowPngImage(Image* g_image, HMODULE hModule, LPCWSTR ResourceName, HDC hdc)
{
	g_image = LoadPNG(hModule, ResourceName);
	Graphics g(hdc);
	g.DrawImage(g_image, 100, 100);

	/*
	TCHAR temp[80];
	GetObject(image, sizeof(image), image);
	wsprintf(temp, TEXT("%d"), image->GetWidth());
	TextOut(hdc, 0, 0, temp, lstrlen(temp));
	*/

	delete g_image;
}

void ShowBmpImage(HDC hdc, HINSTANCE g_hInstance, LPCWSTR ResourceName)
{
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP myBitmap, prevBitmap;
	myBitmap = LoadBitmap(g_hInstance, ResourceName);
	prevBitmap = (HBITMAP)SelectObject(memDC, myBitmap);
	BitBlt(hdc, 100, 100, 120, 120, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, prevBitmap);
	DeleteObject(myBitmap);
	DeleteDC(memDC);
}

void InitImage(HINSTANCE g_hInstance, LPCWSTR ResourceName, HBITMAP* g_droneBitmap, DRONEPOS** g_dronePos, 
	int inputDroneCount, int inputClusterCount)
{
	int idx = 0, idy = 0;
	*g_droneBitmap  = LoadBitmap(g_hInstance, ResourceName);
	BITMAP bm;
	GetObject(*g_droneBitmap, sizeof(BITMAP), &bm);

	while(idx < inputClusterCount)
	{
		while(idy < inputDroneCount)
		{
			ZeroMemory(&g_dronePos[idx][idy], sizeof(DRONEPOS));
			g_dronePos[idx][idy].width = bm.bmWidth;
			g_dronePos[idx][idy].height = bm.bmHeight;
			g_dronePos[idx][idy].x = 0;
			g_dronePos[idx][idy].y = 0;
			g_dronePos[idx][idy].currentPos = CLUSTER_SIZE - (idy * (CLUSTER_SIZE / inputDroneCount));
			g_dronePos[idx][idy].dx = DRONE_MOVE_DISTANCE;
			g_dronePos[idx][idy].dy = DRONE_MOVE_DISTANCE;

			idy++;
		}
		
		idx++;
		idy = 0;

	}

}

void DronePosUpdate(RECT* prc, DRONEPOS** g_dronePos, int inputDroneCount, int inputClusterCount)
{

	int droneIDX = 0, droneIDY = 0;
	double deg, radian;

	while(droneIDX < inputClusterCount)
	{
		while(droneIDY < inputDroneCount)
		{
			if (CLUSTER_SIZE <= g_dronePos[droneIDX][droneIDY].currentPos)
			{
				g_dronePos[droneIDX][droneIDY].currentPos = DRONE_MOVE_DISTANCE;
			}
			else
			{
				g_dronePos[droneIDX][droneIDY].currentPos += DRONE_MOVE_DISTANCE;
			}

			deg = 360 / (CLUSTER_SIZE + 1) * g_dronePos[droneIDX][droneIDY].currentPos;
			radian = deg * PI / DEGREE180;

			g_dronePos[droneIDX][droneIDY].x = CENTER_X * ((droneIDX % MAX_LINE_CLUSTER_COUNT) + 1) + 
				((droneIDX % MAX_LINE_CLUSTER_COUNT) * 200) + RADIUS * cos(radian);	// Elipse Center X pos + r * cos(radian)
			g_dronePos[droneIDX][droneIDY].y = CENTER_Y * ((droneIDX / MAX_LINE_CLUSTER_COUNT) + 1) + 
				((droneIDX / MAX_LINE_CLUSTER_COUNT) * 240) + RADIUS * sin(radian);	// Elipse Center Y pos + r * sin(radian)


			droneIDY++;
		}

		droneIDX++;
		droneIDY = 0;
	}

	//g_dronePos->x += g_dronePos->dx;
	//g_dronePos->y += g_dronePos->dy;

	/*
	// 윈도우 화면 벗어났을 때 예외처리
	
	if (g_dronePos->x < 0)
	{
		g_dronePos->x = 0;
		g_dronePos->dx = DRONE_MOVE_DISTANCE;
	}
	else if (g_dronePos->x + g_dronePos->width > prc->right)
	{
		g_dronePos->x = prc->right - g_dronePos->width;
		g_dronePos->dx = -DRONE_MOVE_DISTANCE;
	}

	if (g_dronePos->y < 0)
	{
		g_dronePos->y = 0;
		g_dronePos->dy = DRONE_MOVE_DISTANCE;
	}
	else if (g_dronePos->y + g_dronePos->height > prc->bottom)
	{
		g_dronePos->y = prc->bottom - g_dronePos->height;
		g_dronePos->dy = -DRONE_MOVE_DISTANCE;
	}
	*/
}

void DrawDrone(HDC hdc, RECT* prc, DRONEPOS** g_dronePos, HBITMAP g_droneBitmap, 
	HINSTANCE g_hInstance, LPCWSTR ResourceName, HWND hWnd, int inputDroneCount, int inputClusterCount)
{
	int droneIDX = 0, droneIDY = 0;

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBuffer);
	
	FillRect(hdcBuffer, prc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//PatBlt(hdcBuffer, 0, 0, prc->right, prc->bottom, WHITENESS);
	//BitBlt(hdcBuffer, g_ballInfo->x, g_ballInfo->y, g_ballInfo->width, g_ballInfo->height, hdcBuffer, 0, 0, SRCAND);
	
	SelectObject(hdcMem, g_droneBitmap);

	while(droneIDX < inputClusterCount)
	{
		while(droneIDY < inputDroneCount)
		{
			BitBlt(hdcBuffer, g_dronePos[droneIDX][droneIDY].x, g_dronePos[droneIDX][droneIDY].y,
				g_dronePos[droneIDX][droneIDY].width, g_dronePos[droneIDX][droneIDY].height, hdcMem, 0, 0, SRCAND);
			droneIDY++;
		}

		droneIDX++;
		droneIDY = 0;
	}

	/*
	TCHAR temp[80];
	wsprintf(temp, TEXT("pos1 : %d, pos2 : %d, pos3 : %d"), g_dronePos[0].currentPos, g_dronePos[1].currentPos, g_dronePos[2].currentPos);
	TextOut(hdcBuffer, 10, 10, temp, lstrlen(temp));

	
	TCHAR temp[80];
	wsprintf(temp, TEXT("x1 : %d, y1 : %d, x2 : %d, y2 : %d, x3 : %d, y3 : %d"), 
		g_dronePos[0].x, g_dronePos[0].y, g_dronePos[1].x, g_dronePos[1].y, g_dronePos[2].x, g_dronePos[2].y);
	TextOut(hdcBuffer, 10, 10, temp, lstrlen(temp));

	
	TCHAR temp[80];
	wsprintf(temp, TEXT("x : %d, y : %d, idx : %d"), g_dronePos->x, g_dronePos->y);
	TextOut(hdcBuffer, 10, 10, temp, lstrlen(temp));
	*/


	BitBlt(hdc, 0, 100, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

void InitControlBox(HWND hWnd, HWND* hDroneList, HWND* hClusterList, HINSTANCE g_hInstance, char droneCount[][15], int droneCountSize,
	char clusterCount[][15], int clusterCountSize)
{
	int droneCnt = 0;
	int clusterCnt = 0;

	CreateWindow(L"static", L"Count of Drone : ", WS_CHILD | WS_VISIBLE | SS_CENTER,
		10, 17, 110, 19, hWnd, (HMENU)IDC_DRONE_STATIC_TEXT, g_hInstance, NULL);

	*hDroneList = CreateWindow(L"combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		130, 15, 70, 170, hWnd, (HMENU)IDC_DRONE_CNT_COMBOBOX, g_hInstance, NULL);
	
	CreateWindow(L"static", L"Count of Cluster : ", WS_CHILD | WS_VISIBLE | SS_CENTER,
		250, 17, 120, 19, hWnd, (HMENU)IDC_CLUSTER_STATIC_TEXT, g_hInstance, NULL);

	*hClusterList = CreateWindow(L"combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		380, 15, 70, 150, hWnd, (HMENU)IDC_CLUSTER_CNT_COMBOBOX, g_hInstance, NULL);
	
	//CreateWindow(L"button", L"sendButton", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	//10, 10, 100, 30, hWnd, (HMENU)IDC_SEND, g_hInstance, NULL);

	while(droneCnt < droneCountSize)
	{
		SendMessage(*hDroneList, CB_ADDSTRING, 0, (LPARAM)droneCount[droneCnt]);
		droneCnt++;
	}

	while(clusterCnt < clusterCountSize)
	{
		SendMessage(*hClusterList, CB_ADDSTRING, 0, (LPARAM)clusterCount[clusterCnt]);
		clusterCnt++;
	}
}

DRONEPOS** InitializeDroneCount(int inputDroneCount, int inputClusterCount)
{
	int droneID = 0;
	DRONEPOS** g_dronePos = (DRONEPOS**)malloc(sizeof(DRONEPOS*) * inputClusterCount);
	
	while(droneID < inputClusterCount)
	{
		g_dronePos[droneID] = (DRONEPOS*)malloc(sizeof(DRONEPOS) * inputDroneCount);
		droneID++;
	}

	return g_dronePos;
}