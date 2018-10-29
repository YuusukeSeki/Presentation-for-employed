//*****************************************************************************
//
//		スカイドーム
//													Autohr : Yusuke Seki
//*****************************************************************************
#include "SkyDome.h"


// コンストラクタ
SkyDome::SkyDome() : ObjectModel(Object::TYPE::TYPE_MODEL_SKYDOME)
{

}

// コンストラクタ
SkyDome::SkyDome(Object::TYPE type) : ObjectModel(type)
{

}

// デストラクタ
SkyDome::~SkyDome()
{
	Uninit();

}


// 生成処理
SkyDome* SkyDome::Create(D3DXVECTOR3& position, const char* FileName)
{
	// メモリの確保
	SkyDome* pSkyDome = new SkyDome;

	// 初期化処理
	pSkyDome->Init(position, FileName);

	// 生成したスカイドームを返す
	return pSkyDome;

}

// 初期化処理
void SkyDome::Init(D3DXVECTOR3& position, const char* FileName)
{
	// 継承データの初期化処理
	ObjectModel::Init(position, FileName);

}

// 終了処理
void SkyDome::Uninit(void)
{
	// 継承データの終了処理
	ObjectModel::Uninit();
	
}

// 更新処理
void SkyDome::Update(void)
{
}

// 描画処理
void SkyDome::Draw(void)
{
	// 描画
	ObjectModel::Draw();

}

