//*****************************************************************************
//	
//		オブジェクト基底クラス（リスト構造）
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _OBJECT_LIST_H_
#define _OBJECT_LIST_H_

#include "main.h"

#define _WLW		// ワンダー用のフレームワークに変更（コメントアウトで無し）



class Object
{
public:
	//----- 列挙型の定義 -----
	enum TYPE { // 種類（描画順序込み）
		TYPE_COMMANDER,					// 指揮官（座標のみ）
		TYPE_MODEL,						// ★モデル
		TYPE_MODEL_PLAYER,				// プレイヤー
		TYPE_MODEL_SOLDIER,				// 兵士
		TYPE_MODEL_CASTLE,				// 城
		TYPE_MODEL_TOWER,				// 塔
		TYPE_MODEL_SKYDOME,				// スカイドーム
		TYPE_3D_FIELD,					// フィールド
		TYPE_3D_SKYBOX,					// スカイボックス
		TYPE_3D,						// ★3Dポリゴン
		TYPE_3D_CUBE,					// キューブ
		TYPE_3D_WALL,					// 壁
		TYPE_3D_DRAWRANGE,				// DS射程範囲
		TYPE_3D_DRAWLINE,				// 描画線
		TYPE_3D_SOLDIERBULLET,			// 兵士弾
		TYPE_3D_STRAIGHTSHOT,			// ストレートショット
		TYPE_3D_DRAWSHOT,				// ドローショット
		TYPE_3D_BILLBOARD_LIFEGAUGE,	// 体力ゲージ
		TYPE_3D_BILLBOARD_ICON,			// アイコン
		TYPE_3D_BILLBOARD_HOLD,			// "HOLD"
		TYPE_3D_BILLBOARD,				// ビルボード
		TYPE_3D_BULLET,					// バレット（3D）
		TYPE_3D_BILLBOARD_BULLET,		// バレット（ビルボード）
		TYPE_3D_BILLBOARD_EFFECT,		// エフェクト（ビルボード）
		TYPE_3D_GAMETITLE,				// ゲームタイトル
		TYPE_3D_TOUCHSCREEN,			// タッチスクリーン
		TYPE_2D,						// ★2Dポリゴン
		TYPE_2D_FRAMEBORDER,			// 枠線

		TYPE_MAX,	// ※ 種類の最大数
	};


public:
	//----- コンストラクタ / デストラクタ -----
	Object(){}
	Object(Object::TYPE type);
	virtual ~Object(){}

	
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
	        void SetNextPointer(Object* pNext)		{ m_pNext = pNext; }		// 次のポインターを設定する
	        void SetPrevPointer(Object* pPrev)		{ m_pPrev = pPrev; }		// 前のポインターを設定する


	//----- データを返す処理 -----
	static Object*      GetLDATA_HEAD(Object::TYPE type) { return m_pHEAD[type]; }	// 実体を返す
	       Object*      GetNextPointer()                 { return m_pNext; }		// 自分の次のポインタを返す
	       Object*      GetPrevPointer()                 { return m_pPrev; }		// 自分の前のポインタを返す
	       D3DXVECTOR3& GetPosition()                    { return m_position; }		// 座標を返す
		   Object::TYPE GetType()						 { return m_type; }			// 種類を返す

private:
	//----- データ -----
	D3DXVECTOR3 m_position;		// 座標
	Object::TYPE m_type;		// 種類

	//----- 管理用データ -----
	static Object* m_pHEAD[Object::TYPE_MAX];	// リストデータの先頭
	static Object* m_pTAIL[Object::TYPE_MAX];	// リストデータの最後
	       Object* m_pNext;						// 次のオブジェクト
	       Object* m_pPrev;						// 前のオブジェクト


#ifdef _WLW
public:
	//----- 列挙型の定義 -----
	enum GROUP { // 陣営
		GROUP_NONE, // 無所属
		GROUP_A,	// A
		GROUP_B,	// B
	};

	//----- 設定処理 -----
	void SetGroup(Object::GROUP group) { m_group = group; }	// WLW陣営設定

	//----- データを返す処理 -----
	Object::GROUP GetGroup() { return m_group; }	// 所属陣営を返す


private:
	//----- データ -----
	Object::GROUP m_group;	// 陣営

#endif

};

#endif

