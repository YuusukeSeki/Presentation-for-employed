// author : yusuke seki
// data   : 20181111
#ifndef ObjectBILLBOARD_H_
#define ObjectBILLBOARD_H_

#include "Object3D.h"
#include "camera.h"
class Unit;

class ObjectBillboard : public Object3D
{
public:
	ObjectBillboard();
	ObjectBillboard(const Object::TYPE& type);
	virtual ~ObjectBillboard();

	static void CreateBuffer(const unsigned int& _numCreate);
	static ObjectBillboard* Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, bool _positionIsLeftTop = false);
	static ObjectBillboard* DynamicCreate(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, bool _positionIsLeftTop = false);	// ìÆìIê∂ê¨
	virtual void Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, bool _positionIsLeftTop = false);
	virtual void Uninit(void);
	virtual void Update(void) {}
	virtual void Draw(void);

	void SetCamera(Camera* _camera);
	Camera* GetCamera();

	void SetParent(Unit* _parentUnit);
	void SetParent(D3DXMATRIX* _parentMatrix);

private:
	static ObjectBillboard* FindNonActiveObjectBillboard();
	void UpdateWorldMatrix();

	Camera* camera_;
	D3DXMATRIX* parentMatrix_;

};

#endif
