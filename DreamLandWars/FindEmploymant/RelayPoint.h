// author : yusuke seki
// data   : 20181112

#include "Object.h"

class RelayPoint
{
public:
	enum Point
	{
		FRIENDCASTLECENTER,
		FRIENDCASTLELEFT,
		FRIENDCASTLERIGHT,
		FRIENDLEFT,
		FRIENDRIGHT,
		ENEMYCASTLECENTER,
		ENEMYCASTLELEFT,
		ENEMYCASTLERIGHT,
		ENEMYLEFT,
		ENEMYRIGHT,
	};

	RelayPoint(const D3DXVECTOR3& _position);
	~RelayPoint();

	static RelayPoint* Create(const D3DXVECTOR3& _position);
	void Release();

	void SetFrontRelayPoint(RelayPoint* _frontRelayPoint);
	void SetBackRelayPoint(RelayPoint* _backRelayPoint);

	RelayPoint* GetNextRelayPoint(Object::GROUP _group);
	
	void SetPosition(const D3DXVECTOR3& _position);
	D3DXVECTOR3 GetPosition();

private:
	D3DXVECTOR3 position_;
	RelayPoint* frontRelayPoint_;
	RelayPoint* backRelayPoint_;

};
