//*****************************************************************************
//
//		体力ゲージ（拠点）
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _BASEPOINT_LIFEGAUGE_H_
#define _BASEPOINT_LIFEGAUGE_H_

#include "main.h"
#include "ObjectBillboard.h"
#include "BasePoint.h"

class Camera;


class LifeGauge : public ObjectBillboard
{
public:
	//----- コンストラクタ / デストラクタ -----
	LifeGauge();
	~LifeGauge();

	//----- 基本的な関数 -----
	static LifeGauge* Create(BasePoint* pObjModel, Camera* pCamera);
	void Init(BasePoint* pObjModel, Camera* pCamera);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//----- 増減処理 -----
	void MoveLife(float moveLife);	// ゲージ量の増減処理

	//----- 設定処理 -----

	//----- データを返す処理 -----


private:
	//----- データ -----
	BasePoint* m_pParent;	// 親モデル

	float m_initLife;		// 初期体力
	float m_initWidth;		// ポリゴンの初期幅

	ObjectBillboard* m_pBG;	// 背景

};

#endif
