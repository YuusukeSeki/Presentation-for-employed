//*****************************************************************************
//
//		タイトルシーン
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "GameScene.h"
#include "list_LoadTexture_Title.h"

// Frame Work
class Camera;
class Light;

// Object
class SkyDome;
class GameTitle;
class TouchScreen;
class Object2D;

// Resource
class Texture;


class Title : public GameScene
{
public:
	void Init();
	void Uninit();
	GameScene* Update();
	void Draw();
	void Release();

private:
	Texture* _texture[List_LoadTexture_Title::__LOADTEXTURE_MAX];

	Camera* _camera;
	Light* _light;

	static const int NUM_CHARACTER_TITLENAME = 13;
	static const int NUM_CHARACTER_TOUCHSCREEN = 11;
	SkyDome* _skydome;
	GameTitle* _gameTitle[NUM_CHARACTER_TITLENAME];
	GameTitle* _gameTitle_Border[NUM_CHARACTER_TITLENAME];
	TouchScreen* _touchScreen[NUM_CHARACTER_TOUCHSCREEN];

	int tutorialNumber;
	
	Object2D* _tutorial;

};

#endif
