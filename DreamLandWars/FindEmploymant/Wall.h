//*****************************************************************************
//
//		壁
//												Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "ObjectCube.h"

class Wall : public ObjectCube
{
public:
	//----- コンストラクタ / デストラクタ -----
	Wall();
	Wall(Object::TYPE type);
	virtual ~Wall();


	//----- 基本的な関数 -----
	static Wall* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

};

#endif

