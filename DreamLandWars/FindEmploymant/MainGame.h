//*****************************************************************************
//	
//		メインゲーム
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "main.h"
#include "GameScene.h"
#include "list_LoadTexture_MainGame.h"

// FW
class Camera;
class Light;
class Texture;

// Object
class Player;
class Field;
class SkyBox;
class DrawRange;
class Tower;
class Castle;
class Wall;

// UI
class Timer;
class Score;
class FrameBorder;

// etc...
class Object2D;


class MainGame : public GameScene
{
public:
	//----- 基本的な関数 -----
	void Init();
	void Uninit();
	GameScene* Update();
	void Draw();
	void Release();

	//----- データを返す処理 -----
	static Field* GetField() { return m_pField; }																// フィールドの取得
	static SkyBox* GetSkyBox() { return m_pSkyBox; }															// スカイボックスの取得
	static Camera* GetCamera(int index) { return m_pCamera[index]; }											// カメラの取得
	static Texture* GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME texName) { return m_pTexture[texName]; }	// テクスチャーの取得
	static Wall* GetWall(int index);																			// 壁情報の取得

private:
	//----- 関数 -----
	bool Start_NextScene();	// true で次のシーンへ
	void InputManage();		// 入力制御

	//----- データ -----
	static const int NUM_WALL = 4;	// 壁の数
	static const int NUM_TOWER = 4;	// 塔の数

	static Field* m_pField;			// フィールド
	static Wall* m_pWall[NUM_WALL];	// 壁
	static SkyBox* m_pSkyBox;		// スカイボックス
	static Camera* m_pCamera[2];	// カメラ
	Light* m_pLight;				// ライト
	Player* m_pPlayer;				// プレイヤー
	DrawRange* m_pDrawRange;		// 射程範囲
	Score* m_pScore;				// スコア
	Tower* m_pTower;				// 塔
	Castle* m_pCastle;				// 城
	FrameBorder* m_pFrameBorder;	// 枠線
	Object2D* _tutorial;

	static Texture* m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::__LOADTEXTURE_MAX];	// テクスチャ

	// デバッグ用切り替えスイッチ（カメラ ⇔ プレイヤー）
	int _switchControl;

};

#endif
