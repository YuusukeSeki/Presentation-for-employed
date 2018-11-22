//*****************************************************************************
//
//		ゲームタイトル
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _GAMETITLE_H_
#define _GAMETITLE_H_

#include "main.h"
#include "Object3D.h"


class GameTitle : public Object3D
{
public:
	//----- コンストラクタ / デストラクタ -----
	GameTitle();
	GameTitle(Object::TYPE type);
	virtual ~GameTitle();
	
	//----- 基本的な関数 -----
	static GameTitle* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:


};

#endif
