//*****************************************************************************
//
//		スコア
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"


class Object2D;

class Score
{
public:
	//----- コンストラクタ / デストラクタ -----
	Score();
	~Score() {}

	//----- 基本的な関数 -----
	static Score* Create(const D3DXVECTOR3& centerPosition, const D3DXVECTOR3& sizeAll, unsigned int numDigit);
	void Init(const D3DXVECTOR3& centerPosition, const D3DXVECTOR3& sizeAll, unsigned int numDigit);
	void Uninit(void) {}
	void Update(void);
	void Draw(void);
	void Release(void) { delete this; }

	//----- 増減処理 -----
	void MoveScore(int score);	// スコアの増減

	//----- 設定処理 -----
	void SetScore(int score);							// スコアの設定
	void SetTexture(const LPDIRECT3DTEXTURE9 pTexture);	// テクスチャの設定

	//----- データを返す処理 -----
	int GetScore() { return m_numScore + (int)m_moveScore; }	// 現在のスコアを返す


private:
	//----- 定数 -----
	static const double CONFFICIENT;	// 一度に増える増加率


	//----- データ -----
	         D3DXVECTOR3  m_centerPosition;	// 数列全体の中心点
	         D3DXVECTOR3  m_sizeAll;		// 数列全体の大きさ
	         Object2D*    m_pDigit;			// 数字ポリゴン
	unsigned int          m_numDigit;		// 桁数
	         int          m_maxScore;		// スコアの最大値
	         int          m_numScore;		// スコアの表示値
	         double       m_moveScore;		// スコアの増加値

};

#endif
