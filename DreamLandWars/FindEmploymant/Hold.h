//*****************************************************************************
//
//		ホールド
//												Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _HOLD_H_
#define _HOLD_H_

#include "main.h"
#include "ObjectBillboard.h"

class ObjectModel;
class Camera;


class Hold : public ObjectBillboard
{
public:
	//----- コンストラクタ / デストラクタ -----
	Hold();
	virtual ~Hold();


	//----- 基本的な関数 -----
	static Hold* Create(ObjectModel* pObjModel, Camera* pCamera);
	virtual void Init(ObjectModel* pObjModel, Camera* pCamera);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//----- 増減処理 -----
	void MovePosition(D3DXVECTOR3& movePosition);

	//----- 設定処理 -----
	// true で"HOLD"表示
	void SetDrawHold(bool bDraw) { m_bDraw = bDraw; }

	// 
	void ReleaseThis();

	//----- データを返す処理 -----


private:
	//----- データ -----
	ObjectModel*     m_pParent;				// 親のポインタ
	ObjectBillboard* m_pBackGround;			// 照準：背景
	ObjectBillboard* m_pLockOn_OutLine;		// 照準：外側
	ObjectBillboard* m_pLockOn_InnerLine;	// 照準：内側
	ObjectBillboard* m_pLockOn_Hold;		// 照準："HOLD"

	bool m_bDraw;	// true で"Hold"表示


};

#endif

