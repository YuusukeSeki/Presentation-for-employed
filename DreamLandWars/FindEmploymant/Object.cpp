// author : yusuke seki
// data   : 20181110
#include "Object.h"

Object* Object::headObj_[Object::TYPE::TYPE_MAX] = {};
Object* Object::tailObj_[Object::TYPE::TYPE_MAX] = {};

Object::Object()
{

}

Object::Object(Object::TYPE type)
{
	// 初めて生成された種類なら、先頭にポインタを代入。
	if (headObj_[type] == nullptr)
	{
		// ヘッドの更新
		headObj_[type] = this;
		headObj_[type]->nextObj_ = nullptr;
		headObj_[type]->prevObj_ = nullptr;

		// テイルの更新
		tailObj_[type] = headObj_[type];

		// 前後のポインタの初期化
		nextObj_ = nullptr;
		prevObj_ = nullptr;

	}
	// ２個目以上に生成された種類なら、末尾に追加。
	else
	{
		// テイルの次のポインタを"これ"にする
		tailObj_[type]->nextObj_ = this;

		// 前後のポインタの初期化
		nextObj_ = nullptr;
		prevObj_ = tailObj_[type];

		// テイルの更新
		tailObj_[type] = this;

	}

	// メンバ変数の初期化
	position_ = D3DXVECTOR3(0, 0, 0);
	type_ = type;
	group_ = Object::GROUP::GROUP_NONE;
	isActive_ = true;

}


//-----------------------------------------------------------------------------
//	解放処理：個別
//-----------------------------------------------------------------------------
void Object::Release()
{
	// 前後のオブジェクトが持っている、前後のポインタを更新
	if (nextObj_ != nullptr) nextObj_->prevObj_ = prevObj_;
	if (prevObj_ != nullptr) prevObj_->nextObj_ = nextObj_;

	// ヘッドのポインタを更新
	if (headObj_[type_] == this) headObj_[type_] = nextObj_;

	// テイルのポインタを更新
	if (tailObj_[type_] == this) tailObj_[type_] = prevObj_;

	// 終了処理
	Uninit();

	// メモリの解放
	delete this;

}


//-----------------------------------------------------------------------------
//	更新処理：全体
//-----------------------------------------------------------------------------
void Object::UpdateAll()
{
	int cnt[Object::TYPE::TYPE_MAX] = {};

	for (int i = 0; i < Object::TYPE::TYPE_MAX; i++)
	{
		Object* pCurrent = headObj_[i];

		for (;;)
		{
			if (pCurrent == nullptr)
			{
				break;
			}
			else
			{
				if (pCurrent->GetActive() == true)
				{
					pCurrent->Update();
				}

				pCurrent = pCurrent->nextObj_;
				cnt[i]++;
			}
		}
	}

	OX::DebugFont::print(0, SCREEN_HEIGHT - 420, 0xffff8080, "___Create Object___");
	OX::DebugFont::print(0, SCREEN_HEIGHT - 380, 0xffff0000, "TYPE_COMMANDER           : %d", cnt[TYPE_COMMANDER]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 360, 0xffff0000, "TYPE_MODEL_SOLDIER       : %d", cnt[TYPE_MODEL_SOLDIER]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 300, 0xffff0000, "TYPE_MODEL_TOWER         : %d", cnt[TYPE_MODEL_TOWER]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 260, 0xffff0000, "TYPE_3D_BILLBOARD        : %d", cnt[TYPE_3D_BILLBOARD]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 240, 0xffff0000, "TYPE_3D_SOLDIERBULLET    : %d", cnt[TYPE_3D_SOLDIERBULLET]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 220, 0xffff0000, "TYPE_3D_BILLBOARD_BULLET : %d", cnt[TYPE_3D_BILLBOARD_BULLET]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 200, 0xffff0000, "TYPE_3D_BILLBOARD_EFFECT : %d", cnt[TYPE_3D_BILLBOARD_EFFECT]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 180, 0xffff0000, "TYPE_3D_STRAIGHTSHOT     : %d", cnt[TYPE_3D_STRAIGHTSHOT]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 160, 0xffff0000, "TYPE_3D_DRAWLINE         : %d", cnt[TYPE_3D_DRAWLINE]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 140, 0xffff0000, "TYPE_3D_DRAWSHOT         : %d", cnt[TYPE_3D_DRAWSHOT]);

}


//-----------------------------------------------------------------------------
//	描画処理：全体
//-----------------------------------------------------------------------------
void Object::DrawAll()
{
	for (int i = 0; i < Object::TYPE::TYPE_MAX; i++)
	{
		Object* pCurrent = headObj_[i];

		for (;;)
		{
			if (pCurrent == nullptr)
			{
				break;
			}
			else
			{
				if (pCurrent->GetActive() == true)
				{
					pCurrent->Draw();
				}

				pCurrent = pCurrent->nextObj_;
			}
		}
	}

}


//-----------------------------------------------------------------------------
//	解放処理：全体
//-----------------------------------------------------------------------------
void Object::ReleaseAll()
{
	for (int i = 0; i < Object::TYPE::TYPE_MAX; i++)
	{
		if (headObj_[i] == nullptr)
		{
			continue;
		}

		Object* pCurrent = headObj_[i];
		Object* pNext    = headObj_[i]->nextObj_;

		for (;;)
		{
			if (pCurrent == nullptr)
			{
				break;
			}
			else
			{
				pNext = pCurrent->nextObj_;
				pCurrent->Release();
				pCurrent = pNext;
				
			}
		}
	}
}


