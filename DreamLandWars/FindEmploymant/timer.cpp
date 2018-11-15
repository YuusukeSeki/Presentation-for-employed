//*****************************************************************************
//
//		タイマー
//													Author : Yusuke Seki
//*****************************************************************************
#include "timer.h"
#include "renderer.h"
#include "Object2D.h"
#include "texture.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Timer::Timer()
{
	m_pMinute = m_pColon = m_pSecondLeft  = m_pSecondRight = nullptr;
	m_cntFrame   = m_numMinutes = m_numSeconds = 0;
	m_bStop = true;

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Timer::~Timer()
{
}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Timer* Timer::Create(const D3DXVECTOR3& posLeftEdge, const D3DXVECTOR3& sizeAll)
{
	// 実体の生成
	Timer* pTimer = new Timer;

	// 実体の初期化
	pTimer->Init(posLeftEdge, sizeAll);

	// 生成されたタイマーのポインタを返す
	return pTimer;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Timer::Init(const D3DXVECTOR3& posLeftEdge, const D3DXVECTOR3& sizeAll)
{
	D3DXVECTOR3 posBuf1, posBuf2, posBuf3, posBuf4;

	// 座標を算出
	posBuf1 = D3DXVECTOR3(posLeftEdge.x + sizeAll.x * 0.15f, posLeftEdge.y, 0.f);
	posBuf2 = D3DXVECTOR3(posLeftEdge.x + sizeAll.x * 0.35f, posLeftEdge.y, 0.f);
	posBuf3 = D3DXVECTOR3(posLeftEdge.x + sizeAll.x * 0.55f, posLeftEdge.y, 0.f);
	posBuf4 = D3DXVECTOR3(posLeftEdge.x + sizeAll.x * 0.85f, posLeftEdge.y, 0.f);

	// 実体を生成
	m_pMinute      = Object2D::Create(posBuf1, D3DXVECTOR3(sizeAll.x * 0.32f, sizeAll.y, 0.f));
	m_pColon       = Object2D::Create(posBuf2, D3DXVECTOR3(sizeAll.x * 0.04f, sizeAll.y, 0.f));
	m_pSecondLeft  = Object2D::Create(posBuf3, D3DXVECTOR3(sizeAll.x * 0.32f, sizeAll.y, 0.f));
	m_pSecondRight = Object2D::Create(posBuf4, D3DXVECTOR3(sizeAll.x * 0.32f, sizeAll.y, 0.f));

	// UV値の設定
	m_pMinute     ->SetUv_Size(D3DXVECTOR2(0.1f, 1.f));
	m_pColon      ->SetUv_Size(D3DXVECTOR2(1.0f, 1.f));
	m_pSecondLeft ->SetUv_Size(D3DXVECTOR2(0.1f, 1.f));
	m_pSecondRight->SetUv_Size(D3DXVECTOR2(0.1f, 1.f));

	
	// 残り時間とフレームカウンター初期化
	m_cntFrame = m_numMinutes = m_numSeconds = 0;

	
	// タイマー処理一時停止
	m_bStop = true;
}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Timer::Uninit(void)
{
}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Timer::Update(void)
{
	// タイマー止まってる？
	if (m_bStop) { // 止まってる
		// タイマーを動かさないで終了
		return;
	}


	// カウンターを回す
	m_cntFrame++;

	// カウンターは60回回った？
	if (m_cntFrame == 60) { // 回った
		// カウンターを0に
		m_cntFrame = 0;

		// 秒数を減らす
		m_numSeconds--;

		// 秒数が負の数になっている？
		if (m_numSeconds < 0) { // なっている
			// 分数を減らす
			m_numMinutes--;

			// 秒数を59秒に
			m_numSeconds = 59;
		}
	}


	// 分と秒の2桁目と1桁目をそれぞれ切り取る
	int minute, second1, second2;
	minute  = m_numMinutes % 10;
	second1 = m_numSeconds / 10;
	second2 = m_numSeconds % 10;

	// UV値の設定
	m_pMinute     ->SetUv_StartPoint(D3DXVECTOR2(minute  * 0.1f, 0.f));
	m_pSecondLeft ->SetUv_StartPoint(D3DXVECTOR2(second1 * 0.1f, 0.f));
	m_pSecondRight->SetUv_StartPoint(D3DXVECTOR2(second2 * 0.1f, 0.f));


	// 残り時間は残ってる？
	if (m_numMinutes == 0 && m_numSeconds == 0) { // 残ってない
		// これ以上タイマーを進めない
		m_bStop = true;
	}


}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Timer::Draw(void)
{
	// 分
	m_pMinute->Draw();

	// ：
	m_pColon->Draw();

	// 秒
	m_pSecondLeft->Draw();
	m_pSecondRight->Draw();

}


//-----------------------------------------------------------------------------
//	trueで残り時間が０
//-----------------------------------------------------------------------------
bool Timer::TimeUp()
{
	// 残り時間は残ってる？
	if (m_numMinutes == 0 && m_numSeconds == 0) { // 残ってない
		// 時間切れです
		return true;
	}

	// まだ時間は残ってます
	return false;
}


//-----------------------------------------------------------------------------
//	タイマーを進める
//-----------------------------------------------------------------------------
void Timer::StartTimer()
{
	// 残り時間は残ってる？
	if (m_numMinutes == 0 && m_numSeconds == 0) { // 残ってない
												  // タイマーを進めずに返す
		return;
	}

	// タイマー動く
	m_bStop = false;
}


//-----------------------------------------------------------------------------
//	タイマーを止める
//-----------------------------------------------------------------------------
void Timer::StopTimer()
{
	// タイマー止まる
	m_bStop = true;
}


//-----------------------------------------------------------------------------
// タイマーを進める / 止める
//-----------------------------------------------------------------------------
void Timer::SwitchTimer()
{
	// 止まってたら動かす
	if (m_bStop) {
		// 残り時間は残ってる？
		if (m_numMinutes == 0 && m_numSeconds == 0) { // 残ってない
													  // タイマーを進めずに返す
			return;
		}

		// タイマー動く
		m_bStop = false;

	}
	// 動いてたら止める
	else {
		// タイマー止まる
		m_bStop = true;
	}
}



//=============================================================================
// 設定
// 時間
void Timer::SetTimer(unsigned int minutes, unsigned int seconds)
{
	int minute, second1, second2;


	// 秒数を設定
	m_numSeconds = seconds % 60;

	// 分数を秒数の余りだけ増やす
	minutes += seconds / 60;

	// 分数は上限を超えてる？
	if (minutes >= 10) {
		// 上限に設定する
		minutes = 9;
	}

	// 分数の設定
	m_numMinutes = minutes;


	// 分と秒の2桁目と1桁目をそれぞれ切り取る
	minute  = m_numMinutes % 10;
	second1 = m_numSeconds / 10;
	second2 = m_numSeconds % 10;

	// UV値の設定
	m_pMinute->SetUv_StartPoint(D3DXVECTOR2(minute  * 0.1f, 0.f));
	m_pSecondLeft->SetUv_StartPoint(D3DXVECTOR2(second1 * 0.1f, 0.f));
	m_pSecondRight->SetUv_StartPoint(D3DXVECTOR2(second2 * 0.1f, 0.f));
}


// 数字のテクスチャ
void Timer::SetTexture_Number(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pMinute->SetTexture(pTexture);
	m_pSecondLeft->SetTexture(pTexture);
	m_pSecondRight->SetTexture(pTexture);
	
}


// コロンのテクスチャ
void Timer::SetTexture_Colon(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pColon->SetTexture(pTexture);

}


