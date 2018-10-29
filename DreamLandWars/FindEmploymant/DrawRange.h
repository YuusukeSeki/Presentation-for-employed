//*****************************************************************************
//
//		ドローショット射程範囲
//												Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _DRAWRANGE_H_
#define _DRAWRANGE_H_

#include "main.h"
#include "Object3D.h"

class Player;


class DrawRange : public Object3D
{
public:
	//----- コンストラクタ / デストラクタ -----
	DrawRange();
	DrawRange(Object::TYPE type);
	virtual ~DrawRange();


	//----- 基本的な関数 -----
	static DrawRange* Create(Player* pPlayer);
	virtual void Init(Player* pPlayer);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);


	//----- 増減処理 -----

	//----- 設定処理 -----
	void SetPlayer_DrawRange(Player* pPlayer) { m_pPlayer = pPlayer; }	// 親プレイヤーの設定

	//----- データを返す処理 -----


private:
	//----- データ -----
	Player* m_pPlayer;	// 親プレイヤー

};

#endif

