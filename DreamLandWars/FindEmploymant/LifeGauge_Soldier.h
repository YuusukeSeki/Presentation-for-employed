//*****************************************************************************
//
//		体力ゲージ（兵士）
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _LIFEGAUGE_SOLDIER_H_
#define _LIFEGAUGE_SOLDIER_H_

#include "main.h"
#include "Gauge.h"

class Soldier;
class Camera;


class LifeGauge_Soldier : public Gauge
{
public:
	LifeGauge_Soldier();
	~LifeGauge_Soldier();

	static LifeGauge_Soldier* Create(Soldier* pSoldier, Camera* pCamera);
	void Init(Soldier* pSoldier, Camera* pCamera);
	void Uninit(void);
	void Update(void);

	// ゲージ量の増減処理
	void MoveLife(float moveLife);


private:
	Soldier* _pParent;	// 親

};

#endif
