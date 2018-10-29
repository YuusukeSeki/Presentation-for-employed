//*****************************************************************************
//	メイン関数[main.h]
//	Author	:	Yusuke Seki
//*****************************************************************************
#ifndef _MAIN_H_
#define _MAIN_H_

//----------- インクルードファイル
#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION	(0x0800)
#include <dinput.h>
#include <XAudio2.h>
#include "oxdebugfont.h"

//---------- プラグマコメント
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")	// システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")

//---------- 定数定義
const int SCREEN_WIDTH	= 1280;	// 画面幅
const int SCREEN_HEIGHT	= 720;	// 画面高さ
#define FVF_VERTEX_2D	( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )				// 頂点フォーマット(2Dポリゴン)
#define FVF_VERTEX_3D	( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL )	// 頂点フォーマット(3Dポリゴン)
#define _MSG(msg)	    (MessageBox(NULL, msg, NULL, MB_OK               ))
#define _MSGERROR(msg, title)	(MessageBox(NULL, msg, title, MB_OK | MB_ICONERROR))

//---------- 構造体の定義
typedef struct
{	// 頂点座標（2Dポリゴン）
	D3DXVECTOR3 pos;	// ３頂点の座標
	float rhw;			// 座標変換済み頂点フラグ
	D3DCOLOR color;		// 頂点毎の色
	D3DXVECTOR2 tex;	// テクスチャー座標
}VERTEX_2D;

typedef struct
{	// 頂点座標（3Dポリゴン）
	D3DXVECTOR3 pos;	// ３頂点の座標
	D3DXVECTOR3 normal;	// 法線
	D3DCOLOR color;		// 頂点毎の色
	D3DXVECTOR2 tex;	// テクスチャー座標
}VERTEX_3D;

//---------- プロトタイプ宣言
#ifdef _DEBUG
int GetFPS(void);	// FPS取得
#endif

HWND GetHWnd();

#endif