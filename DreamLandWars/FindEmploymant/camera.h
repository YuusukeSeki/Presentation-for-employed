//*****************************************************************************
//
//		カメラ
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"


class Camera
{
public:
	// コンストラクタ / デストラクタ
	Camera();
	~Camera();

	// 基本的な関数
	static Camera* Create(D3DXVECTOR3 posEye, D3DXVECTOR3 posAt, D3DXVECTOR3 vecUp, float fNear, float fFar);
	void Release(void);
	void Init(D3DXVECTOR3 posEye, D3DXVECTOR3 posAt, D3DXVECTOR3 vecUp, float fNear, float fFar);
	void Uninit(void);
	void Update(void);

	// 増減
	void MovePosEye_Z(float moveZ);  // 移動（前後）
	void MovePosEye_X(float moveX);  // 移動（左右）
	void MoveRotEye  (float move);	 // 回転
	void MoveTurnEye (float move);	 // 旋回

	// 設定
	//void SetPosEye(D3DXVECTOR3 posEye); // 視点
	//void SetPosAt(D3DXVECTOR3 posAt);   // 注視点
	//void SetVecUp(D3DXVECTOR3 vecUp);   // 上方向
	void SetPosEye(D3DXVECTOR3 posEye);      // 視点
	D3DXVECTOR3 SetPosAt(D3DXVECTOR3 posAt); // 注視点
	void SetVecUp(D3DXVECTOR3 vecUp);        // 上方向

	// 取得
	D3DXMATRIX GetMtxView() { return m_mtxView; }
	D3DXMATRIX GetMtxProj() { return m_mtxProj; }
	float GetRot() { return m_rot; }
	D3DXVECTOR3 GetVecZ() {
		D3DXVECTOR3 vecZ = m_posAt - m_posEye;
		vecZ.y = 0.f;

		return *D3DXVec3Normalize(&vecZ, &vecZ);
	}
	D3DXVECTOR3 GetVecX() {
		D3DXVECTOR3 vecZ, vecX;
		vecZ = m_posAt - m_posEye;
		D3DXVec3Cross(&vecX, &m_vecUp, &vecZ);
		vecX.y = 0.f;

		return *D3DXVec3Normalize(&vecX, &vecX);
	}
	D3DXVECTOR3 GetVecUp() { return m_vecUp; }
	D3DXVECTOR3 GetVecZ_UnNormal() { return m_posAt - m_posEye; }
	D3DXVECTOR3 GetPosEye() { return m_posEye; }
	D3DXVECTOR3 GetPosAt() { return m_posAt; }

	const D3DXMATRIX* GetMtxView() const { return &m_mtxView; }


protected:


private:
	// 変数
	D3DXMATRIX  m_mtxView;	// ビュー行列
	D3DXMATRIX  m_mtxProj;	// プロジェクション行列
	D3DXMATRIX  m_mtxZRot;	// Z軸回転行列
	D3DXMATRIX  m_mtxTrans;	// 移動行列
	D3DXVECTOR3 m_posEye;	// 視点
	D3DXVECTOR3 m_posAt;	// 注視点
	D3DXVECTOR3 m_vecUp;	// 上方向
	float       m_rot;      // 回転率
	D3DXVECTOR3 m_distance; // 注視点から視点までの距離

	D3DXQUATERNION m_quaternion;


};

#endif
