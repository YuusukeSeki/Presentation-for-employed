// author : yusuke seki
// data   : 20181115
#ifndef TOWER_H_
#define TOWER_H_

#include "Unit.h"
class Hold;
class LifeGauge;
class Icon;
class SoldierGenerator;

class Tower : public Unit
{
public:
	Tower();
	Tower(Object::TYPE type);
	~Tower();

	static Tower* Create(D3DXVECTOR3& position, const char* FileName, Object::GROUP group);
	void Init(D3DXVECTOR3& position, const char* FileName, Object::GROUP group);
	void Uninit(void);
	void Update(void);
	void Draw(void);



	// 拠点にダメージを与える
	// breakPower : 与えるダメージ
	// 【返り値】	true  : 拠点が壊れた
	//				false : 拠点が壊れてない
	bool BrowTower(float breakPower);

	// 塔を殴れる範囲との当たり判定
	// position : 対象キャストの位置
	// 【返り値】	true  : 殴れる
	//				false : 殴れない
	bool CollisionBrowRange(D3DXVECTOR3& position);

	// 体力を返す
	// 【返り値】現在の体力
	float GetLife() { return m_life; }

	// "Hold"の実体を返す
	// 【返り値】"Hold"の実体
	Hold* GetHold() { return m_pHold; }

	// 移動処理
	void MovePosition(D3DXVECTOR3& movePosition);


private:
	SoldierGenerator* soldierGenerator_;
	Hold* hold_;
	LifeGauge* lifeGauge_;
	Icon* icon_;

};

#endif
