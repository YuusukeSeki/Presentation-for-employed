// author : yusuke seki
// data   : 20181111
#include "BulletBillboard.h"

BulletBillboard::BulletBillboard() : Bullet3D(Object::TYPE::TYPE_3D_BILLBOARD_BULLET)
{

}

BulletBillboard::BulletBillboard(const Object::TYPE& type) : Bullet3D(type)
{

}

BulletBillboard::~BulletBillboard()
{
	Uninit();
}

BulletBillboard* BulletBillboard::Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera)
{
	BulletBillboard* bullet = new BulletBillboard(Object::TYPE::TYPE_3D_BILLBOARD_BULLET);
	bullet->Init(_position, _size, _camera);

	return bullet;
}

void BulletBillboard::Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera)
{
	camera_ = _camera;
	Bullet3D::Init(_position, _size);
}

void BulletBillboard::Uninit(void)
{
	Bullet3D::Uninit();
}

void BulletBillboard::Draw(void)
{
	Bullet3D::Draw();
}

void BulletBillboard::SetCamera(Camera* _camera)
{
	camera_ = _camera;
}

Camera* BulletBillboard::GetCamera()
{
	return camera_;
}

void BulletBillboard::UpdateWorldMatrix()
{
	if (camera_ == nullptr)
	{
		_MSGERROR("ƒJƒƒ‰‚ªÝ’è‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ", "BulletBillboard::UpdateWorldMatrix");
		Bullet3D::UpdateWorldMatrix();
		return;
	}
	
	D3DXMATRIX worldMatrix, translateMatrix, rotateMatrix, scaleMatrix, viewMatrixInverse;
	D3DXMATRIX viewMatrix = camera_->GetMtxView();
	D3DXVECTOR3 position = GetPosition();
	D3DXQUATERNION quaternion = GetQuaternion();
	D3DXVECTOR3 scale = GetScale();

	D3DXMatrixIdentity(&worldMatrix);

	D3DXMatrixTranslation(&translateMatrix, position.x, position.y, position.z);
	D3DXMatrixRotationQuaternion(&rotateMatrix, &quaternion);
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
	D3DXMatrixTranspose(&viewMatrixInverse, &viewMatrix);
	viewMatrixInverse._14 = viewMatrixInverse._24 = viewMatrixInverse._34 = 0.0f;

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleMatrix);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotateMatrix);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &viewMatrixInverse);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	SetWorldMatrix(worldMatrix);

}
