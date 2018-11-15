// author : yusuke seki
// data   : 20181111
#include "ObjectBillboard.h"

ObjectBillboard::ObjectBillboard() : Object3D(Object::TYPE::TYPE_3D_BILLBOARD)
{
	camera_ = nullptr;
}

ObjectBillboard::ObjectBillboard(const Object::TYPE& type) : Object3D(type)
{
	camera_ = nullptr;
}

ObjectBillboard::~ObjectBillboard()
{
	Uninit();
}

ObjectBillboard* ObjectBillboard::Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera)
{
	ObjectBillboard* bullet = new ObjectBillboard(Object::TYPE::TYPE_3D_BILLBOARD);
	bullet->Init(_position, _size, _camera);

	return bullet;
}

void ObjectBillboard::Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera)
{
	camera_ = _camera;

	Object3D::Init(_position, _size);
}

void ObjectBillboard::Uninit(void)
{
	Object3D::Uninit();
}

void ObjectBillboard::Draw(void)
{
	Object3D::Draw();
}

void ObjectBillboard::SetCamera(Camera* _camera)
{
	camera_ = _camera;
}

Camera* ObjectBillboard::GetCamera()
{
	return camera_;
}

void ObjectBillboard::UpdateWorldMatrix()
{
	if (camera_ == nullptr)
	{
		_MSGERROR("ƒJƒƒ‰‚ªÝ’è‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ", "ObjectBillboard::UpdateWorldMatrix");
		Object3D::UpdateWorldMatrix();
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
