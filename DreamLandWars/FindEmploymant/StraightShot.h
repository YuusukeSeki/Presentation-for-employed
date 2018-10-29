#pragma once
//*****************************************************************************
//	
//		ストレートショット
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _STRAIGHTSHOT_H_
#define _STRAIGHTSHOT_H_

#include "main.h"
#include "Bullet3D.h"

class Player;

class StraightShot : public Bullet3D
{
public:
	//----- コンストラクタ / デストラクタ -----
	StraightShot() {}
	StraightShot(Object::TYPE type);
	virtual ~StraightShot();


	//----- 基本的な関数 -----
	static StraightShot* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Uninit(void);
	void Update(void);
	void Draw(void);


	//----- 設定処理 -----
	static void SetStraightShot(Player& player);	// SSの発生


	//----- データを返す処理 -----


protected:


private:
	//----- メソッド -----
	void SetStraightShot_Private(Player& player);	// SSの設定
	bool CollisionSoldier();	// true で消失
	bool CollisionPlayer();		// true で消失
	bool CollisionWall();		// true で消失

	//----- データ -----
	D3DXVECTOR3 m_uninitPos;		// 消失地点
	D3DXVECTOR3 m_vecInitToUninit;	// 消失用の単位ベクトル
	Player* m_pParent;				// 親

};


#endif

