//*****************************************************************************
//
//		アイコン
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _ICON_H_
#define _ICON_H_

#include "main.h"
#include "ObjectBillboard.h"
#include "ObjectModel.h"

class Camera;


class Icon : public ObjectBillboard
{
public:
	//----- コンストラクタ / デストラクタ -----
	Icon();
	~Icon();

	//----- 基本的な関数 -----
	static Icon* Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera, Object::GROUP group);
	void Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera, Object::GROUP group);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//----- 増減処理 -----

	//----- 設定処理 -----

	//----- データを返す処理 -----


private:
	//----- データ -----

};

#endif

