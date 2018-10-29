//*****************************************************************************
//	
//		エフェクト基底クラス（リスト構造）
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_

#include "main.h"


class EffectManager
{
public:
	//----- 列挙型の定義 -----
	enum TYPE { // 種類（描画順序込み）
		TYPE_3D,			// ★3Dポリゴン
		TYPE_3D_BILLBOARD,	// ビルボード
		TYPE_2D,			// ★2Dポリゴン

		TYPE_MAX,	// ※ 種類の最大数
	};


public:
	//----- コンストラクタ / デストラクタ -----
	EffectManager() {}
	EffectManager(EffectManager::TYPE type);
	virtual ~EffectManager() {}


	//----- 基本的な関数 -----
	virtual void Init(void) {}
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	        void Release(void);
	static  void UpdateAll();
	static  void DrawAll();
	static  void ReleaseAll();


	//----- 増減処理 -----
	virtual void MovePosition(D3DXVECTOR3& movePosition) { m_position += movePosition; }	// 座標を移動する


	//----- 設定処理 -----
	virtual void SetPosition(D3DXVECTOR3& position) { m_position = position; }	// 座標を設定する
	        void SetNextPointer(EffectManager* pNext) { m_pNext = pNext; }		// 次のポインターを設定する
	        void SetPrevPointer(EffectManager* pPrev) { m_pPrev = pPrev; }		// 前のポインターを設定する


	//----- データを返す処理 -----
	static EffectManager* GetLDATA_HEAD(EffectManager::TYPE type) { return m_pHEAD[type]; }	// 実体を返す
	       EffectManager* GetNextPointer()                        { return m_pNext; }		// 自分の次のポインタを返す
	       EffectManager* GetPrevPointer()                        { return m_pPrev; }		// 自分の前のポインタを返す
	       D3DXVECTOR3*   GetPosition()                           { return &m_position; }	// 座標を返す


protected:


private:
	//----- データ -----
	D3DXVECTOR3 m_position;		// 座標
	EffectManager::TYPE m_type;	// 種類

	//----- 管理用データ -----
	static EffectManager* m_pHEAD[EffectManager::TYPE_MAX];	// リストデータの先頭
	static EffectManager* m_pTAIL[EffectManager::TYPE_MAX];	// リストデータの最後
	       EffectManager* m_pNext;							// 次のオブジェクト
	       EffectManager* m_pPrev;							// 前のオブジェクト

};

#endif

