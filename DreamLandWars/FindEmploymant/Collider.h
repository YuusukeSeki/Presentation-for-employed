// author : yusuke seki
// data   : 20181115
#ifndef COLLIDER_H_
#define COLLIDER_H_

#include <d3dx9.h>
#include "Object.h"
class Unit;

class Collider : public Object
{
public:
	Collider(Unit* _parentUnit);
	~Collider();

	static Collider* Create(Unit* _parentUnit);
	void Release();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetRadius(const float& _radius);
	float GetRadius();

	void SetParentUnit(Unit* _parentUnit);

	bool Collision(Collider* _targetCollider);
	bool Collision(const D3DXVECTOR3& _point);


#ifdef _DEBUG
	void SetColor(const float& _r, const float& _g, const float& _b, const float& _a);
	void SetDraw(bool _isDraw);
#endif

private:
	Collider() {}

	bool IsMovePositionParentUnit();
	void ChaseParentUnit();

#ifdef _DEBUG
	void UpdateWorldMatrix();
	void CreateSphere();
#endif

	float radius_;

	Unit* parentUnit_;
	D3DXVECTOR3 previewParentUnitPosition_;

#ifdef _DEBUG
	LPD3DXMESH mesh_;
	D3DMATERIAL9 material_;
	D3DXMATRIX worldMatrix_;
	bool isDraw_;
#endif

};

#endif
