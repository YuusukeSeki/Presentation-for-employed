//*****************************************************************************
//
//		Life Gauge of Soldier
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _SOLDIER_LIFEGAUGE_H_
#define _SOLDIER_LIFEGAUGE_H_

#include "main.h"
#include "ObjectModel.h"

class Hold;
class LifeGauge;
class Icon;


class Castle : public ObjectModel
{
public:
	//----- コンストラクタ / デストラクタ -----
	Castle();
	Castle(Object::TYPE type);
	virtual ~Castle();

	//----- 基本的な関数 -----
	static Castle* Create(const char* FileName, Object::GROUP group);
	virtual void Init(const char* FileName, Object::GROUP group);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// ダメージを与える
	// breakPower : 与えるダメージ
	void BrowCastle(float breakPower);

	// 殴れる範囲との当たり判定
	// position : 対象キャストの位置
	// 【返り値】	true  : 殴れる
	//				false : 殴れない
	bool CollisionBrowRange(D3DXVECTOR3& position);


private:
	float m_browRange;	// 殴れる範囲
	Hold* m_pHold;	// 殴れる範囲のGUI

	int m_frameCounter;	// 兵士生成用フレームカウンター

	D3DXVECTOR3 vector_LEFT, vector_RIGHT;	// 兵士生成時の指揮官の初期前方向

};

#endif

