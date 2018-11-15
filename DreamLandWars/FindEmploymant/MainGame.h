// author : yusuke seki
// data   : 20181102
#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "main.h"
#include "GameScene.h"
#include "list_LoadTexture_MainGame.h"
#include "camera.h"
#include "light.h"
#include "field.h"
#include "skybox.h"
#include "texture.h"
#include "player.h"
#include "DrawRange.h"
#include "tower.h"
#include "castle.h"
#include "Wall.h"
#include "timer.h"
#include "score.h"
#include "FrameBorder.h"
#include "Object2D.h"
#include <vector>

class MainGame : public GameScene
{
private:
	static const int NUM_WALL = 4;
	static const int NUM_TOWER = 4;
	static std::vector<Player*> players;
	static Field* m_pField;
	static Wall* m_pWall[NUM_WALL];
	static SkyBox* m_pSkyBox;
	//static Camera* m_pCamera[2];
	static Texture* m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::__LOADTEXTURE_MAX];

	Light* m_pLight;
	DrawRange* m_pDrawRange;
	Score* m_pScore;
	Tower* m_pTower;
	Castle* m_pCastle;
	FrameBorder* m_pFrameBorder;
	Object2D* _tutorial;
	int _switchControl;

public:
	static std::vector<Player*> GetPlayers() { return players; }
	static Player* GetPlayer(unsigned int index);
	static Field* GetField() { return m_pField; }
	static SkyBox* GetSkyBox() { return m_pSkyBox; }
	//static Camera* GetCamera(int index) { return m_pCamera[index]; }
	static Texture* GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME texName) { return m_pTexture[texName]; }
	static Wall* GetWall(int index);

	void Init();
	void Uninit();
	GameScene* Update();
	void Draw();
	void Release();

private:
	bool Start_NextScene();
	void InputManage();

};

#endif
