//*****************************************************************************
//
//		指揮官
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _COMMANDER_H_
#define _COMMANDER_H_

#include "main.h"
#include "Object.h"

class Soldier;
class ObjectModel;


class Commander : public Object
{
public:
	Commander();
	~Commander();
	
	static Commander* CreateBuffer();
	void Init();
	void Uninit();
	void Update();
	void Draw() {}


	// 指揮官の動的生成処理
	// positon ：生成位置
	// 返り値  ：生成された指揮官のポインタ
	static Commander* SetCommander(D3DXVECTOR3& position, D3DXVECTOR3& front, Object::GROUP group);
	//static Commander* SetCommander(D3DXVECTOR3& position, D3DXVECTOR3& front, Object::GROUP group, D3DXVECTOR3 targetPosition);

	// 返り値：	true  使用中
	//			false 未使用
	bool GetInstance() { return m_bInstance; }

	// 指揮下の兵士から敵発見報告を受け取る
	void ReceiveReport_SearchEnemy() { m_bSearchEnemy = true; }

	// 指揮下の兵士から死亡報告を受け取る
	// pSoldier：死んだ兵士
	void ReceiveReport_Death(Soldier* pSoldier);

	// 自分が向いてる方向を返す
	// 返り値：向き
	D3DXVECTOR3& GetFront() { return m_front; }

	// 整列しているかどうかを返す
	// 返り値：	true  整列している
	//			false 整列していない
	bool GetFormLine() { return m_bFormLine; }

	// 指揮下の兵士から拠点発見報告を受け取る
	// pTarget：見つけた拠点
	void ReceiveReport_SearchBasePoint(ObjectModel* pTarget) { m_pSearchBasePoint = pTarget; }

	// 指揮下の兵士から拠点破壊報告を受け取る
	void ReceiveReport_BreakBasePoint();

private:
	// 指揮官の設定処理
	void SetCommander_private(D3DXVECTOR3& position, D3DXVECTOR3& front, Object::GROUP group);
	//void SetCommander_private(D3DXVECTOR3& position, D3DXVECTOR3& front, Object::GROUP group, D3DXVECTOR3 targetPosition);

	// 指揮下の兵士を生成
	void SpawnSoldier();

	// 指揮下の兵士を整列させる
	void FormLine();

	// 指揮下の兵士に弾を撃たせる
	void ShotBullet();

	// 指揮下の兵士を進ませる
	void SoldierMove();

	// 指揮下の兵士を拠点に突撃させる
	void AssaultBasePoint();

	// 指揮下の兵士の目標座標を設定する
	void SetTargetPosition();

	// 指揮下の兵士が整列しているかどうかを検査する
	// 返り値：	true  整列している
	//			false 整列していない
	bool CheckFormLine_Mine();

	// 前方の兵士列が整列しているかどうかを検査する
	// 返り値：	true  整列している
	//			false 整列していない、存在しない
	bool CheckFormLine_Others();



	Soldier* m_pSoldier;				// 指揮下の兵士の先頭アドレス
	D3DXVECTOR3 m_front;				// 前方向
	bool m_bSearchEnemy;				// 敵発見フラグ
	int  m_restTime;					// 次の射撃までの休憩時間
	bool m_bInstance;					// 使用中フラグ
	float m_radius_ViewAngle;			// 視野角の半径
	bool  m_bFormLine;					// true で整列している
	float m_velocity;					// 移動速度
	ObjectModel*  m_pSearchBasePoint;	// 拠点の発見フラグ	



};


#endif

