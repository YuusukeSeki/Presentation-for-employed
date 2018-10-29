//*****************************************************************************
//
//		チームレベル
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _TEAMLEVEL_H_
#define _TEAMLEVEL_H_

#include "main.h"
#include "Object2D.h"


class TeamLevel : public Object2D
{
public:
	//----- コンストラクタ / デストラクタ -----
	TeamLevel() {}
	TeamLevel(Object::TYPE type);
	~TeamLevel();

	//----- 基本的な関数 -----
	static TeamLevel* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Release(void) { delete this; }

	//----- 増減処理 -----

	//----- 設定処理 -----

	//----- データを返す処理 -----


private:
	//----- データ -----
	Object2D* m_pLabel_YOURTEAM;		// 【ラベル】"Lv"
	Object2D* m_pLabel_ENEMY;			// 【ラベル】"Lv"
	Object2D* m_pTeamLevel_YOURTEAM;	// 【チームレベル】数字
	Object2D* m_pTeamLevel_ENEMY;		// 【チームレベル】数字

};

#endif
#pragma once
