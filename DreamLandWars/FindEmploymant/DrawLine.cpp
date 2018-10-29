//*****************************************************************************
//
//		描画線
//												Autohr	:	Yusuke Seki
//*****************************************************************************
#include "DrawLine.h"
#include "renderer.h"
#include "player.h"
#include "DrawShot.h"


//----- 静的なデータの初期化 -----
const float DrawLine::DRAWLINE_HEIGHT = 50.f;	// 描画線ポリゴンの高さ


//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
DrawLine::DrawLine(Object::TYPE type) :Object3D(type)
{
	// データのクリア
	m_type      = DrawLine::TYPE::TYPE_START;	// 種類
	m_pNext     = nullptr;						// 次の点
	m_pTail     = nullptr;						// 末尾の点
	m_pPlayer   = nullptr;						// 親プレイヤー
	m_bInstance = false;						// 使用中フラグ

}


//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
DrawLine::~DrawLine()
{
	Uninit();
}


//-----------------------------------------------------------------------------
//	生成
//-----------------------------------------------------------------------------
DrawLine* DrawLine::Create(D3DXVECTOR3& position)
{
	DrawLine* pDrawLine = new DrawLine(Object::TYPE::TYPE_3D_DRAWLINE);
	pDrawLine->Init(position);

	return pDrawLine;
}


//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
void DrawLine::Init(D3DXVECTOR3& position)
{
	// 継承データの初期化
	Object3D::Init(position, D3DXVECTOR3(5, 0, 5));
	
	// データの初期化
	m_type      = DrawLine::TYPE::TYPE_START;	// 種類
	m_pNext     = nullptr;						// 次の点
	m_pTail     = nullptr;						// 末尾の点
	m_pPlayer   = nullptr;						// 親プレイヤー
	m_bInstance = false;						// 使用中フラグ

}


//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void DrawLine::Uninit(void)
{
	// 継承データの終了処理
	Object3D::Uninit();

}


//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
void DrawLine::Update(void)
{
	// 未使用で処理無し
	if (!m_bInstance) return;

	// 徐々に透明になる
	//SetColor((255 / ELASE_FRAME) * m_life);
	//m_life = m_life - 1 < 0 ? 0 : m_life - 1;

}


//-----------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------
void DrawLine::Draw(void)
{
	// 未使用で処理無し
	if (!m_bInstance) return;

	// 描画処理
	Object3D::Draw();
	
}

//=============================================================================
// 設定
// 始点の設定
DrawLine* DrawLine::SetStartLine(D3DXVECTOR3& position, Player& player)
{
	// 実体持ちを探す
	DrawLine* pDrawLine = (DrawLine*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_DRAWLINE);

	// １つも作られていなかったら生成する
	if (pDrawLine == nullptr) {
		// 生成処理
		pDrawLine = DrawLine::Create(D3DXVECTOR3(0, 0, 0));

		// 設定処理
		pDrawLine->SetStartLine_private(position, player);

		return pDrawLine;
	}

	// 作られていたら未使用領域を探す
	DrawLine* pCurrent = (DrawLine*)pDrawLine;
	DrawLine* pNext    = (DrawLine*)pDrawLine->GetNextPointer();
	for (;;) {
		// 未使用なら設定して終了
		if (!pCurrent->GetInstance()) {
			pCurrent->SetStartLine_private(position, player);
			return pCurrent;
		}

		// 未使用領域が見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = DrawLine::Create(D3DXVECTOR3(0, 0, 0));

			// 設定処理
			pNext->SetStartLine_private(position, player);

			return pNext;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext = (DrawLine*)pCurrent->GetNextPointer();

	}

}

// 経由点の設定
void DrawLine::SetViaPoint(D3DXVECTOR3& position, DrawLine* pStartPoint)
{
	// 始点のNULLチェック
	if (pStartPoint == nullptr) {
		// エラーメッセージ
		_MSGERROR("pStartPoint cannnot be nullptr!!", "void DrawLine::SetViaPoint(D3DXVECTOR3& position, DrawLine* pStartPoint)");

	}


	// 実体持ちを探す
	DrawLine* pDrawLine = (DrawLine*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_DRAWLINE);

	// １つも作られていなかったら生成する
	if (pDrawLine == nullptr) {
		// 生成処理
		pDrawLine = DrawLine::Create(D3DXVECTOR3(0, 0, 0));

		// 設定処理
		pDrawLine->SetViaPoint_private(position, pStartPoint);

		return;
	}

	// 作られていたら未使用領域を探す
	DrawLine* pCurrent = (DrawLine*)pDrawLine;
	DrawLine* pNext    = (DrawLine*)pDrawLine->GetNextPointer();
	for (;;) {
		// 未使用なら設定して終了
		if (!pCurrent->GetInstance()) {
			pCurrent->SetViaPoint_private(position, pStartPoint);
			break;
		}

		// 未使用領域が見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = DrawLine::Create(D3DXVECTOR3(0, 0, 0));

			// 設定処理
			pNext->SetViaPoint_private(position, pStartPoint);

			break;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext = (DrawLine*)pCurrent->GetNextPointer();

	}

}

// DSの発射
void DrawLine::SetDrawShot(DrawLine* pStartPoint)
{
	// 始点のNULLチェック
	if (pStartPoint == nullptr) {
		// エラーメッセージ
		_MSGERROR("pStartPoint cannnot be nullptr!!", "void DrawLine::SetDrawShot(DrawLine* pStartPoint)");
	}

	// DSを撃つ
	DrawShot::SetDrawShot(pStartPoint);

}

// パラメータのクリア
void DrawLine::ClearParameter()
{
	// データのクリア
	m_type      = DrawLine::TYPE::TYPE_START;	// 種類
	m_pNext     = nullptr;						// 次の点
	m_pTail     = nullptr;						// 末尾の点
	m_bInstance = false;						// 使用中フラグ
	m_pPlayer   = nullptr;						// 親プレイヤー

}

// データのクリア：繋がってる領域全て
void DrawLine::ClearParameter_List(DrawLine* pStartPoint)
{
	// 始点のNULLチェック
	if (pStartPoint == nullptr) {
		// エラーメッセージ
		_MSGERROR("pStartPoint cannnot be nullptr!!", "void DrawLine::ClearParameter_List(DrawLine* pStartPoint)");
	}

	// 始点と次の点の取得
	DrawLine* pCurrent = (DrawLine*)pStartPoint;
	DrawLine* pNext    = (DrawLine*)pStartPoint->m_pNext;

	for (;;) {
		// データのクリア
		pCurrent->ClearParameter();

		// 終点なら処理終了
		if (pNext == nullptr) break;

		// 次の点へずらす
		pCurrent = pNext;
		pNext    = pCurrent->m_pNext;

	}
}



//=============================================================================
//	private関数
// 始点の設定
void DrawLine::SetStartLine_private(D3DXVECTOR3& position, Player& player)
{
	// 種類の設定
	m_type = DrawLine::TYPE::TYPE_START;

	// 座標の設定
	SetPosition(position);

	// 末尾のポインタ設定
	m_pTail = this;

	// 親プレイヤーの設定
	m_pPlayer = &player;

	// 使用中フラグON
	m_bInstance = true;

}

// 経由点の設定
void DrawLine::SetViaPoint_private(D3DXVECTOR3& position, DrawLine* pStartPoint)
{
	// 種類の設定
	m_type = DrawLine::TYPE::TYPE_VIA;

	// 座標の設定
	SetPosition(position);

	// 前のポインタと自分を繋ぐ
	pStartPoint->m_pTail->m_pNext = this;

	// 末尾のポインタを自分に更新
	pStartPoint->m_pTail = this;

	// 親プレイヤーの設定
	m_pPlayer = pStartPoint->m_pPlayer;

	// 使用中フラグON
	m_bInstance = true;

}
