//*****************************************************************************
//
//		ゲージ
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

class ObjectBillboard;
class Camera;


class Gauge
{
public:
	Gauge();
	~Gauge();

	// 生成処理
	// position : 座標
	// size : 大きさ
	static Gauge* Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera);

	// 初期化処理
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera);

	// 終了処理
	virtual void Uninit(void);

	// 更新処理
	virtual void Update(void);

	// 解放処理
	void Release(void);

	// ゲージ量の増減処理
	virtual void MoveValue(float moveLife);

	// 現在のゲージ量の取得
	// 返り値 ： 現在のゲージ量
	float GetValue(void);

	// 描画の有無スイッチ
	void SetDraw(bool bDraw);
	

private:
	bool _bDraw;					// 描画する？
	float _value;					// 現在のゲージ量
	float _initWidth;				// ゲージの初期幅

	ObjectBillboard* _pGauge;		// ゲージ
	ObjectBillboard* _pBackGround;	// ゲージの背景

};

#endif
