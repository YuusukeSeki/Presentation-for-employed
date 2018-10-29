//*****************************************************************************
//
//		チームゲージマネージャー
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _TEAMGAUGE_MANAGER_H_
#define _TEAMGAUGE_MANAGER_H_

#include "main.h"
#include "Object2D.h"

class TeamGauge;


class TeamGaugeManager : public Object2D
{
public:
	//----- コンストラクタ / デストラクタ -----
	TeamGaugeManager() {}
	TeamGaugeManager(Object::TYPE type);
	~TeamGaugeManager();

	//----- 基本的な関数 -----
	static TeamGaugeManager* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Release(void) { delete this; }

	//----- 増減処理 -----

	//----- 設定処理 -----

	//----- データを返す処理 -----
	static TeamGauge* GetTeamGauge_YOURTEAM() { return m_pTeamGauge_YOURTEAM; }
	static TeamGauge* GetTeamGauge_ENEMY()    { return m_pTeamGauge_ENEMY; }


private:
	//----- データ -----
	Object2D*  m_pLabel_YOURTEAM;		// 【ラベル】"YOUR TEAM"
	Object2D*  m_pLabel_ENEMY;			// 【ラベル】"ENEMY"
	static TeamGauge* m_pTeamGauge_YOURTEAM;	// 【ゲージ】"YOUR TEAM"
	static TeamGauge* m_pTeamGauge_ENEMY;		// 【ゲージ】"ENEMY"

};

#endif
