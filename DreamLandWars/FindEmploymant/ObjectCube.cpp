//*****************************************************************************
//
//		キューブ
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "ObjectCube.h"
#include "Object3D.h"
#include "renderer.h"
#include "collision.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
ObjectCube::ObjectCube() :Object(Object::TYPE::TYPE_3D)
{
	// メンバ変数初期化
	_surface_TOP    = nullptr;	// 上面
	_surface_BOTTOM = nullptr;	// 底面
	_surface_LEFT   = nullptr;	// 左面
	_surface_RIGHT  = nullptr;	// 右面
	_surface_FRONT  = nullptr;	// 前面
	_surface_BACK   = nullptr;	// 背面

	m_halfSize           = D3DXVECTOR3(0, 0, 0);
	m_rotate             = D3DXVECTOR3(0, 0, 0);
	m_scale              = D3DXVECTOR3(0, 0, 0);
	m_radius             = 0;
	m_normal[0]          = D3DXVECTOR3(0, 0, 0);
	m_normal[1]          = D3DXVECTOR3(0, 0, 0);
	m_normal[2]          = D3DXVECTOR3(0, 0, 0);
	m_normal[3]          = D3DXVECTOR3(0, 0, 0);
	m_color.color        = 0xffffffff;
	m_UV_StartPoint      = D3DXVECTOR2(0, 0);
	m_UV_Size            = D3DXVECTOR2(0, 0);
	m_pTexture           = nullptr;
	m_bDraw              = true;
}


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
ObjectCube::ObjectCube(Object::TYPE type) :Object(type)
{
	// メンバ変数初期化
	_surface_TOP    = nullptr;	// 上面
	_surface_BOTTOM = nullptr;	// 底面
	_surface_LEFT   = nullptr;	// 左面
	_surface_RIGHT  = nullptr;	// 右面
	_surface_FRONT  = nullptr;	// 前面
	_surface_BACK   = nullptr;	// 背面

	m_halfSize           = D3DXVECTOR3(0, 0, 0);
	m_rotate             = D3DXVECTOR3(0, 0, 0);
	m_scale              = D3DXVECTOR3(0, 0, 0);
	m_radius             = 0;
	m_normal[0]          = D3DXVECTOR3(0, 0, 0);
	m_normal[1]          = D3DXVECTOR3(0, 0, 0);
	m_normal[2]          = D3DXVECTOR3(0, 0, 0);
	m_normal[3]          = D3DXVECTOR3(0, 0, 0);
	m_color.color        = 0xffffffff;
	m_UV_StartPoint      = D3DXVECTOR2(0, 0);
	m_UV_Size            = D3DXVECTOR2(0, 0);
	m_pTexture           = nullptr;
	m_bDraw              = true;
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
ObjectCube::~ObjectCube()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
ObjectCube* ObjectCube::Create(D3DXVECTOR3& pos, D3DXVECTOR3& size)
{
	// 生成
	ObjectCube* pObjectCube = new ObjectCube(Object::TYPE::TYPE_3D_CUBE);

	// 初期化
	pObjectCube->Init(pos, size);

	return pObjectCube;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void ObjectCube::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	//----- データの設定 -----
	Object::SetPosition(position);																// 座標
	m_halfSize           = size * 0.5f;															// 半分の大きさ
	m_rotate             = D3DXVECTOR3(0, 0, 0);												// 回転率
	m_scale              = D3DXVECTOR3(1, 1, 1);												// 拡縮率
	m_radius             = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	m_normal[0]          = D3DXVECTOR3(0, 0, -1);												// 法線
	m_normal[1]          = D3DXVECTOR3(0, 0, -1);												// 法線
	m_normal[2]          = D3DXVECTOR3(0, 0, -1);												// 法線
	m_normal[3]          = D3DXVECTOR3(0, 0, -1);												// 法線
	m_color.color        = 0xffffffff;															// 色
	m_UV_StartPoint      = D3DXVECTOR2(0, 0);													// UV開始点
	m_UV_Size            = D3DXVECTOR2(1, 1);													// UV大きさ
	m_bDraw              = true;																// true で描画処理を行う

	// 上面
	D3DXVECTOR3 setPos  = D3DXVECTOR3(position.x, position.y + size.y * 0.5f, position.z);
	D3DXVECTOR3 setSize = D3DXVECTOR3(size.x, size.z, 0);
	_surface_TOP    = Object3D::Create(setPos, setSize);
	_surface_TOP->SetRotate(D3DXVECTOR3(1, 0, 0), D3DXToRadian(90));

	// 底面
	setPos  = D3DXVECTOR3(position.x, position.y - size.y * 0.5f, position.z);
	_surface_BOTTOM = Object3D::Create(setPos, setSize);
	_surface_BOTTOM->SetRotate(D3DXVECTOR3(1, 0, 0), D3DXToRadian(-90));

	// 左面
	setPos  = D3DXVECTOR3(position.x - size.x * 0.5f, position.y, position.z);
	setSize = D3DXVECTOR3(size.z, size.y, 0);
	_surface_LEFT   = Object3D::Create(setPos, setSize);
	_surface_LEFT->SetRotate(D3DXVECTOR3(0, 1, 0), D3DXToRadian(90));

	// 右面
	setPos = D3DXVECTOR3(position.x + size.x * 0.5f, position.y, position.z);
	_surface_RIGHT  = Object3D::Create(setPos, setSize);
	_surface_RIGHT->SetRotate(D3DXVECTOR3(0, 1, 0), D3DXToRadian(-90));

	// 前面
	setPos = D3DXVECTOR3(position.x, position.y, position.z - size.z * 0.5f);
	setSize = D3DXVECTOR3(size.x, size.y, 0);
	_surface_FRONT  = Object3D::Create(setPos, setSize);

	// 背面
	setPos = D3DXVECTOR3(position.x, position.y, position.z + size.z * 0.5f);
	_surface_BACK   = Object3D::Create(setPos, setSize);
	_surface_BACK->SetRotate(D3DXVECTOR3(0, 1, 0), D3DXToRadian(180));

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void ObjectCube::Uninit(void)
{

}



//=============================================================================
//	増減処理
// 座標の移動
void ObjectCube::MovePosition(D3DXVECTOR3& movePosition)
{
	// 座標の移動
	Object::MovePosition(movePosition);

	_surface_TOP->MovePosition(movePosition);		// 上面
	_surface_BOTTOM->MovePosition(movePosition);	// 底面
	_surface_LEFT->MovePosition(movePosition);		// 左面
	_surface_RIGHT->MovePosition(movePosition);		// 右面
	_surface_FRONT->MovePosition(movePosition);		// 前面
	_surface_BACK->MovePosition(movePosition);		// 背面

}

// 大きさの増減
void ObjectCube::MoveSize(D3DXVECTOR3& moveSize)
{
	// 大きさの増減
	m_halfSize += moveSize * 0.5f;
	m_radius    = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	
	_surface_TOP->MoveSize(moveSize);		// 上面
	_surface_BOTTOM->MoveSize(moveSize);	// 底面
	_surface_LEFT->MoveSize(moveSize);		// 左面
	_surface_RIGHT->MoveSize(moveSize);		// 右面
	_surface_FRONT->MoveSize(moveSize);		// 前面
	_surface_BACK->MoveSize(moveSize);		// 背面

}

// 回転率の増減
void ObjectCube::MoveRotate(D3DXVECTOR3& moveRotate)
{
	// 回転率の増減
	m_rotate += moveRotate;
	
	//_surface_TOP->MoveRotate(moveRotate);		// 上面
	//_surface_BOTTOM->MoveRotate(moveRotate);	// 底面
	//_surface_LEFT->MoveRotate(moveRotate);		// 左面
	//_surface_RIGHT->MoveRotate(moveRotate);		// 右面
	//_surface_FRONT->MoveRotate(moveRotate);		// 前面
	//_surface_BACK->MoveRotate(moveRotate);		// 背面

}

// 拡縮率の増減
void ObjectCube::MoveScale(D3DXVECTOR3& moveScale)
{
	// 拡縮率の増減
	m_scale.x = m_scale.x + moveScale.x < 0 ? 0 : m_scale.x + moveScale.x;
	m_scale.y = m_scale.y + moveScale.y < 0 ? 0 : m_scale.y + moveScale.y;
	m_scale.z = m_scale.z + moveScale.z < 0 ? 0 : m_scale.z + moveScale.z;

	_surface_TOP->MoveScale(moveScale);		// 上面
	_surface_BOTTOM->MoveScale(moveScale);	// 底面
	_surface_LEFT->MoveScale(moveScale);	// 左面
	_surface_RIGHT->MoveScale(moveScale);	// 右面
	_surface_FRONT->MoveScale(moveScale);	// 前面
	_surface_BACK->MoveScale(moveScale);	// 背面

}

// 色の増減
void ObjectCube::MoveColor(int r, int g, int b, int a)
{
	// 色の増減
	m_color.rgba[3] = m_color.rgba[3] + r >= 255 ? 255 : m_color.rgba[3] + r;
	m_color.rgba[2] = m_color.rgba[2] + g >= 255 ? 255 : m_color.rgba[2] + g;
	m_color.rgba[1] = m_color.rgba[1] + b >= 255 ? 255 : m_color.rgba[1] + b;
	m_color.rgba[0] = m_color.rgba[0] + a >= 255 ? 255 : m_color.rgba[0] + a;
	m_color.rgba[3] = m_color.rgba[3] + r <=   0 ?   0 : m_color.rgba[3] + r;
	m_color.rgba[2] = m_color.rgba[2] + g <=   0 ?   0 : m_color.rgba[2] + g;
	m_color.rgba[1] = m_color.rgba[1] + b <=   0 ?   0 : m_color.rgba[1] + b;
	m_color.rgba[0] = m_color.rgba[0] + a <=   0 ?   0 : m_color.rgba[0] + a;

	_surface_TOP->MoveColor(r, g, b, a);	// 上面
	_surface_BOTTOM->MoveColor(r, g, b, a);	// 底面
	_surface_LEFT->MoveColor(r, g, b, a);	// 左面
	_surface_RIGHT->MoveColor(r, g, b, a);	// 右面
	_surface_FRONT->MoveColor(r, g, b, a);	// 前面
	_surface_BACK->MoveColor(r, g, b, a);	// 背面

}

// UV開始点の増減
void ObjectCube::MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint)
{
	// UV開始点の増減
	m_UV_StartPoint += moveUV_StartPoint;

	_surface_TOP->MoveUv_StartPoint(moveUV_StartPoint);		// 上面
	_surface_BOTTOM->MoveUv_StartPoint(moveUV_StartPoint);	// 底面
	_surface_LEFT->MoveUv_StartPoint(moveUV_StartPoint);	// 左面
	_surface_RIGHT->MoveUv_StartPoint(moveUV_StartPoint);	// 右面
	_surface_FRONT->MoveUv_StartPoint(moveUV_StartPoint);	// 前面
	_surface_BACK->MoveUv_StartPoint(moveUV_StartPoint);	// 背面

}

// UV大きさの増減
void ObjectCube::MoveUV_Size(D3DXVECTOR2& moveUV_Size)
{
	// UV開始点の増減
	m_UV_Size += moveUV_Size;

	_surface_TOP->MoveUv_Size(moveUV_Size);		// 上面
	_surface_BOTTOM->MoveUv_Size(moveUV_Size);	// 底面
	_surface_LEFT->MoveUv_Size(moveUV_Size);	// 左面
	_surface_RIGHT->MoveUv_Size(moveUV_Size);	// 右面
	_surface_FRONT->MoveUv_Size(moveUV_Size);	// 前面
	_surface_BACK->MoveUv_Size(moveUV_Size);	// 背面

}



//=============================================================================
// 設定処理
// 座標の設定
void ObjectCube::SetPosition(D3DXVECTOR3& position)
{
	// 上面
	D3DXVECTOR3 vec = _surface_TOP->GetPosition() - GetPosition();
	D3DXVECTOR3 setPos = GetPosition() + vec;
	_surface_TOP->SetPosition(setPos);

	// 底面
	vec = _surface_BOTTOM->GetPosition() - GetPosition();
	setPos = GetPosition() + vec;
	_surface_BOTTOM->SetPosition(setPos);

	// 左面
	vec = _surface_LEFT->GetPosition() - GetPosition();
	setPos = GetPosition() + vec;
	_surface_LEFT->SetPosition(setPos);

	// 右面
	vec = _surface_RIGHT->GetPosition() - GetPosition();
	setPos = GetPosition() + vec;
	_surface_RIGHT->SetPosition(setPos);

	// 前面
	vec = _surface_FRONT->GetPosition() - GetPosition();
	setPos = GetPosition() + vec;
	_surface_FRONT->SetPosition(setPos);

	// 背面
	vec = _surface_BACK->GetPosition() - GetPosition();
	setPos = GetPosition() + vec;
	_surface_BACK->SetPosition(setPos);

	// 座標の移動
	Object::SetPosition(position);

}

// 大きさの設定
void ObjectCube::SetSize(D3DXVECTOR3& size)
{
	// 大きさの設定
	m_halfSize = size * 0.5f;
	m_radius   = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径

	// 上面
	D3DXVECTOR3 setSize = D3DXVECTOR3(size.x, size.z, 0);
	_surface_TOP->SetSize(setSize);

	// 底面
	setSize = D3DXVECTOR3(size.x, size.z, 0);
	_surface_BOTTOM->SetSize(setSize);

	// 左面
	setSize = D3DXVECTOR3(size.z, size.y, 0);
	_surface_LEFT->SetSize(setSize);

	// 右面
	_surface_RIGHT->SetSize(setSize);

	// 前面
	setSize = D3DXVECTOR3(size.x, size.y, 0);
	_surface_FRONT->SetSize(setSize);

	// 背面
	_surface_BACK->SetSize(setSize);

}

// 回転率の設定
void ObjectCube::SetRotate(D3DXVECTOR3& rotate)
{
	//// 回転率の設定
	//m_rotate = rotate;

	//// 上面
	//_surface_TOP->SetRotate(rotate);
	//_surface_TOP->MoveRotate(D3DXVECTOR3(D3DXToRadian(90), 0, 0));

	//// 底面
	//_surface_BOTTOM->SetRotate(rotate);
	//_surface_BOTTOM->MoveRotate(D3DXVECTOR3(D3DXToRadian(-90), 0, 0));

	//// 左面
	//_surface_LEFT->SetRotate(rotate);
	//_surface_LEFT->MoveRotate(D3DXVECTOR3(0, D3DXToRadian(-90), 0));

	//// 右面
	//_surface_RIGHT->SetRotate(rotate);
	//_surface_RIGHT->MoveRotate(D3DXVECTOR3(0, D3DXToRadian(90), 0));

	//// 前面
	//_surface_FRONT->SetRotate(rotate);
	//_surface_FRONT->MoveRotate(D3DXVECTOR3(0, 0, 0));

	//// 背面
	//_surface_BACK->SetRotate(rotate);
	//_surface_FRONT->MoveRotate(D3DXVECTOR3(0, D3DXToRadian(180), 0));

}

// 拡縮率の設定
void ObjectCube::SetScale(D3DXVECTOR3& scale)
{
	// 拡縮率の設定
	m_scale.x = scale.x < 0 ? 0 : scale.x;
	m_scale.y = scale.y < 0 ? 0 : scale.y;
	m_scale.z = scale.z < 0 ? 0 : scale.z;

	_surface_TOP->SetScale(scale);		// 上面
	_surface_BOTTOM->SetScale(scale);	// 底面
	_surface_LEFT->SetScale(scale);		// 左面
	_surface_RIGHT->SetScale(scale);	// 右面
	_surface_FRONT->SetScale(scale);	// 前面
	_surface_BACK->SetScale(scale);		// 背面

}

// 色の設定
void ObjectCube::SetColor(unsigned int rgba)
{
	// 色設定
	m_color.color = rgba;

	_surface_TOP->SetColor(rgba);		// 上面
	_surface_BOTTOM->SetColor(rgba);	// 底面
	_surface_LEFT->SetColor(rgba);		// 左面
	_surface_RIGHT->SetColor(rgba);		// 右面
	_surface_FRONT->SetColor(rgba);		// 前面
	_surface_BACK->SetColor(rgba);		// 背面

}

// UV開始点の設定
void ObjectCube::SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint)
{
	// UV開始点の設定
	m_UV_StartPoint = UV_StartPoint;

	_surface_TOP->SetUv_StartPoint(UV_StartPoint);		// 上面
	_surface_BOTTOM->SetUv_StartPoint(UV_StartPoint);	// 底面
	_surface_LEFT->SetUv_StartPoint(UV_StartPoint);	// 左面
	_surface_RIGHT->SetUv_StartPoint(UV_StartPoint);	// 右面
	_surface_FRONT->SetUv_StartPoint(UV_StartPoint);	// 前面
	_surface_BACK->SetUv_StartPoint(UV_StartPoint);	// 背面

}

// UVの大きさの設定
void ObjectCube::SetUV_Size(D3DXVECTOR2& UV_Size)
{
	// UVの大きさの設定
	m_UV_StartPoint = UV_Size;

	_surface_TOP->SetUv_Size(UV_Size);		// 上面
	_surface_BOTTOM->SetUv_Size(UV_Size);	// 底面
	_surface_LEFT->SetUv_Size(UV_Size);	// 左面
	_surface_RIGHT->SetUv_Size(UV_Size);	// 右面
	_surface_FRONT->SetUv_Size(UV_Size);	// 前面
	_surface_BACK->SetUv_Size(UV_Size);	// 背面

}

// 描画処理の有無の設定
void ObjectCube::SetDraw(bool bDraw)
{
	_surface_TOP->SetIsDraw(bDraw);		// 上面
	_surface_BOTTOM->SetIsDraw(bDraw);	// 底面
	_surface_LEFT->SetIsDraw(bDraw);		// 左面
	_surface_RIGHT->SetIsDraw(bDraw);		// 右面
	_surface_FRONT->SetIsDraw(bDraw);		// 前面
	_surface_BACK->SetIsDraw(bDraw);		// 背面

}

// テクスチャの設定
void ObjectCube::SetTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;

	_surface_TOP->SetTexture(pTexture);		// 上面
	_surface_BOTTOM->SetTexture(pTexture);	// 底面
	_surface_LEFT->SetTexture(pTexture);	// 左面
	_surface_RIGHT->SetTexture(pTexture);	// 右面
	_surface_FRONT->SetTexture(pTexture);	// 前面
	_surface_BACK->SetTexture(pTexture);	// 背面

}
