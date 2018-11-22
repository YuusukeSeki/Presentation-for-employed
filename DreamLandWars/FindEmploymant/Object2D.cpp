// author : yusuke seki
// data   : 20181111
#include "Object2D.h"
#include "renderer.h"
#include "texture.h"

Object2D::Object2D() :Object(Object::TYPE::TYPE_2D)
{
	halfSize_ = D3DXVECTOR3(0, 0, 0);
	color_.color = 0x00000000;
	uvStartPoint_ = D3DXVECTOR2(0, 0);
	uvSize_ = D3DXVECTOR2(0, 0);
	radius_ = 0;
	angle_ = 0;
	rotate_ = 0;
	vertexBuf_ = nullptr;
	texture_ = nullptr;
	isUpdateVertexBuf_ = false;
	isDraw_ = true;
	positionIsLeftTop_ = false;
}

Object2D::Object2D(const Object::TYPE& type) :Object(type)
{
	halfSize_ = D3DXVECTOR3(0, 0, 0);
	color_.color = 0x00000000;
	uvStartPoint_ = D3DXVECTOR2(0, 0);
	uvSize_ = D3DXVECTOR2(0, 0);
	radius_ = 0;
	angle_ = 0;
	rotate_ = 0;
	vertexBuf_ = nullptr;
	texture_ = nullptr;
	isUpdateVertexBuf_ = false;
	isDraw_ = true;
}

Object2D::~Object2D()
{
	Uninit();
}

Object2D* Object2D::Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& size, bool _positionIsLeftTop)
{
	Object2D* object = new Object2D(Object::TYPE::TYPE_2D);
	object->Init(_position, size, _positionIsLeftTop);

	return object;
}

void Object2D::Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, bool _positionIsLeftTop)
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	SetPosition(_position);

	SetSize(_size);

	SetRotate(0.0f);

	SetColor(0xffffffff);

	SetUv_StartPoint(D3DXVECTOR2(0, 0));

	SetUv_Size(D3DXVECTOR2(1, 1));

	positionIsLeftTop_ = _positionIsLeftTop;

	MakeVertexBuf();

	isDraw_ = true;
}

void Object2D::Uninit(void)
{
	if (vertexBuf_ != nullptr)
	{
		vertexBuf_->Release();
		vertexBuf_ = nullptr;
	}
}

void Object2D::Draw(void)
{
	if (!isDraw_)
	{
		return;
	}

	if (isUpdateVertexBuf_)
	{
		UpdateVertexBuf();

		isUpdateVertexBuf_ = false;
	}

	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();

	device->SetStreamSource(0, vertexBuf_, 0, sizeof(VERTEX_2D));

	device->SetFVF(FVF_VERTEX_2D);

	device->SetTexture(0, texture_);

	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void Object2D::SetPosition(const D3DXVECTOR3& _position)
{
	Object::SetPosition(_position);

	isUpdateVertexBuf_ = true;
}

void Object2D::MovePosition(const D3DXVECTOR3& _move)
{
	Object::MovePosition(_move);

	isUpdateVertexBuf_ = true;
}

void Object2D::SetSize(const D3DXVECTOR3& _size)
{
	halfSize_ = _size * 0.5f;
	radius_ = sqrtf(halfSize_.x * halfSize_.x + halfSize_.y * halfSize_.y);
	angle_ = atan2f(halfSize_.y, halfSize_.x);

	isUpdateVertexBuf_ = true;
}

void Object2D::MoveSize(const D3DXVECTOR3& _move)
{
	halfSize_ += _move * 0.5f;
	radius_ = sqrtf(halfSize_.x * halfSize_.x + halfSize_.y * halfSize_.y);
	angle_ = atan2f(halfSize_.y, halfSize_.x);

	isUpdateVertexBuf_ = true;
}

D3DXVECTOR3 Object2D::GetSize()
{
	D3DXVECTOR3 size = halfSize_ * 2;
	return size;
}

D3DXVECTOR3 Object2D::GetHalfSize()
{
	return halfSize_;
}

float Object2D::GetRadius()
{
	return radius_;
}

void Object2D::SetRotate(const float& _rotate)
{
	rotate_ = _rotate;

	isUpdateVertexBuf_ = true;
}

void Object2D::MoveRotate(const float& _move)
{
	rotate_ += _move;
	
	isUpdateVertexBuf_ = true;
}

float Object2D::GetRotate()
{
	return rotate_;
}

void Object2D::SetColor(const unsigned int& _rgba)
{
	color_.color = _rgba;

	isUpdateVertexBuf_ = true;
}

void Object2D::SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a)
{
	color_.rgba[3] = _r;
	color_.rgba[2] = _g;
	color_.rgba[1] = _b;
	color_.rgba[0] = _a;

	isUpdateVertexBuf_ = true;
}

void Object2D::MoveColor(const int& _r, const int& _g, const int& _b, const int& _a)
{
	color_.rgba[3] = color_.rgba[3] + _r >= 255 ? 255 : color_.rgba[3] + _r;
	color_.rgba[2] = color_.rgba[2] + _g >= 255 ? 255 : color_.rgba[2] + _g;
	color_.rgba[1] = color_.rgba[1] + _b >= 255 ? 255 : color_.rgba[1] + _b;
	color_.rgba[0] = color_.rgba[0] + _a >= 255 ? 255 : color_.rgba[0] + _a;
	color_.rgba[3] = color_.rgba[3] + _r <=   0 ?   0 : color_.rgba[3] + _r;
	color_.rgba[2] = color_.rgba[2] + _g <=   0 ?   0 : color_.rgba[2] + _g;
	color_.rgba[1] = color_.rgba[1] + _b <=   0 ?   0 : color_.rgba[1] + _b;
	color_.rgba[0] = color_.rgba[0] + _a <=   0 ?   0 : color_.rgba[0] + _a;

	isUpdateVertexBuf_ = true;
}

void Object2D::SetUv_StartPoint(const D3DXVECTOR2& _Uv_StartPoint)
{
	uvStartPoint_ = _Uv_StartPoint;

	isUpdateVertexBuf_ = true;
}

void Object2D::MoveUv_StartPoint(const D3DXVECTOR2& _move)
{
	uvStartPoint_ += _move;

	isUpdateVertexBuf_ = true;
}

D3DXVECTOR2 Object2D::GetUv_StartPoint()
{
	return uvStartPoint_;
}

void Object2D::SetUv_Size(const D3DXVECTOR2& _Uv_Size)
{
	uvSize_ = _Uv_Size;

	isUpdateVertexBuf_ = true;
}

void Object2D::MoveUv_Size(const D3DXVECTOR2& _move)
{
	uvSize_ += _move;

	isUpdateVertexBuf_ = true;
}

D3DXVECTOR2 Object2D::GetUv_Size()
{
	return uvSize_;
}

void Object2D::SetTexture(const LPDIRECT3DTEXTURE9& _texture)
{
	texture_ = _texture;
}

LPDIRECT3DTEXTURE9 Object2D::GetTexture()
{
	return texture_;
}

void Object2D::SetDraw(const bool& _isDraw)
{
	isDraw_ = _isDraw;
}

void Object2D::MakeVertexBuf()
{
	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();
	VERTEX_2D* vertexBuf;
	D3DXVECTOR3 position = GetPosition();

	if (vertexBuf_ == nullptr)
	{
		HRESULT hr = device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &vertexBuf_, NULL);

		if (FAILED(hr))
		{
			_MSGERROR("頂点バッファの生成に失敗しました", "Object2D::MakeVertexBuf");

			Release();

			return;
		}
	}

	vertexBuf_->Lock(0, 0, (void**)&vertexBuf, 0);

	if (positionIsLeftTop_ == false)
	{
		vertexBuf[0].pos = D3DXVECTOR3(position.x - halfSize_.x, position.y - halfSize_.y, 0.0f);
		vertexBuf[1].pos = D3DXVECTOR3(position.x + halfSize_.x, position.y - halfSize_.y, 0.0f);
		vertexBuf[2].pos = D3DXVECTOR3(position.x - halfSize_.x, position.y + halfSize_.y, 0.0f);
		vertexBuf[3].pos = D3DXVECTOR3(position.x + halfSize_.x, position.y + halfSize_.y, 0.0f);
	}
	else
	{
		vertexBuf[0].pos = D3DXVECTOR3(position.x                  , position.y                  , 0.0f);
		vertexBuf[1].pos = D3DXVECTOR3(position.x + halfSize_.x * 2, position.y                  , 0.0f);
		vertexBuf[2].pos = D3DXVECTOR3(position.x                  , position.y + halfSize_.y * 2, 0.0f);
		vertexBuf[3].pos = D3DXVECTOR3(position.x + halfSize_.x * 2, position.y + halfSize_.y * 2, 0.0f);
	}

	vertexBuf[0].rhw = 1.0f;
	vertexBuf[1].rhw = 1.0f;
	vertexBuf[2].rhw = 1.0f;
	vertexBuf[3].rhw = 1.0f;

	vertexBuf[0].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[1].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[2].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[3].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);

	vertexBuf[0].tex = D3DXVECTOR2(uvStartPoint_.x             , uvStartPoint_.y              );
	vertexBuf[1].tex = D3DXVECTOR2(uvStartPoint_.x + uvSize_.x, uvStartPoint_.y              );
	vertexBuf[2].tex = D3DXVECTOR2(uvStartPoint_.x             , uvStartPoint_.y + uvSize_.y);
	vertexBuf[3].tex = D3DXVECTOR2(uvStartPoint_.x + uvSize_.x, uvStartPoint_.y + uvSize_.y);

	vertexBuf_->Unlock();
}

void Object2D::UpdateVertexBuf()
{
	D3DXVECTOR3 position = GetPosition();
	VERTEX_2D* vertexBuf;

	vertexBuf_->Lock(0, 0, (void**)&vertexBuf, 0);

	if (positionIsLeftTop_ == false)
	{
		vertexBuf[0].pos = D3DXVECTOR3( position.x + cosf(rotate_ + angle_ + D3DX_PI) * radius_
									  , position.y + sinf(rotate_ + angle_ + D3DX_PI) * radius_, 0.0f);
		vertexBuf[1].pos = D3DXVECTOR3( position.x + cosf(rotate_ - angle_          ) * radius_
									  , position.y + sinf(rotate_ - angle_          ) * radius_, 0.0f);
		vertexBuf[2].pos = D3DXVECTOR3( position.x + cosf(rotate_ - angle_ + D3DX_PI) * radius_
									  , position.y + sinf(rotate_ - angle_ + D3DX_PI) * radius_, 0.0f);
		vertexBuf[3].pos = D3DXVECTOR3( position.x + cosf(rotate_ + angle_          ) * radius_
									  , position.y + sinf(rotate_ + angle_          ) * radius_, 0.0f);
	}
	else
	{
		position += halfSize_;

		vertexBuf[0].pos = D3DXVECTOR3( position.x + cosf(rotate_ + angle_ + D3DX_PI) * radius_
									  , position.y + sinf(rotate_ + angle_ + D3DX_PI) * radius_, 0.0f);
		vertexBuf[1].pos = D3DXVECTOR3( position.x + cosf(rotate_ - angle_          ) * radius_
									  , position.y + sinf(rotate_ - angle_          ) * radius_, 0.0f);
		vertexBuf[2].pos = D3DXVECTOR3( position.x + cosf(rotate_ - angle_ + D3DX_PI) * radius_
									  , position.y + sinf(rotate_ - angle_ + D3DX_PI) * radius_, 0.0f);
		vertexBuf[3].pos = D3DXVECTOR3( position.x + cosf(rotate_ + angle_          ) * radius_
									  , position.y + sinf(rotate_ + angle_          ) * radius_, 0.0f);
	}

	vertexBuf[0].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[1].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[2].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	vertexBuf[3].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);

	vertexBuf[0].tex = D3DXVECTOR2(uvStartPoint_.x              , uvStartPoint_.y              );
	vertexBuf[1].tex = D3DXVECTOR2(uvStartPoint_.x + uvSize_.x, uvStartPoint_.y              );
	vertexBuf[2].tex = D3DXVECTOR2(uvStartPoint_.x              , uvStartPoint_.y + uvSize_.y);
	vertexBuf[3].tex = D3DXVECTOR2(uvStartPoint_.x + uvSize_.x, uvStartPoint_.y + uvSize_.y);

	vertexBuf_->Unlock();
}
