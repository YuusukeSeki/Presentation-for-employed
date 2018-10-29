//*****************************************************************************
//
//		スカイドーム
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _SKYDOME_H_
#define _SKYDOME_H_

#include "main.h"
#include "ObjectModel.h"


class SkyDome : public ObjectModel
{
public:
	//----- コンストラクタ / デストラクタ -----
	SkyDome();
	SkyDome(Object::TYPE type);
	virtual ~SkyDome();

	
	//----- 基本的な関数 -----
	static SkyDome* Create(D3DXVECTOR3& position, const char* FileName);
	virtual void Init(D3DXVECTOR3& position, const char* FileName);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

};

#endif

