//*****************************************************************************
//
//		リザルト
//												Author : Yusuke Seki
//*****************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "GameScene.h"
#include "list_LoadTexture_Result.h"

class Scene2D;
class Object2D;
class Texture;


class Result : public GameScene
{
public:
	// 基本的な関数
	void Init();
	void Uninit();
	GameScene* Update();
	void Draw();
	void Release();


private:
	Scene2D * m_pScene2D;
	Object2D* m_pObject2D;
	Texture* m_pTexture[List_LoadTexture_Result::__LOADTEXTURE_MAX];

};

#endif
#pragma once
