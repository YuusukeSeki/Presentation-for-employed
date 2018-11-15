// author : yusuke seki
// data   : 20181115
#ifndef COLLIDER_H_
#define COLLIDER_H_

#include <d3dx9.h>
class Unit;

class Collider
{
public:
	Collider(Unit* _parentUnit);
	~Collider();

	static Collider* Create(Unit* _parentUnit);
	void Release();

	void Init();
	void Uninit();
	void Update();

	void SetPosition(const D3DXVECTOR3& _position);
	D3DXVECTOR3 GetPosition();

	void SetRadius(const float& _radius);
	float GetRadius();

	void SetParentUnit(Unit* _parentUnit);
	void SetActive(bool _isActive);

	bool Collision(Collider* _targetCollider);

#ifdef _DEBUG
	void Draw();
	void SetDraw(bool _isDraw);
#endif

private:
	Collider() {}

	bool IsMovePositionParentUnit();
	void ChaseParentUnit();
	void MovePosition(const D3DXVECTOR3& _moveVector, const float& _distance);

#ifdef _DEBUG
	void UpdateWorldMatrix();
	void UpdateVertexBuffer();
	void CreateSphere();
#endif

	D3DXVECTOR3 position_;
	float radius_;

	Unit* parentUnit_;
	D3DXVECTOR3 previewParentUnitPosition_;

	bool isActive_;

#ifdef _DEBUG
	LPD3DXMESH mesh_;
	D3DXMATRIX worldMatrix_;
	bool isDraw_;
#endif

};

#endif
