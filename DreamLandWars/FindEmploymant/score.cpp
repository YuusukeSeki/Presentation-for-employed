//*****************************************************************************
//
//		スコア
//													Author : Yusuke Seki
//*****************************************************************************
#include "score.h"
#include "renderer.h"
#include "Object2D.h"
#include "texture.h"


//----- 静的変数の初期化 ------
const double Score::CONFFICIENT = 0.5; // 一度に増える増加率



//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Score::Score()
{
	m_centerPosition = D3DXVECTOR3(0, 0, 0);
	m_sizeAll        = D3DXVECTOR3(0, 0, 0);
	m_pDigit         = nullptr;
	m_numDigit       = 0;
	m_maxScore       = 0;
	m_numScore       = 0;
	m_moveScore      = 0;

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Score* Score::Create(const D3DXVECTOR3& posLeftEdge, const D3DXVECTOR3& sizeAll, unsigned int numDigit)
{
	// ※エラーチェック：桁数が０以下なら生成せずに終了する
	if (numDigit <= 0) {
		_MSGERROR("Failed Create Score!!\nCannot Read Digit!!", "Score* Score::Create(const D3DXVECTOR3& posLeftEdge, const D3DXVECTOR3& sizeAll, unsigned int numDigit)");
		return nullptr;
	}

	// 実体の生成
	Score* pScore = new Score;

	// 実体の初期化
	pScore->Init(posLeftEdge, sizeAll, numDigit);

	// 生成されたタイマーのポインタを返す
	return pScore;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Score::Init(const D3DXVECTOR3& centerPosition, const D3DXVECTOR3& sizeAll, unsigned int numDigit)
{
	// 数列全体の中心点の取得
	m_centerPosition = centerPosition;

	// 数列全体の大きさの取得
	m_sizeAll = sizeAll;

	// 桁数の取得
	m_numDigit = numDigit;


	// 桁数分のポリゴンを生成
	m_pDigit = Object2D::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
	Object2D* pDigit = m_pDigit;
	for (int i = 1; i < (int)m_numDigit; i++) {
		Object2D::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
	}



	// 生成した数字ポリゴンのパラメータを設定する
	{
		// それぞれの数字ポリゴンの大きさと中心点を算出
		float       rightEdge = m_centerPosition.x + m_sizeAll.x * 0.5f;						// 数列の右端の座標
		D3DXVECTOR3 size      = D3DXVECTOR3(m_sizeAll.x / m_numDigit, m_sizeAll.y, 0);			// サイズ
		D3DXVECTOR3 cPos      = D3DXVECTOR3(rightEdge - size.x * 0.5f , m_centerPosition.y, 0);	// 中心点

		// 設定する
		for (int i = 0; i < (int)m_numDigit; i++) {
			pDigit->Init(cPos, size);					// 中心点とサイズ
			pDigit->SetUv_Size(D3DXVECTOR2(0.1f, 1.f));	// UVサイズ

			// 次のポリゴンの中心点算出
			cPos.x -= size.x;

			// 次のポリゴンへ
			pDigit = (Object2D*)pDigit->GetNextPointer();
		}

	}


	// 表示値と増減値の初期化
	m_numScore  = 0;
	m_moveScore = 0;

	// スコアの上限を算出
	int digit  = 9;
	m_maxScore = 0;
	for (int i = 0; i < (int)m_numDigit; i++) {
		m_maxScore += digit;
		digit *= 10;
	}

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Score::Update(void)
{
	// 増加値が０～１？
	if (m_moveScore * CONFFICIENT > 0 && m_moveScore * CONFFICIENT < 1.f) {
		m_numScore += 1;
		m_moveScore = 0;
	}
	// 減少値が０～１？
	else if (m_moveScore * CONFFICIENT < 0 && m_moveScore * CONFFICIENT > -1.f){
		m_numScore -= 1;
		m_moveScore = 0;
	}
	// 増減値を足す
	else {
		m_numScore  += (int)(m_moveScore * CONFFICIENT);
		m_moveScore -= (int)(m_moveScore * CONFFICIENT);
	}



	//----- UV値の設定 -----
	int scoreAll, number;
	scoreAll = m_numScore;

	// 桁数分回す
	Object2D* pDigit = m_pDigit;
	for (int i = 0; i < (int)m_numDigit; i++) {
		// 数値の算出
		number = scoreAll % 10;

		// 数値をUV値に反映
		pDigit->SetUv_StartPoint(D3DXVECTOR2(number * 0.1f, 0.f));

		// 桁を一桁下げる
		scoreAll /= 10;

		// 次の数字ポリゴンへ
		pDigit = (Object2D*)pDigit->GetNextPointer();
	}
	// End...UV値の設定

}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Score::Draw(void)
{
	// 描画
	Object2D* pDigit = m_pDigit;
	for (int i = 0; i < (int)m_numDigit; i++) {
		pDigit->Draw();
		pDigit = (Object2D*)pDigit->GetNextPointer();
	}
}



//=============================================================================
// 増減
// 得点
void Score::MoveScore(int score)
{
	// まずはそのまま足す
	m_moveScore += score;


	// スコアが上限を超えた？
	if (m_numScore + m_moveScore >= m_maxScore) { // 超えた
		// 増加値を上限一杯までにする
		m_moveScore = m_maxScore - m_numScore;

	}
	// スコアが下限を超えた？
	else if(m_numScore + m_moveScore < 0){ // 超えた
		// 減少値を下限一杯までにする
		m_moveScore = 0 - m_numScore;

	}

}


//=============================================================================
// 設定
// 得点
void Score::SetScore(int score)
{
	// まずはそのまま設定する
	m_moveScore = score - m_numScore;


	// スコアが上限を超えた？
	if (m_numScore + m_moveScore >= m_maxScore) { // 超えた
		// 増加値を上限一杯までにする
		m_moveScore = m_maxScore - m_numScore;

	}
	// スコアが下限を超えた？
	else if (m_numScore + m_moveScore < 0) { // 超えた
		// 減少値を下限一杯までにする
		m_moveScore = 0 - m_numScore;

	}

}


// テクスチャ
void Score::SetTexture(const LPDIRECT3DTEXTURE9 pTexture) 
{
	// テクスチャの設定
	Object2D* pDigit = m_pDigit;
	for (int i = 0; i < (int)m_numDigit; i++) {
		pDigit->SetTexture(pTexture);
		pDigit = (Object2D*)pDigit->GetNextPointer();
	}

}

