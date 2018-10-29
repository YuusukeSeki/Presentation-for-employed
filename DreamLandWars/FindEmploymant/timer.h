//*****************************************************************************
//
//		タイマー
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"


class Object2D;

class Timer
{
public:
	// コンストラクタ / デストラクタ
	Timer();
	~Timer();

	// 関数
	static Timer* Create(const D3DXVECTOR3& posLeftEdge, const D3DXVECTOR3& size);
	void Init(const D3DXVECTOR3& posLeftEdge, const D3DXVECTOR3& sizeAll);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Release() { delete this; }

	// フラグ取得
	bool TimeUp();

	// 設定
	void SetTimer(unsigned int minutes, unsigned int seconds);
	void StartTimer();	// タイマーを動かす
	void StopTimer();	// タイマーを止める
	void SwitchTimer(); // タイマーを動かす / 止める
	void SetTexture_Number(const LPDIRECT3DTEXTURE9 pTexture);
	void SetTexture_Colon(const LPDIRECT3DTEXTURE9 pTexture);




protected:


private:
	Object2D* m_pMinute;
	Object2D* m_pColon;
	Object2D* m_pSecondLeft;
	Object2D* m_pSecondRight;

	unsigned int m_cntFrame;

	int m_numMinutes;
	int m_numSeconds;

	bool m_bStop;

};

#endif
