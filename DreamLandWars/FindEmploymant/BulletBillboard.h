// author : yusuke seki
// data   : 20181111
#ifndef BULLETBILLBOARD_H_
#define BULLETBILLBOARD_H_

#include "Bullet3D.h"
#include "camera.h"

class BulletBillboard : public Bullet3D
{
public:
	BulletBillboard();
	BulletBillboard(const Object::TYPE& type);
	virtual ~BulletBillboard();

	static BulletBillboard* Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera);
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
