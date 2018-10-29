//*****************************************************************************
//	
//		エフェクト（ビルボード）
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _EFFECTBILLBOARD_H_
#define _EFFECTBILLBOARD_H_

#include "main.h"
#include "Effect3D.h"

class Camera;


class EffectBillboard : public Effect3D
{
public:
	//----- コンストラクタ / デストラクタ -----
	EffectBillboard() {}
	EffectBillboard(EffectManager::TYPE type);
	virtual ~EffectBillboard();

	
	//----- 基本的な関数 -----
	static EffectBillboard* Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);


	//----- 増減処理 -----


	//----- 設定処理 -----
	static void SetEffect(D3DXVECTOR3& position, D3DXVECTOR3& vecZ, Camera* pCamera, Effect3D::TYPE effectType);	// エフェクトの発生
	void SetCamera(Camera& pCamera) { m_pCamera = &pCamera; }											// カメラの設定


	//----- データを返す処理 -----
	Camera* GetCamera() { return m_pCamera; }	// 持っているカメラのポインタを返す

	
protected:


private:
	//----- 関数 -----
	void UpdateWorldMatrix();	// ワールドマトリクスの更新

	//----- データ -----
	Camera* m_pCamera; // カメラ

};

#endif

