// author : yusuke seki
// data   : 20181121
#include "SoldierGauge.h"
#include "MainGame.h"
#include "Soldier.h"

SoldierGauge::SoldierGauge() : Object(TYPE_SOLDIERGAUGE)
{
	worldMatrix_ = {};

	maxHp_ = nullptr;
	currentHp_ = nullptr;
	halfSize_ = D3DXVECTOR3(0, 0, 0);
	initWidth_ = 0;

	currentGauge_ = nullptr;
	backGround_ = nullptr;

	isUpdateWorldMatrix_ = false;

	parentSoldier_ = nullptr;
	previewParentSoldierPosition_ = D3DXVECTOR3(0, 0, 0);
}

SoldierGauge::~SoldierGauge()
{
	Uninit();
}

void SoldierGauge::CreateBuffer(const unsigned int& _numCreate)
{
	for (unsigned int i = 0; i < _numCreate; ++i)
	{
		SoldierGauge* buf = new SoldierGauge();
		buf->SetActive(false);
	}
}

SoldierGauge* SoldierGauge::Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, Soldier* _parentSoldier)
{
	SoldierGauge* gauge = new SoldierGauge();
	gauge->Init(_position, _size, _camera, _parentSoldier);

	return gauge;
}

SoldierGauge* SoldierGauge::DynamicCreate(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, Soldier* _parentSoldier)
{
	SoldierGauge* soldierGauge = FindNonActive();

	if (soldierGauge == nullptr)
	{
		soldierGauge = Create(_position, _size, _camera, _parentSoldier);
	}
	else
	{
		soldierGauge->Init(_position, _size, _camera, _parentSoldier);
	}

	return soldierGauge;
}

void SoldierGauge::Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Camera* _camera, Soldier* _parentSoldier)
{
	SetPosition(_position);

	halfSize_ = _size * 0.5f;

	parentSoldier_ = _parentSoldier;

	previewParentSoldierPosition_ = parentSoldier_->GetPosition();

	UpdateWorldMatrix();

	isUpdateWorldMatrix_ = false;

	maxHp_ = nullptr;
	currentHp_ = nullptr;

	backGround_ = ObjectBillboard::DynamicCreate(D3DXVECTOR3(0, 0, 0), halfSize_ * 1.0f, MainGame::GetCamera(), false);
	backGround_->SetParent(&worldMatrix_);
	backGround_->SetColor(0x000000ff);

	D3DXVECTOR3 size = backGround_->GetSize();
	size.x *= 0.8f;
	size.y *= 0.5f;

	currentGauge_ = ObjectBillboard::DynamicCreate(D3DXVECTOR3(0, 0, 0), size, MainGame::GetCamera(), false);
	currentGauge_->SetParent(&worldMatrix_);
	currentGauge_->SetColor(parentSoldier_->GetColor().color);

	initWidth_ = currentGauge_->GetSize().x;

	SetActive(true);
}

void SoldierGauge::Uninit(void)
{

}

void SoldierGauge::Update(void)
{
	CheckCurrentHp();

	if (IsMovePositionParentUnit() == true)
	{
		ChaseParentSoldier();
	}

	previewParentSoldierPosition_ = parentSoldier_->GetPosition();

	isUpdateWorldMatrix_ = true;
}

void SoldierGauge::Draw(void)
{
	if (isUpdateWorldMatrix_ == true)
	{
		UpdateWorldMatrix();

		backGround_->SetUpdateWorldMatrix(true);
		currentGauge_->SetUpdateWorldMatrix(true);

		isUpdateWorldMatrix_ = false;
	}
}

void SoldierGauge::Release(void)
{
	Object::Release();
}

void SoldierGauge::ReleaseGauge()
{
	if (currentGauge_ != nullptr)
	{
		currentGauge_->Release();
		currentGauge_ = nullptr;
	}

	if (backGround_ != nullptr)
	{
		backGround_->Release();
		backGround_ = nullptr;
	}

	Object::Release();
}

void SoldierGauge::SetMaxHpPointer(float* _maxHp)
{
	maxHp_ = _maxHp;
}

void SoldierGauge::SetCurrentHpPointer(float* _currentHp)
{
	currentHp_ = _currentHp;
}

void SoldierGauge::SetPosition(const D3DXVECTOR3& _position)
{
	Object::SetPosition(_position);

	isUpdateWorldMatrix_ = true;
}

void SoldierGauge::MovePosition(const D3DXVECTOR3& _move)
{
	Object::MovePosition(_move);

	isUpdateWorldMatrix_ = true;
}

void SoldierGauge::SetActive(const bool& _isActive)
{
	Object::SetActive(_isActive);

	currentGauge_->SetActive(_isActive);
	
	backGround_->SetActive(_isActive);
}

SoldierGauge* SoldierGauge::FindNonActive()
{
	SoldierGauge* soldierGauge = (SoldierGauge*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_SOLDIERGAUGE);

	if (soldierGauge != nullptr)
	{
		for (;;)
		{
			if (soldierGauge->GetActive() == false)
			{
				break;
			}

			soldierGauge = (SoldierGauge*)soldierGauge->GetNextPointer();

			if (soldierGauge == nullptr)
			{
				break;
			}
		}
	}

	return soldierGauge;
}

void SoldierGauge::CheckCurrentHp()
{
	if (maxHp_ == nullptr || currentHp_ == nullptr)
	{
		return;
	}

	float percentage = *currentHp_ / *maxHp_ < 0 ? 0 : *currentHp_ / *maxHp_;

	if (percentage >= 1.0f)
	{
		backGround_->SetIsDraw(false);
		currentGauge_->SetIsDraw(false);
	}
	else
	{
		backGround_->SetIsDraw(true);
		currentGauge_->SetIsDraw(true);
	}

	D3DXVECTOR3 movePosition(0, 0, 0);
	float currentWidth, nextWidth, differenceWidth;
	currentWidth = currentGauge_->GetSize().x;

	nextWidth = currentWidth;
	nextWidth = initWidth_ * percentage;

	differenceWidth = currentWidth - nextWidth;

	movePosition = D3DXVECTOR3(differenceWidth * 0.5f * -1, 0, 0);

	currentGauge_->MovePosition(movePosition);

	currentGauge_->SetSize(D3DXVECTOR3(nextWidth, currentGauge_->GetSize().y, currentGauge_->GetSize().z));

	currentGauge_->SetUv_Size(D3DXVECTOR2(percentage, 1));
}

void SoldierGauge::UpdateWorldMatrix()
{
	D3DXMATRIX translateMatrix, rotateMatrix, scaleMatrix;
	D3DXVECTOR3 position = GetPosition();

	D3DXMatrixIdentity(&worldMatrix_);

	D3DXMatrixTranslation(&translateMatrix, position.x, position.y, position.z);

	D3DXMatrixRotationYawPitchRoll(&rotateMatrix, 0, 0, 0);

	D3DXMatrixScaling(&scaleMatrix, 1, 1, 1);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &scaleMatrix);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &rotateMatrix);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &translateMatrix);
}

bool SoldierGauge::IsMovePositionParentUnit()
{
	if (parentSoldier_->GetPosition() != previewParentSoldierPosition_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SoldierGauge::ChaseParentSoldier()
{
	D3DXVECTOR3 moveVector = parentSoldier_->GetPosition() - previewParentSoldierPosition_;
	MovePosition(moveVector);
}
