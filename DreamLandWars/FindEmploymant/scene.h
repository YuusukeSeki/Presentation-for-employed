//*****************************************************************************
//	
//		ポリゴンマネージャー
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//----- インクルードファイル
#include "main.h"


//----- クラスの定義
class Scene
{
public:
	// 定数
	static const int NUM_TYPE  = 3; // Model, 3D, 2D
	//static const int MAX_SCENE = 0x00000ffff; // 各Sceneの最大生成数
	static const int MAX_SCENE = 1; // 各Sceneの最大生成数

	// 色
	union Color {
		unsigned int color;
		unsigned char rgba[4];
	};

	// タイプ
	enum TYPE { // 描画順序兼用
		TYPE_MODEL,	       // ★モデル
		TYPE_MODEL_PLAYER, // プレイヤー
		TYPE_MODEL_ENEMY,  // エネミー
		TYPE_3D,           // ★3Dポリゴン
		TYPE_3D_FIELD,	   // フィールド
		TYPE_3D_BILLBOARD, // ビルボード
		TYPE_3D_SKYBOX,    // スカイボックス
		TYPE_3D_SPHERE,    // 球体
		TYPE_BB_BULLET,    // バレット
		TYPE_2D,           // ★2Dポリゴン
		TYPE_MAX,
	};

	// グループ
	enum GROUP {
		GROUP_NONE,
		GROUP_A,
		GROUP_B,
	};

	// コンストラクタ / デストラクタ
	Scene(){}
	Scene(int priority, Scene::TYPE type);
	virtual ~Scene(){}

	// 基本的な関数
	virtual void Init(void){}
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	void Release(void);

	// ALL
	static void UninitAll();
	static void UpdateAll();
	static void DrawAll();
	static void ReleaseAll();

	// 増減
	virtual void MovePos(D3DXVECTOR3 move) { m_pos += move; }

	// 設定
	virtual void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetGroup(GROUP group) { m_group = group; }

	// 取得
	static Scene* GetScene(Scene::TYPE type, int arrayIdx1, int arrayIdx2);
	D3DXVECTOR3 GetPos() { return m_pos; }
	Scene::TYPE GetType() { return m_sceneType; }
	GROUP GetGroup() { return m_group; }


protected:


private:
	// 変数
	static Scene* m_pScene[NUM_TYPE][MAX_SCENE];
	D3DXVECTOR3 m_pos;
	TYPE m_sceneType;

	GROUP m_group;

};

#endif

