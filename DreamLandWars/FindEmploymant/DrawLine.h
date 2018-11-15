//*****************************************************************************
//
//		描画線
//												Autohr	:	Yusuke Seki
//*****************************************************************************
#ifndef _DRAWLINE_H_
#define _DRAWLINE_H_

#include "main.h"
#include "Object3D.h"

class Player;


class DrawLine : public Object3D
{
public:
	//----- 列挙型の定義 -----
	enum TYPE{	// 点の種類
		TYPE_START,		// 始点
		TYPE_VIA,		// 経由点
	};


public:
	//----- コンストラクタ / デストラクタ -----
	DrawLine() {}
	DrawLine(Object::TYPE type);
	virtual ~DrawLine();


	//----- 基本的な関数 -----
	static DrawLine* Create(D3DXVECTOR3& position);
	void Init(D3DXVECTOR3& position);
	void Uninit(void);
	void Update(void);
	void Draw(void);


	//----- 設定処理 -----
	static DrawLine* SetStartLine(D3DXVECTOR3& position, Player& player);		// 始点の設定
	static void SetViaPoint(D3DXVECTOR3& position, DrawLine* pStartPoint);		// 経由点の設定
	static void SetDrawShot(DrawLine* pStartPoint);								// DSの発射
	static void ClearParameter_List(DrawLine* pStartPoint);						// データのクリア：繋がってる領域全て
	       void ClearParameter();												// データのクリア：個別


	//-----　データを返す処理 -----
	DrawLine::TYPE GetType_DrawLine()        { return m_type; }			// 種類を返す
	DrawLine*      GetNextPointer_DrawLine() { return m_pNext; }		// 次の点を返す
	bool           GetInstance()             { return m_bInstance; }	// 使用中かどうかを返す：true で使用中
	Player*        GetPlayer_DrawLine()      { return m_pPlayer; }		// 親プレイヤーを返す

	D3DXVECTOR3 GetTailPosition() { return m_pTail->GetPosition(); }

private:
	//----- 設定処理 -----
	void SetStartLine_private(D3DXVECTOR3& position, Player& player);			// 始点の設定
	void SetViaPoint_private(D3DXVECTOR3& position, DrawLine* pStartPoint);		// 経由点の設定

	//----- データ -----
	static const float DRAWLINE_HEIGHT;	// 描画線ポリゴンの高さ

	DrawLine::TYPE m_type;		// 種類
	DrawLine*      m_pNext;		// 次の点
	DrawLine*      m_pTail;		// 末尾の点
	bool           m_bInstance;	// 使用中フラグ
	Player*        m_pPlayer;	// 親プレイヤー

};

#endif
