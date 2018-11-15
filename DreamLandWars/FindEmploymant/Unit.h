// author : yusuke seki
// data   : 20181115
#ifndef UNIT_H_
#define UNIT_H_

#include "Object.h"
#include "Part.h"
#include <map>
#include <string>
class Part;

class Unit : public Object
{
public:
	Unit(const Object::TYPE& _type);
	virtual ~Unit();

	void Release();

	void Init(const D3DXVECTOR3& _position);
	void Uninit();
	void Update();
	void Draw();

	void SetPosition(const D3DXVECTOR3& _position);
	virtual void MovePosition(const D3DXVECTOR3& _move);

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

	void SetGroup(const Object::GROUP& _group);

	void SetActive(const bool& _isActive);

protected:
	void AddPart(const std::string& _key, const std::string& _fileName);
	void ErasePart(const std::string& _key);

	void SetPartPosition(const std::string& _key, const D3DXVECTOR3& _position);

	void SetPartRotate(const std::string& _key, const D3DXVECTOR3& _rotate);

	void SetPartScale(const std::string& _key, const D3DXVECTOR3& _scale);

	void SetParent(const std::string& _childKey, const std::string& _parentKey);

	Part* GetPart(const std::string& _key);

	void ResetRadius();

private:
	union Color
	{
		unsigned int color;
		unsigned char rgba[4];
	};

	void UpdateWorldMatrix();

	std::map<std::string, Part*> objectParts_;

	D3DXMATRIX worldMatrix_;
	D3DXVECTOR3 rotate_;
	D3DXVECTOR3	scale_;
	D3DXVECTOR3 halfSize_;
	float radius_;
	Color color_;
	D3DXVECTOR3 front_;

	bool isUpdateWorldMatrix_;

};

#endif
