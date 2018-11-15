// author : yusuke seki
// data   : 20181115
#ifndef PART_H_
#define PART_H_

#include "main.h"
#include "ObjectModel.h"
#include <string>

class Part : public ObjectModel
{
public:
	Part();
	Part(const Object::TYPE& _type);
	virtual ~Part();

	static Part* Create(const D3DXVECTOR3& _position, const std::string& _fileName, D3DXMATRIX* _parentObjectWorldMatrix);
	void Init(const D3DXVECTOR3& _position, const std::string& _fileName, D3DXMATRIX* _parentObjectWorldMatrix);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParent(Part* _parentPart);
	Part* GetParent();

private:
	void UpdateWorldMatrix();

	Part* parentPart_;
	D3DXMATRIX* parentObjectWorldMatrix_;

};

#endif

