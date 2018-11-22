// author : yusuke seki
// data   : 20181110
#ifndef OBJECTMODEL_H_
#define OBJECTMODEL_H_

#include "Object.h"
#include <string>

class ObjectModel : public Object
{
public:
	ObjectModel();
	ObjectModel(const Object::TYPE& type);
	virtual ~ObjectModel();

	static ObjectModel* Create(const D3DXVECTOR3& _position, const std::string& _fileName);
	virtual void Init(const D3DXVECTOR3& _position, const std::string& _fileName);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	D3DXMATRIX GetWorldMatrix();

	void SetPosition(const D3DXVECTOR3& _position);
	void MovePosition(const D3DXVECTOR3& _move);

	void SetRotate(const D3DXVECTOR3& _rotate);
	void MoveRotate(const D3DXVECTOR3& _move);
	D3DXVECTOR3 GetRotate();

	void SetScale(const D3DXVECTOR3& _scale);
	void MoveScale(const D3DXVECTOR3& _move);
	D3DXVECTOR3 GetScale();
	D3DXVECTOR3 GetSize();
	D3DXVECTOR3 GetHalfSize();
	float GetRadius();

	void SetColor(const unsigned int& _rgba);
	void SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a);
	void MoveColor(const int& _r, const int& _g, const int& _b, const int& _a);

	void SetFront(const D3DXVECTOR3& _front);
	D3DXVECTOR3 GetFront();

	void SetRotateToPosition(const D3DXVECTOR3& _position);
	D3DXVECTOR3 GetRotateToPosition(const D3DXVECTOR3& _position);

	void SetTexture(const LPDIRECT3DTEXTURE9& _texture);
	LPDIRECT3DTEXTURE9 GetTexture();

	void SetIsDraw(const bool& _isDraw);

	void SetWorldMatrix(const D3DXMATRIX& _worldMatrix);

	void SetUpdateWorldMatrix(bool _isUpdate);

	D3DXVECTOR3 GetCenterVertex();
	D3DXVECTOR3 GetMinVertex();
	D3DXVECTOR3 GetMaxVertex();

private:
	void UpdateVertexBuf();
	virtual void UpdateWorldMatrix();
	void LoadMeshModel_DX(const char* _fileName);
	void LoadModelSizeFromX(const char* _fileName);

	LPD3DXMESH mesh_;
	D3DMATERIAL9* material_;
	DWORD numMaterial_;
	LPDIRECT3DTEXTURE9* texture_;

	D3DXMATRIX worldMatrix_;
	D3DXVECTOR3 halfSize_;

	D3DXQUATERNION quaternion_;

	D3DXVECTOR3 rotate_;
	D3DXVECTOR3	scale_;
	float radius_;
	Color color_;
	D3DXVECTOR3 front_;

	bool isUpdateVertexBuf_;
	bool isUpdateWorldMatrix_;
	bool isDraw_;

	D3DXVECTOR3 centerVertex_, minVertex_, maxVertex_;

};

#endif
