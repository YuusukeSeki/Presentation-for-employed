//*****************************************************************************
//
//		タッチスクリーン
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _TOUCHSCREEN_H_
#define _TOUCHSCREEN_H_

#include "main.h"
#include "Object3D_Quaternion.h"


class TouchScreen : public Object3D_Quaternion
{
public:
	//----- コンストラクタ / デストラクタ -----
	TouchScreen();
	TouchScreen(Object::TYPE type);
	virtual ~TouchScreen();

	//----- 基本的な関数 -----
	static TouchScreen* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:


};

#endif
