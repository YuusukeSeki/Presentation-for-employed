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



	// ���_�Ƀ_���[�W��^����
	// breakPower : �^����_���[�W
	// �y�Ԃ�l�z	true  : ���_����ꂽ
	//				false : ���_�����ĂȂ�
	bool BrowTower(float breakPower);

	// ���������͈͂Ƃ̓����蔻��
	// position : �ΏۃL���X�g�̈ʒu
	// �y�Ԃ�l�z	true  : �����
	//				false : ����Ȃ�
	bool CollisionBrowRange(D3DXVECTOR3& position);

	// �̗͂�Ԃ�
	// �y�Ԃ�l�z���݂̗̑�
	float GetLife() { return m_life; }

	// "Hold"�̎��̂�Ԃ�
	// �y�Ԃ�l�z"Hold"�̎���
	Hold* GetHold() { return m_pHold; }

	// �ړ�����
	void MovePosition(D3DXVECTOR3& movePosition);


private:
	SoldierGenerator* soldierGenerator_;
	Hold* hold_;
	LifeGauge* lifeGauge_;
	Icon* icon_;

};

#endif
