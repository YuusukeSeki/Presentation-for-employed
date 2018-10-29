//*****************************************************************************
//	ライト[light.cpp]
//	Autohr	:	Yusuke Seki
//*****************************************************************************

//-----------------------------------------------------------------------------
//	インクルードファイル
//-----------------------------------------------------------------------------
#include "light.h"
#include "renderer.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Light::Light()
{
	// メンバ変数初期化
	ZeroMemory( &m_light , sizeof( D3DLIGHT9   ) );
	ZeroMemory( &m_vecDir, sizeof( D3DXVECTOR3 ) );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Light::~Light()
{
	Uninit();
}

//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Light* Light::Create(void)
{
	Light* pLight = new Light;
	pLight->Init();

	return pLight;
}
	

//-----------------------------------------------------------------------------
// 解放処理
//-----------------------------------------------------------------------------
void Light::Release(void)
{
	delete this;
}


//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Light::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// ライトの設定
	m_light.Type		= D3DLIGHT_DIRECTIONAL;							// 平行光源
	m_light.Diffuse		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// 拡散光（ light.Typeの色 ）
	m_light.Ambient		= D3DXCOLOR( 0.7f, 0.7f, 0.7f, 1.0f );			// 環境光
	m_light.Specular	= D3DXCOLOR( 0.7f, 0.7f, 0.7f, 1.0f );			// 鏡面光
	m_vecDir			= D3DXVECTOR3( 0.f , -1.0f , 0.f );				// ライトの向き
	D3DXVec3Normalize( ( D3DXVECTOR3* )&m_light.Direction, &m_vecDir );	// 単位ベクトル化
	pDevice->SetLight( 0, &m_light );									// ライトを設定
	pDevice->LightEnable( 0, TRUE );									// 使います
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );					// 全体のライトを有効にする
}

//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void Light::Uninit(void)
{
}

//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
void Light::Update(void)
{
}

//-----------------------------------------------------------------------------
//	拡散光変更
//-----------------------------------------------------------------------------
void Light::SetDiffuse(float r, float g, float b, float a)
{
	m_light.Diffuse.r = r;
	m_light.Diffuse.g = g;
	m_light.Diffuse.b = b;
	m_light.Diffuse.a = a;
}

//-----------------------------------------------------------------------------
//	環境光変更
//-----------------------------------------------------------------------------
void Light::SetAmbient(float r, float g, float b, float a)
{
	m_light.Ambient.r = r;
	m_light.Ambient.g = g;
	m_light.Ambient.b = b;
	m_light.Ambient.a = a;
}

//-----------------------------------------------------------------------------
//	鏡面光変更
//-----------------------------------------------------------------------------
void Light::SetSpecular(float r, float g, float b, float a)
{
	m_light.Specular.r = r;
	m_light.Specular.g = g;
	m_light.Specular.b = b;
	m_light.Specular.a = a;
}

// 向きの設定
// vecDirection : 設定したい向き
void Light::SetVectorDirection(D3DXVECTOR3& vecDirection)
{
	m_vecDir = vecDirection;
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &m_vecDir);
	Renderer::GetDevice()->SetLight(0, &m_light);

}
