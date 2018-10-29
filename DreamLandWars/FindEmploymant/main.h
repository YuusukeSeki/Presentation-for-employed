//	Author : Yusuke Seki
//	Author : 20181029
#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION	(0x0800)
#include <dinput.h>
#include <XAudio2.h>
#include "oxdebugfont.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

const int SCREEN_WIDTH	= 1280;
const int SCREEN_HEIGHT	= 720;
#define FVF_VERTEX_2D	( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define FVF_VERTEX_3D	( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL )
#define _MSG(msg)	    (MessageBox(NULL, msg, NULL, MB_OK               ))
#define _MSGERROR(msg, title)	(MessageBox(NULL, msg, title, MB_OK | MB_ICONERROR))

typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR color;
	D3DXVECTOR2 tex;
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	D3DXVECTOR2 tex;
}VERTEX_3D;

#ifdef _DEBUG
int GetFPS(void);
#endif

HWND GetHWnd();

#endif