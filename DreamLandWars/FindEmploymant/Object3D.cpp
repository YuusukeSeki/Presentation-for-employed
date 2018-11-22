// author : yusuke seki
// data   : 20181111
#include "Object3D.h"
#include "renderer.h"
#include "texture.h"

Object3D::Object3D() : Object(Object::TYPE::TYPE_3D)
{
	worldMatrix_ = {};
	halfSize_ = D3DXVECTOR3(0, 0, 0);
	quaternion_ = D3DXQUATERNION(0, 0, 0, 1);
	scale_ = D3DXVECTOR3(0, 0, 0);
	radius_ = 0;
	color_.color = 0x00000000;
	uvStartPoint_ = D3DXVECTOR2(0, 0);
	uvSize_ = D3DXVECTOR2(0, 0);
	vertexBuf_ = nullptr;
	texture_ = nullptr;
	isUpdateVertexBuf_ = false;
	isUpdateWorldMatrix_ = false;
	isDraw_ = false;

	positionIsLeftTop_ = false;
}

Object3D::Object3D(const Object::TYPE& type) : Object(type)
{
	worldMatrix_ = {};
	halfSize_ = D3DXVECTOR3(0, 0, 0);
	quaternion_ = D3DXQUATERNION(0, 0, 0, 1);
	scale_ = D3DXVECTOR3(0, 0, 0);
	radius_ = 0;
	color_.color = 0x00000000;
	uvStartPoint_ = D3DXVECTOR2(0, 0);
	uvSize_ = D3DXVECTOR2(0, 0);
	vertexBuf_ = nullptr;
	texture_ = nullptr;
	isUpdateVertexBuf_ = false;
	isUpdateWorldMatrix_ = false;
	isDraw_ = false;

	positionIsLeftTop_ = false;
}

Object3D::~Object3D()
{
	Uninit();
}

Object3D* Object3D::Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, bool _positionIsLeftTop)
{
	Object3D* object = new Object3D(Object::TYPE::TYPE_3D);
	object->Init(_position, _size, _positionIsLeftTop);

	return object;
}

void Object3D::Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, bool _positionIsLeftTop)
{
	SetPosition(_position);

	SetSize(_size);

	D3DXQuaternionIdentity(&quaternion_);

	SetScale(D3DXVECTOR3(1, 1, 1));

	SetColor(0xffffffff);

	SetUv_StartPoint(D3DXVECTOR2(0, 0));

	SetUv_Size(D3DXVECTOR2(1, 1));

	positionIsLeftTop_ = _positionIsLeftTop;

	MakeVertexBuf();

	UpdateWorldMatrix();

	texture_ = nullptr;

	isDraw_ = true;

	SetActive(true);
}

void Object3D::Uninit()
{
	if (vertexBuf_ != nullptr)
	{
		vertexBuf_->Release();
		vertexBuf_ = nullptr;
	}
}

void Object3D::Draw()
{
	if (isDraw_ == false)
	{
		return;
	}

	if (isUpdateVertexBuf_ == true)
	{
		UpdateVertexBuf();

		isUpdateVertexBuf_ = false;
	}

	if (isUpdateWorldMatrix_ == true)
	{
		UpdateWorldMatrix();

		isUpdateWorldMatrix_ = false;
	}

	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();

	device->SetTransform(D3DTS_WORLD, &worldMatrix_);

	device->SetStreamSource(0, vertexBuf_, 0, sizeof(VERTEX_3D));

	device->SetFVF(FVF_VERTEX_3D);

	device->SetTexture(0, texture_);

	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

D3DXMATRIX Object3D::GetWorldMatrix()
{
	return worldMatrix_;
}

void Object3D::SetPosition(const D3DXVECTOR3& _position)
{
	Object::SetPosition(_position);

	isUpdateWorldMatrix_ = true;
}

void Object3D::MovePosition(const D3DXVECTOR3& _move)
{
	Object::MovePosition(_move);

	isUpdateWorldMatrix_ = true;
}

void Object3D::SetSize(const D3DXVECTOR3& _size)
{
	halfSize_ = _size * 0.5f;
	radius_ = sqrtf(halfSize_.x * halfSize_.x + halfSize_.y * halfSize_.y);

	isUpdateVertexBuf_ = true;
}

void Object3D::MoveSize(const D3DXVECTOR3& _move)
{
	halfSize_ += _move * 0.5f;
	radius_ = sqrtf(halfSize_.x * halfSize_.x + halfSize_.y * halfSize_.y);

	isUpdateVertexBuf_ = true;
}

D3DXVECTOR3 Object3D::GetSize()
{
	return (halfSize_ * 2);
}

D3DXVECTOR3 Object3D::GetHalfSize()
{
	return halfSize_;
}

float Object3D::GetRadius()
{
	return radius_;
}

void Object3D::SetRotate(const D3DXVECTOR3& _vector, const float& _angle)
{
	D3DXQuaternionIdentity(&quaternion_);
	D3DXQuaternionRotationAxis(&quaternion_, &_vector, _angle);

	isUpdateWorldMatrix_ = true;
}

void Object3D::MoveRotate(const D3DXVECTOR3& _vector, const float& _moveAngle)
{
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &_vector, _moveAngle);
	D3DXQuaternionMultiply(&quaternion_, &quaternion_, &quaternion);

	isUpdateWorldMatrix_ = true;
}

D3DXQUATERNION& Object3D::GetQuaternion()
{
	return quaternion_;
}

void Object3D::SetScale(const D3DXVECTOR3& _scale)
{
	scale_.x = _scale.x < 0 ? 0 : _scale.x;
	scale_.y = _scale.y < 0 ? 0 : _scale.y;
	scale_.z = _scale.z < 0 ? 0 : _scale.z;

	isUpdateWorldMatrix_ = true;
}

void Object3D::MoveScale(const D3DXVECTOR3& _move)
{
	scale_.x = scale_.x + _move.x < 0 ? 0 : scale_.x + _move.x;
	scale_.y = scale_.y + _move.y < 0 ? 0 : scale_.y + _move.y;
	scale_.z = scale_.z + _move.z < 0 ? 0 : scale_.z + _move.z;

	isUpdateWorldMatrix_ = true;
}

D3DXVECTOR3 Object3D::GetScale()
{
	return scale_;
}

void Object3D::SetColor(const unsigned int& _rgba)
{
	color_.color = _rgba;

	isUpdateVertexBuf_ = true;
}

void Object3D::SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a)
{
	color_.rgba[3] = _r;
	color_.rgba[2] = _g;
	color_.rgba[1] = _b;
	color_.rgba[0] = _a;

	isUpdateVertexBuf_ = true;
}

void Object3D::MoveColor(const int& _r, const int& _g, const int& _b, const int& _a)
{
	color_.rgba[3] = color_.rgba[3] + _r >= 255 ? 255 : color_.rgba[3] + _r;
	color_.rgba[2] = color_.rgba[2] + _g >= 255 ? 255 : color_.rgba[2] + _g;
	color_.rgba[1] = color_.rgba[1] + _b >= 255 ? 255 : color_.rgba[1] + _b;
	color_.rgba[0] = color_.rgba[0] + _a >= 255 ? 255 : color_.rgba[0] + _a;
	color_.rgba[3] = color_.rgba[3] + _r <= 0 ? 0 : color_.rgba[3] + _r;
	color_.rgba[2] = color_.rgba[2] + _g <= 0 ? 0 : color_.rgba[2] + _g;
	color_.rgba[1] = color_.rgba[1] + _b <= 0 ? 0 : color_.rgba[1] + _b;
	color_.rgba[0] = color_.rgba[0] + _a <= 0 ? 0 : color_.rgba[0] + _a;

	isUpdateVertexBuf_ = true;
}

unsigned int Object3D::GetColor()
{
	return color_.color;
}

void Object3D::SetUv_StartPoint(const D3DXVECTOR2& _uvStartPoint)
{
	uvStartPoint_ = _uvStartPoint;

	isUpdateVertexBuf_ = true;
}

void Object3D::MoveUv_StartPoint(const D3DXVECTOR2& _move)
{
	uvStartPoint_ += _move;

	isUpdateVertexBuf_ = true;

}

D3DXVECTOR2 Object3D::GetUv_StartPoint()
{
	return uvStartPoint_;
}

void Object3D::SetUv_Size(const D3DXVECTOR2& _uvSize)
{
	uvSize_ = _uvSize;

	isUpdateVertexBuf_ = true;
}

void Object3D::MoveUv_Size(const D3DXVECTOR2& _move)
{
	uvSize_ += _move;

	isUpdateVertexBuf_ = true;
}

D3DXVECTOR2 Object3D::GetUv_Size()
{
	return uvSize_;
}

void Object3D::SetTexture(const LPDIRECT3DTEXTURE9& _texture)
{
	texture_ = _texture;
}

LPDIRECT3DTEXTURE9 Object3D::GetTexture()
{
	return texture_;
}

void Object3D::SetIsDraw(const bool& _isDraw)
{
	isDraw_ = _isDraw;
}

bool Object3D::GetIsDraw()
{
	return isDraw_;
}

void Object3D::SetWorldMatrix(const D3DXMATRIX& _worldMatrix)
{
	worldMatrix_ = _worldMatrix;
}

void Object3D::SetUpdateVertexBuf(const bool& _isUpdateVertexBuf)
{
	isUpdateVertexBuf_ = _isUpdateVertexBuf;
}

void Object3D::SetUpdateWorldMatrix(const bool& _isUpdateWorldMatrix)
{
	isUpdateWorldMatrix_ = _isUpdateWorldMatrix;
}

void Object3D::MakeVertexBuf()
{
	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();

	if (vertexBuf_ == nullptr)
	{
		HRESULT hr = device->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &vertexBuf_, nullptr);

		if (FAILED(hr))
		{
			_MSGERROR("頂点バッファの生成に失敗しました", "Object3D::MakeVertexBuf");

			Release();

			return;
		}
	}

	VERTEX_3D* vertexBuf;
	vertexBuf_->Lock(0, 0, (void**)&vertexBuf, 0);

	if (positionIsLeftTop_ == false)
	{
		vertexBuf[0].pos = D3DXVECTOR3(-halfSize_.x,  halfSize_.y,  halfSize_.z);
		vertexBuf[1].pos = D3DXVECTOR3( halfSize_.x,  halfSize_.y,  halfSize_.z);
		vertexBuf[2].pos = D3DXVECTOR3(-halfSize_.x, -halfSize_.y, -halfSize_.z);
		vertexBuf[3].pos = D3DXVECTOR3( halfSize_.x, -halfSize_.y, -halfSize_.z);
	}
	else
	{
		vertexBuf[0].pos = D3DXVECTOR3(0              , 0              , 0);
		vertexBuf[1].pos = D3DXVECTOR3(halfSize_.x * 2, 0              , 0);
		vertexBuf[2].pos = D3DXVECTOR3(0              , halfSize_.y * 2, 0);
		vertexBuf[3].pos = D3DXVECTOR3(halfSize_.x * 2, halfSize_.y * 2, 0);
	}

	vertexBuf[0].normal = D3DXVECTOR3(0, 0, -1);
	vertexBuf[1].normal = D3DXVECTOR3(0, 0, -1);
	vertexBuf[2].normal = D3DXVECTOR3(0, 0, -1);
	vertexBuf[3].normal = D3DXVECTOR3(0, 0, -1);

	vertexBuf[0].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[1].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[2].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[3].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);

	vertexBuf[0].tex = D3DXVECTOR2(uvStartPoint_.x, uvStartPoint_.y);
	vertexBuf[1].tex = D3DXVECTOR2(uvStartPoint_.x + uvSize_.x, uvStartPoint_.y);
	vertexBuf[2].tex = D3DXVECTOR2(uvStartPoint_.x, uvStartPoint_.y + uvSize_.y);
	vertexBuf[3].tex = D3DXVECTOR2(uvStartPoint_.x + uvSize_.x, uvStartPoint_.y + uvSize_.y);

	vertexBuf_->Unlock();
}

void Object3D::UpdateVertexBuf()
{
	VERTEX_3D* vertexBuf;
	vertexBuf_->Lock(0, 0, (void**)&vertexBuf, 0);

	if (positionIsLeftTop_ == false)
	{
		vertexBuf[0].pos = D3DXVECTOR3(-halfSize_.x,  halfSize_.y,  halfSize_.z);
		vertexBuf[1].pos = D3DXVECTOR3( halfSize_.x,  halfSize_.y,  halfSize_.z);
		vertexBuf[2].pos = D3DXVECTOR3(-halfSize_.x, -halfSize_.y, -halfSize_.z);
		vertexBuf[3].pos = D3DXVECTOR3( halfSize_.x, -halfSize_.y, -halfSize_.z);
	}
	else
	{
		vertexBuf[0].pos = D3DXVECTOR3(0              , 0              , 0);
		vertexBuf[1].pos = D3DXVECTOR3(halfSize_.x * 2, 0              , 0);
		vertexBuf[2].pos = D3DXVECTOR3(0              , halfSize_.y * 2, 0);
		vertexBuf[3].pos = D3DXVECTOR3(halfSize_.x * 2, halfSize_.y * 2, 0);
	}

	vertexBuf[0].normal = D3DXVECTOR3(0, 0, -1);
	vertexBuf[1].normal = D3DXVECTOR3(0, 0, -1);
	vertexBuf[2].normal = D3DXVECTOR3(0, 0, -1);
	vertexBuf[3].normal = D3DXVECTOR3(0, 0, -1);

	vertexBuf[0].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[1].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[2].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[3].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);

	vertexBuf[0].tex = D3DXVECTOR2(uvStartPoint_.x, uvStartPoint_.y);
	vertexBuf[1].tex = D3DXVECTOR2(uvStartPoint_.x + uvSize_.x, uvStartPoint_.y);
	vertexBuf[2].tex = D3DXVECTOR2(uvStartPoint_.x, uvStartPoint_.y + uvSize_.y);
	vertexBuf[3].tex = D3DXVECTOR2(uvStartPoint_.x + uvSize_.x, uvStartPoint_.y + uvSize_.y);

	vertexBuf_->Unlock();
}

void Object3D::UpdateWorldMatrix()
{
	D3DXMATRIX translateMatrix, rotateMatrix, scaleMatrix;
	D3DXVECTOR3 position = GetPosition();

	D3DXMatrixTranslation(&translateMatrix, position.x, position.y, position.z);
	D3DXMatrixRotationQuaternion(&rotateMatrix, &quaternion_);
	D3DXMatrixScaling(&scaleMatrix, scale_.x, scale_.y, scale_.z);

	D3DXMatrixIdentity(&worldMatrix_);
	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &scaleMatrix);
	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &rotateMatrix);
	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &translateMatrix);
}
