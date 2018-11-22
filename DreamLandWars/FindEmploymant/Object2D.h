// author : yusuke seki
// data   : 20181111
#ifndef OBJECT2D_H_
#define OBJECT2D_H_

#include "Object.h"

class Object2D : public Object
{
public:
	Object2D();
	Object2D(const Object::TYPE& _type);
	virtual ~Object2D();

	static Object2D* Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, bool _positionIsLeftTop = false);
	virtual void Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, bool _positionIsLeftTop = false);
	virtual void Uninit(void);
	virtual void Update(void) {}
	virtual void Draw(void);

	virtual void SetPosition(const D3DXVECTOR3& _position);
	virtual void MovePosition(const D3DXVECTOR3& _move);

	virtual void SetSize(const D3DXVECTOR3& _size);
	virtual void MoveSize(const D3DXVECTOR3& _move);
	D3DXVECTOR3 GetSize();
	D3DXVECTOR3 GetHalfSize();
	float GetRadius();

	virtual void SetRotate(const float& _rotate);
	virtual void MoveRotate(const float& _move);
	float GetRotate();

	virtual void SetColor(const unsigned int& _rgba);
	virtual void SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a);
	virtual void MoveColor(const int& _r, const int& _g, const int& _b, const int& _a);

	virtual void SetUv_StartPoint(const D3DXVECTOR2& _Uv_StartPoint);
	virtual void MoveUv_StartPoint(const D3DXVECTOR2& _move);
	D3DXVECTOR2 GetUv_StartPoint();

	virtual void SetUv_Size(const D3DXVECTOR2& _Uv_Size);
	virtual void MoveUv_Size(const D3DXVECTOR2& _move);
	D3DXVECTOR2 GetUv_Size();

	void SetTexture(const LPDIRECT3DTEXTURE9& _texture);
	LPDIRECT3DTEXTURE9 GetTexture();

	void SetDraw(const bool& _isDraw);

private:
	void MakeVertexBuf();
	void UpdateVertexBuf();

	D3DXVECTOR3 halfSize_;
	float radius_;
	float angle_;
	float rotate_;
	Color color_;
	D3DXVECTOR2 uvStartPoint_;
	D3DXVECTOR2 uvSize_;

	LPDIRECT3DVERTEXBUFFER9 vertexBuf_;
	LPDIRECT3DTEXTURE9 texture_;

	bool isUpdateVertexBuf_;
	bool isDraw_;

	bool positionIsLeftTop_;

};

#endif
