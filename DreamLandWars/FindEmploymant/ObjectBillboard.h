// author : yusuke seki
// data   : 20181111
#ifndef ObjectBILLBOARD_H_
#define ObjectBILLBOARD_H_

#include "Object3D.h"
#include "camera.h"

class ObjectBillboard : public Object3D
{
public:
	ObjectBillboard();
	ObjectBillboard(const Object::TYPE& type);
	virtual ~ObjectBillboard();

	static ObjectBillboard* Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera);
	virtual void Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera);
	virtual void Uninit(void);
	virtual void Update(void) {}
	virtual void Draw(void);

	void SetCamera(Camera* _camera);
	Camera* GetCamera();

private:
	void UpdateWorldMatrix();

	Camera* camera_;

};

#endif
