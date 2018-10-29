//*****************************************************************************
//	
//		バレット（ビルボード）
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _BULLETBILLBOARD_H_
#define _BULLETBILLBOARD_H_

#include "main.h"
#include "Bullet3D.h"

class Camera;


class BulletBillboard : public Bullet3D
{
public:
	// コンストラクタ / デストラクタ
	BulletBillboard();
	BulletBillboard(Object::TYPE type);
	virtual ~BulletBillboard();

	// 基本的な関数
	static BulletBillboard* Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera);
	virtual void Uninit(void);
	virtual void Update(void) {}
	virtual void Draw(void);
	

	// カメラの設定
	void SetCamera(Camera& pCamera) { m_pCamera = &pCamera; }


	// 持っているカメラのポインタを返す
	Camera* GetCamera() { return m_pCamera; }	


private:
	// ワールドマトリクスの更新
	void UpdateWorldMatrix();


	//----- データ -----
	Camera* m_pCamera;	// カメラ

};


#endif

