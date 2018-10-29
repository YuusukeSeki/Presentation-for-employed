//*****************************************************************************
//
//		チームゲージ
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _TEAMGAUGE_H_
#define _TEAMGAUGE_H_

#include "main.h"
#include "Object2D.h"



class TeamGauge : public Object2D
{
public:
	//----- コンストラクタ / デストラクタ -----
	TeamGauge() {}
	TeamGauge(Object::TYPE type);
	~TeamGauge();

	//----- 基本的な関数 -----
	static TeamGauge* Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Object::GROUP group);
	void Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Object::GROUP group);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//----- 増減処理 -----
	void MoveLife(float moveLife);	// ゲージ量の増減処理

	//----- 設定処理 -----

	//----- データを返す処理 -----
	float GetCurrentGauge() { return m_life; }	// 現在のゲージ量を返す
	bool Lose();


private:
	float m_life;		// 現在のゲージ量
	float m_initWidth;	// ポリゴンの初期幅

};

#endif
