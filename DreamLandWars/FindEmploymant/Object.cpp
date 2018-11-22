// author : yusuke seki
// data   : 20181110
#include "Object.h"
#include "renderer.h"

#include "SoldierCommander.h"

Object* Object::headObj_[Object::TYPE::TYPE_MAX] = {};
Object* Object::tailObj_[Object::TYPE::TYPE_MAX] = {};
#ifdef _DEBUG
bool Object::isDrawCollider_ = true;
#endif


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

	static int j;
	j = 0;

	for (int i = 0; i < Object::TYPE::TYPE_MAX; i++)
	{
		Object* current = headObj_[i];

		if (current != nullptr)
		{
			Object* next = current->nextObj_;

			for (;;)
			{
				if (current != nullptr)
				{
					cnt[i]++;

					//if (i == TYPE_SOLDIERGAUGE)
					//{
					//	OX::DebugFont::print(0, 60 + 20 * j, 0xffffffff
					//		, "commander : %f, %f, %f", current->GetPosition().x, current->GetPosition().y, current->GetPosition().z);

					//	j++;
					//}
					//else if (i == TYPE_MODEL_SOLDIER)
					//{
					//	OX::DebugFont::print(0, 60 + 20 * j, 0xffffffff
					//		, "soldier : %f, %f, %f", current->GetPosition().x, current->GetPosition().y, current->GetPosition().z);

					//	j++;
					//}

					if (current->GetActive() == true)
					{
						current->Update();
					}

					if (next == nullptr)
					{
						break;
					}

					current = next;
					next = current->nextObj_;
				}
			}
		}
	}

	int posY = SCREEN_HEIGHT - 140;
	int a = 0;

	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "gauge     : %d", cnt[TYPE_SOLDIERGAUGE]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "billboard : %d", cnt[TYPE_3D_BILLBOARD]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "collider  : %d", cnt[COLLIDER]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "model     : %d", cnt[TYPE_MODEL]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "castle    : %d", cnt[TYPE_MODEL_CASTLE]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "tower     : %d", cnt[TYPE_MODEL_TOWER]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "soldier   : %d", cnt[TYPE_MODEL_SOLDIER]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "commander : %d", cnt[TYPE_COMMANDER]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff0000, "generator : %d", cnt[TYPE_GENERATOR]);
	a++;
	OX::DebugFont::print(0, posY - 20 * a, 0xffff8080, "___Create Object___");
	a++;
}


//-----------------------------------------------------------------------------
//	描画処理：全体
//-----------------------------------------------------------------------------
void Object::DrawAll()
{
	for (int i = 0; i < Object::TYPE::TYPE_MAX; i++)
	{
#ifdef _DEBUG
		if (i == Object::TYPE::COLLIDER)
		{
			if (isDrawCollider_ == false)
			{
				continue;
			}
			else
			{
				//Renderer::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
		}
#endif

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

#ifdef _DEBUG
		//Renderer::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
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


