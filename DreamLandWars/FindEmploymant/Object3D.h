// author : yusuke seki
// data   : 20181111
#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include "Object.h"

class Object3D : public Object
{
public:
	Object3D();
	Object3D(const Object::TYPE& type);
	virtual ~Object3D();

	static Object3D* Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size);
	virtual void Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size);
	virtual void Uninit();
	virtual void Update() {}
	virtual void Draw();

	D3DXMATRIX GetWorldMatrix();
	void SetPosition(const D3DXVECTOR3& _position);
	void MovePosition(const D3DXVECTOR3& _move);

	void SetSize(const D3DXVECTOR3& _size);
	void MoveSize(const D3DXVECTOR3& _move);
	D3DXVECTOR3 GetSize();
	D3DXVECTOR3 GetHalfSize();
	float GetRadius();

	void SetRotate(const D3DXVECTOR3& _vector, const float& _angle);
	void MoveRotate(const D3DXVECTOR3& _vector, const float& _moveAngle);
	D3DXQUATERNION& GetQuaternion();

	void SetScale(const D3DXVECTOR3& _scale);
	void MoveScale(const D3DXVECTOR3& _move);
	D3DXVECTOR3 GetScale();

	void SetColor(const unsigned int& _rgba);
	void SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a);
	void MoveColor(const int& _r, const int& _g, const int& _b, const int& _a);
	unsigned int GetColor();

	void SetUv_StartPoint(const D3DXVECTOR2& _uvStartPoint);
	void MoveUv_StartPoint(const D3DXVECTOR2& _move);
	D3DXVECTOR2 GetUv_StartPoint();

	void SetUv_Size(const D3DXVECTOR2& _uvSize);
	void MoveUv_Size(const D3DXVECTOR2& _move);
	D3DXVECTOR2 GetUv_Size();

	void SetTexture(const LPDIRECT3DTEXTURE9& _texture);
	LPDIRECT3DTEXTURE9 GetTexture();

	void SetIsDraw(const bool& _isDraw);
	bool GetIsDraw();

protected:
	union Color
	{
		unsigned int color;
		unsigned char rgba[4];
	};

	void SetWorldMatrix(const D3DXMATRIX& _worldMatrix);
	void SetUpdateVertexBuf(const bool& _isUpdateVertexBuf);
	void SetUpdateWorldMatrix(const bool& _isUpdateWorldMatrix);
	virtual void UpdateWorldMatrix();

private:
	void MakeVertexBuf();
	void UpdateVertexBuf();

	D3DXMATRIX worldMatrix_;
	D3DXQUATERNION quaternion_;
	D3DXVECTOR3 scale_;
	D3DXVECTOR3 halfSize_;
	float radius_;
	Color color_;
	D3DXVECTOR2 uvStartPoint_;
	D3DXVECTOR2 uvSize_;

	LPDIRECT3DVERTEXBUFFER9 vertexBuf_;
	LPDIRECT3DTEXTURE9 texture_;

	bool isUpdateVertexBuf_;
	bool isUpdateWorldMatrix_;
	bool isDraw_;

};

#endif
