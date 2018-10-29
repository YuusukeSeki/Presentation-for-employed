//*****************************************************************************
//	
//		兵士の弾
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _SOLDIERBULLET_H_
#define _SOLDIERBULLET_H_

#include "main.h"
#include "BulletBillboard.h"

class Camera;
class ObjectModel;


class SoldierBullet : public BulletBillboard
{
public:
	// コンストラクタ / デストラクタ
	SoldierBullet();
	virtual ~SoldierBullet();

	// 基本的な関数
	static SoldierBullet* CreateBuffer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 兵士弾の動的生成処理
	// position : 生成位置
	// pTarget  : ターゲット
	static void SetBullet(D3DXVECTOR3& position, ObjectModel *pTarget, Camera *pCamera);


private:
	// 兵士弾の設定処理
	void SetBullet_private(D3DXVECTOR3& position, ObjectModel *pTarget, Camera *pCamera);


	//----- データ -----
	static const float bulletDamage_cast;	// 標的：キャストの場合の兵士弾のダメージ
	static const float bulletDamage_soldier;	// 標的：兵士の場合の兵士弾のダメージ

	ObjectModel *m_pTarget;	// ターゲットのポインタ
	int m_cntEraseFrame;	// 消去までの時間
	D3DXVECTOR3 m_front;	// 進む方向
	bool m_bTracking;		// 追尾フラグ

};


#endif

