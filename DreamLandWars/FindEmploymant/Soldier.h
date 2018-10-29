//*****************************************************************************
//
//		兵士
//												Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _SOLDIER_H_
#define _SOLDIER_H_

#include "main.h"
#include "ObjectModel.h"

class Commander;
class Tower;
class Castle;
class Player;


class Soldier : public ObjectModel
{
public:	//----- 列挙型の定義 -----
	enum POSTURE {	// 体勢
		POSTURE_BONE,		// 生まれてすぐ（無敵）
		POSTURE_STAND,		// 立ち止まる
		POSTURE_RUN,		// 走る
		POSTURE_BACKWORD,	// のけ反る
		POSTURE_TUMBLE,		// 倒れる
		POSTURE_RISE,		// 起き上がる
		POSTURE_DEATH,		// 死ぬ
		POSTURE_BULLETSHOT,	// 射撃攻撃
		POSTURE_ASSAULT,	// 突撃（塔、城へ）
	};

public:	// 生成処理・兵士列設定、取得処理
	// 兵士の動的生成処理
	static Soldier* SetSoldier(D3DXVECTOR3& position, D3DXVECTOR3& front, const char* modelPass, Object::GROUP group, Commander* pCommnader);

	// 次の兵士を設定する
	void SetNextSoldier(Soldier* pSoldier) { m_pNextSoldier = pSoldier; }

	// 前の兵士を設定する
	void SetPrevSoldier(Soldier* pSoldier) { m_pPrevSoldier = pSoldier; }

	// 次の兵士を取得する
	Soldier* GetNextSoldier() { return m_pNextSoldier; }

	// 前の兵士を取得する
	Soldier* GetPrevSoldier() { return m_pPrevSoldier; }


public:
	Soldier();
	~Soldier();
	static Soldier* Create(D3DXVECTOR3& position, const char* modelPass);
	void Init(D3DXVECTOR3& position, const char* modelPass);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 走る
	// vecZ：進む方向
	void Run(D3DXVECTOR3& vecZ, float velocity);

	// 使用中フラグを返す
	// 返り値： true  - 使用中
	//			false - 未使用
	bool GetInstance() { return m_bInstance; }

	// 攻撃される
	void Attack(float damage, D3DXVECTOR3& vector, float accelerate, Player* pPlayer);

	// ダメージを受ける
	// damage：与えるダメージ数
	// 返り値： true  死んだよ
	//			false 生きてるよ
	bool Damage(float damage);

	// 回復する
	// heal：与える回復数
	void Heal(float heal) { m_life = m_life + heal > m_maxLife ? m_maxLife : m_life + heal; }

	// 目標拠点を設定する
	void SetTargetBasePoint(ObjectModel* pTarget);

	// 敵に攻撃をする
	void ShotBullet();

	// この兵士を殺す
	void KillMe();

	// IDを取得
	// 返り値：兵士固有のID
	int GetID() { return m_ID; }


private:
	// 兵士の設定処理
	void SetSoldier_private(D3DXVECTOR3& position, D3DXVECTOR3& front, Object::GROUP group, Commander* pCommnader);

	// 索敵（塔）
	// 返り値： Towerのポインタ  発見
	//			nullptr          いないよ
	Tower* SearchTower();

	// 索敵（城）
	// 返り値： Castleのポインタ  発見
	//			nullptr          いないよ
	Castle* SearchCastle();

	// 索敵（ユニット）
	// 返り値： true  発見
	//			false いないよ
	bool SearchEnemy();

	// 索敵（兵士）
	// 返り値： true  発見
	//			false いないよ
	bool SearchEnemy_Soldier();

	// 索敵（キャスト）
	// 返り値： true  発見
	//			false いないよ
	bool SearchEnemy_Cast();

	// 一番近い敵を探す
	// 返り値：	一番近い敵のポインタ
	ObjectModel* SearchNearEnemy();

	// 拠点との当たり判定
	bool CollisionBasePoint();

	// 拠点を殴る
	void BreakBasePoint();


	//----- データ -----
	Commander* m_pParent;	// 直属の指揮官

	Soldier* m_pNextSoldier;	// 次の兵士
	Soldier* m_pPrevSoldier;	// 前の兵士

	bool m_bInstance;	// 使用中フラグ

	float m_searchRange;	// 索敵範囲

	float m_maxLife;	// 体力の最大値
	float m_life;		// 現在の体力

	float m_breakPower;					// 拠点破壊力
	ObjectModel* m_pAssaultBasePoint;	// 発見した拠点

	static int m_cntID;	// 一意のID割り振り
	int m_ID;			// 一意のID割り振り

	

};


#endif
