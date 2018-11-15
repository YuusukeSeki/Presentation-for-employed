// author : yusuke seki
// data   : 20181112

#include "RelayPoint.h"

RelayPoint::RelayPoint(const D3DXVECTOR3& _position)
{
	position_ = _position;
	frontRelayPoint_ = nullptr;
	backRelayPoint_ = nullptr;
}

RelayPoint::~RelayPoint()
{

}

RelayPoint* RelayPoint::Create(const D3DXVECTOR3& _position)
{
	return new RelayPoint(_position);
}

void RelayPoint::Release()
{
	delete this;
}

void RelayPoint::SetFrontRelayPoint(RelayPoint* _frontRelayPoint)
{
	frontRelayPoint_ = _frontRelayPoint;
}

void RelayPoint::SetBackRelayPoint(RelayPoint* _backRelayPoint)
{
	backRelayPoint_ = _backRelayPoint;
}

RelayPoint* RelayPoint::GetNextRelayPoint(Object::GROUP _group)
{
	if (_group == Object::GROUP::GROUP_A)
	{
		return frontRelayPoint_;
	}
	else if (_group == Object::GROUP::GROUP_B)
	{
		return backRelayPoint_;
	}
	else
	{
		_MSGERROR("所属グループの値が範囲外です", "RelayPoint::GetNextRelayPoint");
		return nullptr;
	}
}

D3DXVECTOR3 RelayPoint::GetPosition()
{
	return position_;
}
