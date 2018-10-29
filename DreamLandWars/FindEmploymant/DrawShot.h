//*****************************************************************************
//	
//		ドローショット
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _DRAWSHOT_H_
#define _DRAWSHOT_H_

#include "main.h"
#include "Bullet3D.h"
#include "collision.h"

class Player;
class DrawLine;
class Camera;
class CollisionCharacter;


class DrawShot : public Bullet3D
{
public:
	//----- コンストラクタ / デストラクタ -----
	DrawShot() {}
	DrawShot(Object::TYPE type);
	virtual ~DrawShot();

	//----- 基本的な関数 -----
	static DrawShot* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// DSの動的生成処理
	// pStartPoint：開始点のポインタ
	static void SetDrawShot(DrawLine* pStartPoint);

	//----- データを返す処理 -----


private:
	// DSの設定
	// pStartPoint：開始点のポインタ
	void SetDrawShot_private(DrawLine* pStartPoint);

	// 敵兵士との当たり判定
	void CollisionSoldier();

	// 塔との当たり判定
	// 返り値：	true  当たった
	//			false 当たってない
	bool Collision_Tower();

	// 当たった相手を初期化
	void ClearCollisionID();


	//----- データ -----
	DrawLine*   m_pMoveWay;			// 移動経路
	D3DXVECTOR3 m_vecPrevToNext;	// 経路毎の単位ベクトル
	Camera*     m_pCamera;			// カメラ
	Player*     m_pPlayer;			// 親プレイヤー

	//CollisionCharacter *m_pCollisionCharacter;	// 当たったキャラクタのポインタ

	static const int NUM_COLLISION_ID_SOLDIER = 64;			// 最大の兵士当たり判定数
	static const int CLEAR_ID = -1;							// CLEAR値
	int m_CollisionID_Soldier[NUM_COLLISION_ID_SOLDIER];	// 当たった兵士のバッファ
	int m_cntCollisionSoldier;								// 当たった兵士数

};


#endif

