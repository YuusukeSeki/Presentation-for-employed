// author : yusuke seki
// data   : 20181121
#ifndef WALL_H_
#define WALL_H_

#include "ObjectModel.h"
class Player;

class Wall : public ObjectModel
{
public:
	enum Collide
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
	};

	Wall();
	~Wall();

	static Wall* Create(const D3DXVECTOR3& _position, const std::string& _fileName, const Collide& _collide);
	void Init(const D3DXVECTOR3& _position, const std::string& _fileName, const Collide& _collide);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetCollidedPosition(Player& _player);

private:
	bool IsCollided(Player& _player);

	Collide collde_;

};

#endif

