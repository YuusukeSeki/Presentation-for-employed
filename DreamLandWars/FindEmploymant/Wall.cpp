//*****************************************************************************
//
//		•Ç
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "Wall.h"
#include "player.h"

Wall::Wall() : ObjectModel(Object::TYPE::TYPE_3D_WALL)
{

}

Wall::~Wall()
{
	Uninit();
}

Wall* Wall::Create(const D3DXVECTOR3& _position, const std::string& _fileName, const Collide& _collide)
{
	Wall* wall = new Wall();

	wall->Init(_position, _fileName, _collide);

	return wall;
}

void Wall::Init(const D3DXVECTOR3& _position, const std::string& _fileName, const Collide& _collide)
{
	ObjectModel::Init(_position, _fileName);

	collde_ = _collide;
}

void Wall::Uninit(void)
{
	ObjectModel::Uninit();
}

void Wall::Update(void)
{

}

void Wall::Draw(void)
{
	ObjectModel::Draw();
}

D3DXVECTOR3 Wall::GetCollidedPosition(Player& _player)
{
	D3DXVECTOR3 collidedPosition(_player.GetPosition());

	if (IsCollided(_player) == true)
	{
		switch (collde_)
		{
		case Collide::FRONT:
			collidedPosition.z = GetPosition().z - GetHalfSize().z - _player.GetHalfSize().z;
			break;

		case Collide::BACK:
			collidedPosition.z = GetPosition().z + GetHalfSize().z + _player.GetHalfSize().z;
			break;

		case Collide::LEFT:
			collidedPosition.x = GetPosition().x + GetHalfSize().x + _player.GetHalfSize().x;
			break;

		case Collide::RIGHT:
			collidedPosition.x = GetPosition().x - GetHalfSize().x - _player.GetHalfSize().x;
			break;
		}
	}

	return collidedPosition;
}

bool Wall::IsCollided(Player& _player)
{
	D3DXVECTOR3 colliderPosition = _player.GetObjectCollider()->GetPosition();

	switch (collde_)
	{
		case Collide::FRONT:
			return _player.GetObjectCollider()->GetPosition().z - _player.GetObjectCollider()->GetRadius()
				< GetPosition().z + GetHalfSize().z ? true : false;

		case Collide::BACK:
			return _player.GetObjectCollider()->GetPosition().z + _player.GetObjectCollider()->GetRadius()
				> GetPosition().z - GetHalfSize().z ? true : false;

		case Collide::LEFT:
			return _player.GetObjectCollider()->GetPosition().x + _player.GetObjectCollider()->GetRadius()
				> GetPosition().x - GetHalfSize().z ? true : false;

		case Collide::RIGHT:
			return _player.GetObjectCollider()->GetPosition().x - _player.GetObjectCollider()->GetRadius()
				< GetPosition().x + GetHalfSize().z ? true : false;

		default:
			_MSGERROR("error", "Wall::IsCollided");
			return false;
	}
}
