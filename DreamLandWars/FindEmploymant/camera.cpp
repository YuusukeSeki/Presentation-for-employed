//*****************************************************************************
//
//		カメラ
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "camera.h"
#include "renderer.h"
#include <math.h>
#include "GameManager.h"
#include "MainGame.h"
#include "skybox.h"
//#include "input.h"



//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Camera::Camera()
{
	// メンバ変数初期化
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProj);
	D3DXMatrixIdentity(&m_mtxZRot);
	D3DXMatrixIdentity(&m_mtxTrans);
	m_posEye = m_posAt  = m_vecUp  = m_distance = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_rot = 0.f;

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Camera::~Camera()
{
	Uninit();
}


//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
Camera* Camera::Create(D3DXVECTOR3 posEye, D3DXVECTOR3 posAt, D3DXVECTOR3 vecUp, float fNear, float fFar)
{
	Camera* pCamera = new Camera;
	pCamera->Init(posEye, posAt, vecUp, fNear, fFar);

	return pCamera;
}


//-----------------------------------------------------------------------------
// 解放処理
//-----------------------------------------------------------------------------
void Camera::Release()
{
	delete this;
}


//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Camera::Init(D3DXVECTOR3 posEye, D3DXVECTOR3 posAt, D3DXVECTOR3 vecUp, float fNear, float fFar)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 位置設定
	m_posEye = posEye;

	// 注視点設定
	m_posAt = posAt;

	// 上方向設定
	m_vecUp = vecUp;

	// 視点と注視点との距離を求める
	m_distance = m_posAt - m_posEye;
	//float dx = m_posAt.x - m_posEye.x;
	//float dy = m_posAt.y - m_posEye.y;
	//float dz = m_posAt.z - m_posEye.z;
	//m_distance.z = sqrtf((dx * dx) + (dy * dy) + (dz * dz));

	// 基準軸からの回転率を設定
	m_rot = D3DXToRadian(90);

	// 注視点の再設定
	m_posAt.x = m_posEye.x + m_distance.x * cosf(m_rot);
	m_posAt.y = 0.0f;
	m_posAt.z = m_posEye.z + m_distance.z * sinf(m_rot);

	// ビュー行列を生成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posEye, &m_posAt, &m_vecUp);

	// デバイスのビュー変換行列を設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリクスの生成
	D3DXMatrixPerspectiveFovLH(&m_mtxProj,							// パースペクティブ変換
							   D3DX_PI * 0.3333f,					// 視野角
							   (float)SCREEN_WIDTH / SCREEN_HEIGHT,	// アスペクト比
							   fNear,								// near
							   fFar);								// far

	// デバイスにプロジェクション変換行列を設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);


	D3DXQuaternionIdentity(&m_quaternion);
}


//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void Camera::Uninit(void)
{
}


//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
void Camera::Update(void)
{
	// ビュー行列を更新
	D3DXMatrixLookAtLH(&m_mtxView, &m_posEye, &m_posAt, &m_vecUp);

	// 行列をデバイスに設定
	Renderer::GetDevice()->SetTransform(D3DTS_VIEW, &m_mtxView);

}


//=============================================================================
// 増減
// 移動（前後）
void Camera::MovePosEye_Z(float moveZ)
{
	D3DXVECTOR3 vecZ, move;

	// Z軸を求める
	vecZ = m_posAt - m_posEye;

	// 上下には移動しない
	vecZ.y = 0.f;

	// 向きだけ欲しい
	D3DXVec3Normalize(&vecZ, &vecZ);

	// 移動値の算出
	move = vecZ * moveZ;

	// 視点と注視点の移動
	m_posEye += move;
	m_posAt += move;

}


// 移動（左右）
void Camera::MovePosEye_X(float moveX)
{
	D3DXVECTOR3 vecZ, vecX, move;

	// Z軸を求める
	vecZ = m_posAt - m_posEye;

	// X軸を求める
	D3DXVec3Cross(&vecX, &vecZ, &m_vecUp);

	// 上下には移動しない
	vecX.y = 0.f;

	// 向きだけ欲しい
	D3DXVec3Normalize(&vecX, &vecX);

	// 移動値の算出
	moveX *= -1;
	move = vecX * moveX;

	// 視点と注視点の移動
	m_posEye += move;
	m_posAt  += move;

}


// 回転
void Camera::MoveRotEye(float move)
{
	move *= -1;
	m_rot += move;
	m_posAt.x = m_posEye.x + cosf(m_rot) * m_distance.z;
	m_posAt.z = m_posEye.z + sinf(m_rot) * m_distance.z;

}


// 旋回
void Camera::MoveTurnEye(float move)
{
	float rotXZ_AtoE = atan2f(m_posEye.z - m_posAt.z, m_posEye.x - m_posAt.x);

	m_rot += move;
	rotXZ_AtoE += move;
	m_posEye.x = m_posAt.x + cosf(rotXZ_AtoE) * m_distance.z;
	m_posEye.z = m_posAt.z + sinf(rotXZ_AtoE) * m_distance.z;

}


//=============================================================================
// 設定
// 視点
void Camera::SetPosEye(D3DXVECTOR3 posEye)
{
	D3DXVECTOR3 move = posEye - m_posEye;

	m_posEye += move;
	m_posAt += move;
}

// 注視点
D3DXVECTOR3 Camera::SetPosAt(D3DXVECTOR3 posAt)
{
	D3DXVECTOR3 move = posAt - m_posAt;

	m_posEye += move;
	m_posAt += move;

	return move;
}

// 上方向
void Camera::SetVecUp(D3DXVECTOR3 vecUp)
{
	m_vecUp = vecUp;
}



