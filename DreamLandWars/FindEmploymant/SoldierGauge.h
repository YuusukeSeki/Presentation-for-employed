// author : yusuke seki
// data   : 20181121
#ifndef SOLDIERGAUGE_H_
#define SOLDIERGAUGE_H_

#include "ObjectBillboard.h"
class Soldier;

class SoldierGauge : public Object
{
public:
	SoldierGauge();
	virtual ~SoldierGauge();

	static void CreateBuffer(const unsigned int& _numCreate);
	static SoldierGauge* Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, Soldier* _parentSoldier);
	static SoldierGauge* DynamicCreate(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, Soldier* _parentSoldier);

	virtual void Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, Soldier* _parentSoldier);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void Release();
	void ReleaseGauge();

	void SetPosition(const D3DXVECTOR3& _position);
	void MovePosition(const D3DXVECTOR3& _move);

	void SetMaxHpPointer(float* _maxHp);
	void SetCurrentHpPointer(float* _currentHp);

	void SetActive(const bool& _isActive);

	void SetIsUpdateWorldMatrix(bool _isUpdate)
	{
		isUpdateWorldMatrix_ = _isUpdate;
	}

private:
	static SoldierGauge* FindNonActive();
	void CheckCurrentHp();
	void UpdateWorldMatrix();
	bool IsMovePositionParentUnit();
	void ChaseParentSoldier();

	D3DXMATRIX worldMatrix_;

	float* maxHp_;
	float* currentHp_;
	D3DXVECTOR3 halfSize_;
	float initWidth_;

	bool isUpdateWorldMatrix_;

	ObjectBillboard* currentGauge_;
	ObjectBillboard* backGround_;

	Soldier* parentSoldier_;
	D3DXVECTOR3 previewParentSoldierPosition_;

};

#endif
