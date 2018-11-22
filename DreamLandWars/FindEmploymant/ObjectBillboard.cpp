// author : yusuke seki
// data   : 20181111
#include "ObjectBillboard.h"
#include "Unit.h"

ObjectBillboard::ObjectBillboard() : Object3D(Object::TYPE::TYPE_3D_BILLBOARD)
{
	camera_ = nullptr;
	parentMatrix_ = nullptr;
}

ObjectBillboard::ObjectBillboard(const Object::TYPE& type) : Object3D(type)
{
	camera_ = nullptr;
	parentMatrix_ = nullptr;
}

ObjectBillboard::~ObjectBillboard()
{
	Uninit();
}

void ObjectBillboard::CreateBuffer(const unsigned int& _numCreate)
{
	for (unsigned int i = 0; i < _numCreate; ++i)
	{
		ObjectBillboard* buf = new ObjectBillboard();
		buf->SetActive(false);
	}
}

ObjectBillboard* ObjectBillboard::Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, bool _positionIsLeftTop)
{
	ObjectBillboard* billboard = new ObjectBillboard(Object::TYPE::TYPE_3D_BILLBOARD);
	billboard->Init(_position, _size, _camera, _positionIsLeftTop);

	return billboard;
}

ObjectBillboard* ObjectBillboard::DynamicCreate(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, bool _positionIsLeftTop)
{
	ObjectBillboard* billboard = FindNonActiveObjectBillboard();

	if (billboard == nullptr)
	{
		billboard = Create(_position, _size, _camera, _positionIsLeftTop);
	}
	else
	{
		billboard->Init(_position, _size, _camera, _positionIsLeftTop);
	}

	return billboard;
}

void ObjectBillboard::Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, bool _positionIsLeftTop)
{
	camera_ = _camera;
	parentMatrix_ = nullptr;

	Object3D::Init(_position, _size, _positionIsLeftTop);

	SetActive(true);
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

void ObjectBillboard::SetParent(Unit* _parentUnit)
{
	parentMatrix_ = _parentUnit->GetWorldMatrixPointer();

	SetUpdateWorldMatrix(true);
}

void ObjectBillboard::SetParent(D3DXMATRIX* _parentMatrix)
{
	parentMatrix_ = _parentMatrix;

	SetUpdateWorldMatrix(true);
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

	if (parentMatrix_ != nullptr)
	{
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, parentMatrix_);
	}

	SetWorldMatrix(worldMatrix);
}

ObjectBillboard* ObjectBillboard::FindNonActiveObjectBillboard()
{
	ObjectBillboard* billboard = (ObjectBillboard*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_BILLBOARD);

	if (billboard != nullptr)
	{
		for (;;)
		{
			if (billboard->GetActive() == false)
			{
				break;
			}

			billboard = (ObjectBillboard*)billboard->GetNextPointer();

			if (billboard == nullptr)
			{
				break;
			}
		}
	}

	return billboard;
}
