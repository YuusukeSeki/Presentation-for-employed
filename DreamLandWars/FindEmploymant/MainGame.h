// author : yusuke seki
// data   : 20181102
#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "main.h"
#include "GameScene.h"
#include "list_LoadTexture_MainGame.h"
#include "texture.h"
#include "player.h"
#include "DrawRange.h"
#include "timer.h"
#include "score.h"
#include "FrameBorder.h"
#include "Object2D.h"

#include <vector>
#include <map>
#include <string>

class Field;
class RelayPoint;
class Wall;
class Tower;
class Castle;
class Camera;
class Light;
class SkyBox;

class MainGame : public GameScene
{
public:
	static std::vector<Player*> GetPlayers() { return players; }
	static Player* GetPlayer(unsigned int index);
	static Field* GetField() { return m_pField; }
	static SkyBox* GetSkyBox() { return m_pSkyBox; }
	static Camera* GetCamera() { return camera_; }
	static Texture* GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME texName) { return m_pTexture[texName]; }
	static Wall* GetWall(int index);

	void Init();
	void Uninit();
	GameScene* Update();
	void Draw();
	void Release();

private:
	void CreateField();
	void CreateWall();
	void CreateRelayPoint();
	void CreateUnit();
	void CreateBasePoint();
	void CreateUI();
	void CreateCamera();
	void CreateLight();
	void CreateBackGround();

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



	bool Start_NextScene();
	void InputManage();

	Field* field_;
	std::map<std::string, ObjectModel*> wallList_;
	std::map<std::string, RelayPoint*> relayPoints_;

	static Camera* camera_;
	std::map<std::string, Light*> lights_;

	std::map<std::string, Tower*> towers_;
	std::map<std::string, Castle*> castles_;

	SkyBox* skyBox_;

};

#endif
