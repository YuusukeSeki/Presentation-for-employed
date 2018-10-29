//*****************************************************************************
//
//		塔
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _TOWER_H_
#define _TOWER_H_

#include "main.h"
#include "ObjectModel.h"

class Hold;
class LifeGauge;
class Icon;


class Tower : public ObjectModel
{
public:
	//----- コンストラクタ / デストラクタ -----
	Tower();
	Tower(Object::TYPE type);
	virtual ~Tower();


	//----- 基本的な関数 -----
	static Tower* Create(D3DXVECTOR3& position, const char* FileName, Object::GROUP group);
	void Init(D3DXVECTOR3& position, const char* FileName, Object::GROUP group);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 拠点にダメージを与える
	// breakPower : 与えるダメージ
	// 【返り値】	true  : 拠点が壊れた
	//				false : 拠点が壊れてない
	bool BrowTower(float breakPower);

	// 塔を殴れる範囲との当たり判定
	// position : 対象キャストの位置
	// 【返り値】	true  : 殴れる
	//				false : 殴れない
	bool CollisionBrowRange(D3DXVECTOR3& position);

	// 体力を返す
	// 【返り値】現在の体力
	float GetLife() { return m_life; }

	// "Hold"の実体を返す
	// 【返り値】"Hold"の実体
	Hold* GetHold() { return m_pHold; }

	// 移動処理
	void MovePosition(D3DXVECTOR3& movePosition);


private:
	//----- データ -----
	float m_life;		// 現在の体力
	float m_browRange;	// 殴れる範囲

	Hold*      m_pHold;			// "HOLD"
	LifeGauge* m_pLifeGauge;	// 体力ゲージ
	Icon*      m_pIcon;			// アイコン
	
	int m_cntFrame;	// 兵士生成用カウンター


};

#endif
