// author : yusuke seki
// data   : 20181111
#include "DrawRange.h"
#include "player.h"
#include "main.h"

DrawRange::DrawRange() : parentPlayer_(nullptr)
{
	Release();
}

DrawRange::DrawRange(Player* _parentPlayer) : parentPlayer_(_parentPlayer)
{
	if (_parentPlayer == nullptr)
	{
		_MSGERROR("親プレイヤーの取得に失敗しました", "DrawRange::DrawRange(Player*)");
		Release();
		return;
	}

	object_ = nullptr;
	preRange_ = 0;
}

DrawRange::~DrawRange()
{
	Uninit();
}

DrawRange* DrawRange::Create(Player* const _parentPlayer)
{
	DrawRange* drawRange = new DrawRange(_parentPlayer);
	drawRange->Init();

	return drawRange;
}

void DrawRange::Init()
{
	D3DXVECTOR3 objectSize(parentPlayer_->GetDrawShotRange() * 2, parentPlayer_->GetDrawShotRange() * 2, 0);

	object_ = Object3D::Create(parentPlayer_->GetPosition(), objectSize);
	object_->SetRotate(D3DXVECTOR3(1, 0, 0), D3DXToRadian(90));

	preRange_ = parentPlayer_->GetDrawShotRange();
}

void DrawRange::Uninit(void)
{
	if (object_ != nullptr)
	{
		object_->Uninit();
	}
}

void DrawRange::Update(void)
{
	UpdateIsDraw();

	if (object_->GetIsDraw() == true)
	{
		SneakPlayerPosition();

		if (IsChangeSize() == true)
		{
			ChangeSize();
		}

		preRange_ = parentPlayer_->GetDrawShotRange();
	}
}

void DrawRange::Release()
{
	if (object_ != nullptr)
	{
		object_->Release();
		object_ = nullptr;
	}

	delete this;
}

void DrawRange::SetColor(const unsigned int& _rgba)
{
	object_->SetColor(_rgba);
}

void DrawRange::SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a)
{
	object_->SetColor(_r, _g, _b, _a);
}

void DrawRange::SetTexture(const LPDIRECT3DTEXTURE9& _texture)
{
	object_->SetTexture(_texture);
}

void DrawRange::UpdateIsDraw()
{
	if (parentPlayer_->GetBehave() == Player::Behave::READY_DRAW_SHOT)
	{
		object_->SetIsDraw(true);
	}
	else
	{
		object_->SetIsDraw(false);
	}
}

void DrawRange::SneakPlayerPosition()
{
	object_->SetPosition(parentPlayer_->GetPosition());
}

bool DrawRange::IsChangeSize()
{
	if (preRange_ != parentPlayer_->GetDrawShotRange())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DrawRange::ChangeSize()
{
	float currentRange = parentPlayer_->GetDrawShotRange();
	D3DXVECTOR3 objectSize(currentRange * 2, currentRange * 2, 0);

	object_->SetSize(objectSize);
}
