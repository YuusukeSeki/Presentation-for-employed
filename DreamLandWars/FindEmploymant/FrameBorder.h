//*****************************************************************************
//
//		枠線
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _FRAMEBORDER_H_
#define _FRAMEBORDER_H_

#include "main.h"
#include "Object2D.h"

class Timer;
class TeamGaugeManager;
class TeamLevel;


class FrameBorder : public Object2D
{
public:
	//----- コンストラクタ / デストラクタ -----
	FrameBorder() {}
	FrameBorder(Object::TYPE type);
	~FrameBorder();

	//----- 基本的な関数 -----
	static FrameBorder* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Release(void) { delete this; }

	//----- 増減処理 -----
	//void MovePosition(D3DXVECTOR3& movePosition);			// 座標の移動
	//void MoveSize(D3DXVECTOR3& moveSize);					// 大きさの増減
	//void MoveRotate(float moveRotate);						// 回転率の増減
	//void MoveColor(int r, int g, int b, int a);				// 色の増減
	//void MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint);	// UV開始点の増減
	//void MoveUV_Size(D3DXVECTOR2& moveUV_Size);				// UV大きさの増減

	////----- 設定処理 -----
	//void SetPosition(D3DXVECTOR3& position);			// 座標の設定
	//void SetSize(D3DXVECTOR3& size);					// 大きさの設定
	//void SetRotate(float rotate);						// 回転率の設定
	//void SetColor(unsigned int rgba);					// 色の設定
	//void SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint);	// UV開始点の設定
	//void SetUV_Size(D3DXVECTOR2& UV_Size);				// UV大きさの設定

	//----- データを返す処理 -----
	Timer*            GetTimer()            { return m_pTimer; }			// "子"のタイマーを返す
	TeamGaugeManager* GetTeamGaugeManager() { return m_pTeamGaugeManager; }	// "子"のチームゲージマネージャーを返す
	TeamLevel*        GetTeamLevel()        { return m_pTeamLevel; }		// "子"のチームレベルを返す

private:
	//----- データ -----
	Timer*            m_pTimer;				// 【子】タイマー
	TeamGaugeManager* m_pTeamGaugeManager;	// 【子】チームゲージ
	TeamLevel*        m_pTeamLevel;			// 【子】チームレベル


};

#endif
