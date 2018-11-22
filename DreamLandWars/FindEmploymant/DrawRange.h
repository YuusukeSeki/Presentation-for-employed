// author : yusuke seki
// data   : 20181111
#ifndef DRAWRANGE_H_
#define DRAWRANGE_H_

#include "Object3D.h"
class Player;

class DrawRange
{
public:
	DrawRange(Player* _parentPlayer);
	~DrawRange();
	static DrawRange* Create(Player* _parentPlayer);
	void Init();
	void Uninit();
	void Update();
	void Release();

	void SetColor(const unsigned int& _rgba);
	void SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a);
	void SetTexture(const LPDIRECT3DTEXTURE9& _texture);
	
private:
	DrawRange();

	void UpdateIsDraw();
	void SneakPlayerPosition();
	bool IsChangeSize();
	void ChangeSize();

	Player* const parentPlayer_;
	Object3D* object_;
	float preRange_;

};

#endif

