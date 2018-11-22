// author : yusuke seki
// data   :20181110
#include "player.h"
#include "list_CharaParameters.h"
#include "MainGame.h"
#include "list_LoadTexture_MainGame.h"
#include "ModelData.h"
#include "Part.h"
#include "collision.h"
#include "renderer.h"
#include "field.h"
#include "GameManager.h"
#include "input.h"
#include "camera.h"
#include "StraightShot.h"
#include "DrawLine.h"
#include "tower.h"
#include "castle.h"
#include "Hold.h"
#include "Wall.h"

#include "Unit.h"
#include "Collider.h"

const std::vector<std::string> Player::modelPassList_ =
{
	"data/model/MainGame/player03.x",
	"data/model/MainGame/.x",
	"data/model/MainGame/.x",
};

const float Player::kInirtia_ = 0.7f;
const float Player::kMinThresholdSpeed_ = 0.001f;
const float Player::kThresholdLength_AddDraw_ = 10.0f;

#define KEY_MAX		2

Player::KEY_FRAME g_KeyFrameWalk[KEY_MAX] =
{
	// 0 key
	{
		30,	// num frame
		{
			{ D3DXVECTOR3(0.f, 20.f, -100.f), D3DXVECTOR3( 0.54f,  0.09f,  0.00f) },	// BODY
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.48f, -0.03f,  0.00f) },	// HEAD
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, -0.36f,  0.96f) },	// RARM
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f,  0.00f, -0.87f) },	// REDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, -0.60f, -0.03f) },	// LARM
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f,  0.60f,  0.57f) },	// LEDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.99f,  0.00f,  0.00f) },	// RLEG
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.60f,  0.00f,  0.00f) },	// REDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-1.74f,  0.00f,  0.00f) },	// LLEG
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.51f,  0.00f,  0.00f) },	// LEDGE
		}
	},

	// 1 key
	{
		30,	// num frame
		{
			{ D3DXVECTOR3(0.f, 20.f, -100.f), D3DXVECTOR3( 0.54f, -0.09f,  0.00f) },	// BODY
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.48f, +0.03f,  0.00f) },	// HEAD
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, +0.36f, -0.96f) },	// RARM
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f,  0.00f, +0.87f) },	// REDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, +0.60f, +0.03f) },	// LARM
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, -0.60f, -0.57f) },	// LEDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.99f,  0.00f,  0.00f) },	// RLEG
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.60f,  0.00f,  0.00f) },	// REDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(+1.74f,  0.00f,  0.00f) },	// LLEG
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.51f,  0.00f,  0.00f) },	// LEDGE
		}
	},

	//// 2 key
	//{
	//	30,	// num frame
	//	{
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// BODY
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// HEAD
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// RARM
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// REDGE
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// LARM
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// LEDGE
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// RLEG
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// REDGE
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// LLEG
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// LEDGE
	//	}
	//},

};

Player::Player() : Unit(TYPE::TYPE_MODEL_PLAYER), kCharacter_(Character::FIGHTER), kInitCameraPositionEye_(0.f, 200.f, -110.f), kInitCameraPositionAt_(0.f, 0.f, 0.f)
                  ,kInitCameraVectorUp_(0.f, 1.f, 0.f), kInitCameraNear_(0.1f), kInitCameraFar_(500.0f)
{
	Release();
}

Player::Player(Object::TYPE type, const Player::Character& character) : Unit(type) , kCharacter_(character)
	, kInitCameraPositionEye_(0.f, 200.f, -110.f), kInitCameraPositionAt_(0.f, 0.f, 0.f)
	, kInitCameraVectorUp_(0.f, 1.f, 0.f), kInitCameraNear_(0.1f), kInitCameraFar_(500.0f)
{
	const D3DXVECTOR3 zero(0.0f, 0.0f, 0.0f);

	camera_ = nullptr;
	inverseMatrix_ = {};
	controller_ = nullptr;
	cursorPosition3D_ = zero;

	moveVector_ = zero;
	currentSpeed_ = 0.0f;
	runSpeed_ = 0.0f;
	avoidPower_ = 0.0f;
	heavyAvoidPower_ = 0.0f;
	prePosition_ = zero;

	countTime_Rotate_ = 0.0f;
	startRotate_ = zero;
	endRotate_ = zero;
	startFront_ = zero;
	endFront_ = zero;

	currentBehave_ = Behave::NEUTRAL;
	currentCoolTime_ = 0;
	coolTimeAvoid_ = 0;
	coolTimeHeavyAvoid_ = 0;
	coolTimeStraightShot_ = 0;
	coolTimeDrawShot_ = 0;
	coolTimeBreakBasePoint_ = 0;

	drawLine_ = nullptr;
	drawLine_currentLength_ = 0.0f;
	drawLine_preLength_ = 0.0f;

	currentHp_ = 0.0f;
	maxHp_ = 0.0f;
	currentMp_ = 0.0f;
	maxMp_ = 0.0f;

	straightShotDamage_ = 0.0f;
	straightShotSpeed_ = 0.0f;
	straightShotRange_ = 0.0f;

	drawShotDamage_ = 0.0f;
	drawShotSpeed_ = 0.0f;
	drawShotRange_ = 0.0f;
	drawShotLength_ = 0.0f;

	breakPower_ = 0.0f;
}

Player::~Player()
{
	Uninit();
}

 Player* Player::Create(const D3DXVECTOR3& position, const Player::Character& character, const Object::GROUP& _group)
{
	Player* player = new Player(Object::TYPE_MODEL_PLAYER, character);
	player->Init(position, character, _group);

	return player;
}

void Player::Init(const D3DXVECTOR3& position, const Player::Character& character, const Object::GROUP& _group)
{
	// create controller
	if (controller_ == nullptr)
	{
		controller_ = Controller::Create(Controller::Device::KEYBOARDANDMOUSE);
	}

	Unit::Init(position, _group);

	// create model
	//const std::string modelPass = FindModelPass(character);

	AroundParts();


	// create camera
	camera_ = Camera::Create(kInitCameraPositionEye_, kInitCameraPositionAt_, kInitCameraVectorUp_, kInitCameraNear_, kInitCameraFar_);

	// create draw line
	drawLine_ = nullptr;
	drawLine_currentLength_ = 0;
	drawLine_preLength_ = 0;

	InitParamater(kCharacter_);

	cursorPosition3D_ = FindCursorPosition_3D();

	startRotate_ = GetFront();
	endRotate_ = GetFront();
	countTime_Rotate_ = 1.0f;
}

void Player::Uninit(void)
{
	if (camera_ != nullptr)
	{
		camera_->Release();
		camera_ = nullptr;
	}

	//if (controller_ != nullptr)
	//{
	//	controller_->Release();
	//	controller_ = nullptr;
	//}

}

void Player::Update(void)
{
	UpdateInverseMatrix();

	cursorPosition3D_ = FindCursorPosition_3D();

	if (currentCoolTime_ != 0)
	{
		--currentCoolTime_;

		if (currentCoolTime_ == 0)
		{
			currentBehave_ = Behave::NEUTRAL;
		}
	}

	Move();

	StraightShot();

	DrawShot();

	Break();

	CollisionTower();
	
	CollisionCastle();

	CollisionWall();

	CollisionField();

	prePosition_ = GetPosition();


	return;


	//TempInput();

	for (int i = 0; i < 10; i++)
	{
		float rate = (float)m_Frame / m_KeyFrame[m_Key].Frame;
		D3DXVECTOR3 curPos  = m_KeyFrame[m_Key].Key[i].Position;

		D3DXVECTOR3 nextPos = m_KeyFrame[m_Key + 1 >= KEY_MAX ? 0 : m_Key + 1].Key[i].Position;
		D3DXVECTOR3 curRot  = m_KeyFrame[m_Key].Key[i].Rotation;
		D3DXVECTOR3 nextRot = m_KeyFrame[m_Key + 1 >= KEY_MAX ? 0 : m_Key + 1].Key[i].Rotation;

		m_Part[i]->SetPosition(curPos * (1.f - rate) + nextPos * rate);
		m_Part[i]->SetRotate(curRot * (1.f - rate) + nextRot * rate);

	}

	m_Frame++;

	if (m_Frame >= m_KeyFrame[m_Key].Frame)
	{
		m_Key = m_Key + 1 >= KEY_MAX ? 0 : m_Key + 1;
		m_Frame = 0;
	}
}

void Player::Draw(void)
{
	Unit::Draw();
}

void Player::SetBehave(const Player::Behave& _behave)
{
	currentBehave_ = _behave;
}

Player::Behave Player::GetBehave()
{
	return currentBehave_;
}

void Player::SetCoolTime(const unsigned int _coolTime)
{
	currentCoolTime_ = _coolTime;
}

float Player::GetStraightShotDamage()
{
	return straightShotDamage_;
}

float Player::GetStraightShotSpeed()
{
	return straightShotSpeed_;
}

float Player::GetStraightShotRange()
{
	return straightShotRange_;
}

float Player::GetDrawShotDamage()
{
	return drawShotDamage_;
}

float Player::GetDrawShotSpeed()
{
	return drawShotSpeed_;
}

float Player::GetDrawShotRange()
{
	return drawShotRange_;
}

Camera* Player::GetCamera()
{
	return camera_;
}

void Player::TempInput()
{
	Input *pInput = GameManager::GetInput();
	static int iterator_part, iterator_rotate;
	static char str[3] = {'X', 'Y', 'Z'};
	float addValue = 0.03f;

	if (pInput->GetKeyboardTrigger(DIK_UP))
	{
		iterator_part = iterator_part - 1 < 0 ? 9 : iterator_part - 1;
	}
	if (pInput->GetKeyboardTrigger(DIK_DOWN))
	{
		iterator_part = iterator_part + 1 > 9 ? 0 : iterator_part + 1;
	}

	D3DXVECTOR3 rotate;
	rotate = m_Part[iterator_part]->GetRotate();


	if (pInput->GetKeyboardRepeat(DIK_LEFT))
	{
		if (iterator_part >= 0 && iterator_part <= 9) {
			switch (iterator_rotate) {
				case 0:
					rotate.x -= addValue;
					break;
				case 1:
					rotate.y -= addValue;
					break;
				case 2:
					rotate.z -= addValue;
					break;
			}
		}
	}
	if (pInput->GetKeyboardRepeat(DIK_RIGHT))
	{
		if (iterator_part >= 0 && iterator_part <= 9) {
			switch (iterator_rotate) {
			case 0:
				rotate.x += addValue;
				break;
			case 1:
				rotate.y += addValue;
				break;
			case 2:
				rotate.z += addValue;
				break;
			}
		}
	}

	if (pInput->GetKeyboardTrigger(DIK_X))
		iterator_rotate = 0;
	if (pInput->GetKeyboardTrigger(DIK_Y))
		iterator_rotate = 1;
	if (pInput->GetKeyboardTrigger(DIK_Z))
		iterator_rotate = 2;


	m_Part[iterator_part]->SetRotate(rotate);

	OX::DebugFont::print(400, 20* 0, 0xff00ff00, "select rotate = %c", str[iterator_rotate]);
	OX::DebugFont::print(400, 20* 1, 0xff00ff00, "BODY _ X:%f, Y:%f, Z:%f", m_Part[0]->GetRotate().x, m_Part[0]->GetRotate().y, m_Part[0]->GetRotate().z);
	OX::DebugFont::print(400, 20* 2, 0xff00ff00, "HEAD _ X:%f, Y:%f, Z:%f", m_Part[1]->GetRotate().x, m_Part[1]->GetRotate().y, m_Part[1]->GetRotate().z);
	OX::DebugFont::print(400, 20* 3, 0xff00ff00, "RARM _ X:%f, Y:%f, Z:%f", m_Part[2]->GetRotate().x, m_Part[2]->GetRotate().y, m_Part[2]->GetRotate().z);
	OX::DebugFont::print(400, 20* 4, 0xff00ff00, "REDGE_ X:%f, Y:%f, Z:%f", m_Part[3]->GetRotate().x, m_Part[3]->GetRotate().y, m_Part[3]->GetRotate().z);
	OX::DebugFont::print(400, 20* 5, 0xff00ff00, "LARM _ X:%f, Y:%f, Z:%f", m_Part[4]->GetRotate().x, m_Part[4]->GetRotate().y, m_Part[4]->GetRotate().z);
	OX::DebugFont::print(400, 20* 6, 0xff00ff00, "LEDGE_ X:%f, Y:%f, Z:%f", m_Part[5]->GetRotate().x, m_Part[5]->GetRotate().y, m_Part[5]->GetRotate().z);
	OX::DebugFont::print(400, 20* 7, 0xff00ff00, "RLEG _ X:%f, Y:%f, Z:%f", m_Part[6]->GetRotate().x, m_Part[6]->GetRotate().y, m_Part[6]->GetRotate().z);
	OX::DebugFont::print(400, 20* 8, 0xff00ff00, "REDGE_ X:%f, Y:%f, Z:%f", m_Part[7]->GetRotate().x, m_Part[7]->GetRotate().y, m_Part[7]->GetRotate().z);
	OX::DebugFont::print(400, 20* 9, 0xff00ff00, "LLEG _ X:%f, Y:%f, Z:%f", m_Part[8]->GetRotate().x, m_Part[8]->GetRotate().y, m_Part[8]->GetRotate().z);
	OX::DebugFont::print(400, 20*10, 0xff00ff00, "LEDGE_ X:%f, Y:%f, Z:%f", m_Part[9]->GetRotate().x, m_Part[9]->GetRotate().y, m_Part[9]->GetRotate().z);
	OX::DebugFont::print(390, 20 + iterator_part * 20, 0xff00ff00, "-> ");


}

void Player::ReceiveDamage(const float& _damage, Unit* _unit)
{
	currentHp_ -= _damage;

	if (currentHp_ <= 0.0f)
	{
		// 死亡処理
	}
}

bool Player::IsRun()
{
	if (currentCoolTime_ != 0)
	{
		return false;
	}
	
	if ((currentBehave_ == Behave::NEUTRAL || currentBehave_ == Behave::RUN) == false)
	{
		return false;
	}

	if (controller_->IsButtonPress(Controller::Key::RUN) == false)
	{
		return false;
	}

	return true;
}

bool Player::IsAvoid()
{
	if (currentCoolTime_ != 0)
	{
		return false;
	}

	if ((currentBehave_ == Behave::NEUTRAL || currentBehave_ == Behave::RUN) == false)
	{
		return false;
	}

	if (controller_->IsButtonTrigger(Controller::Key::AVOID) == false)
	{
		return false;
	}

	return true;
}

bool Player::IsHeavyAvoid()
{
	if (currentCoolTime_ == 0)
	{
		return false;
	}

	if ((currentBehave_ == Behave::STRAIGHT_SHOT || currentBehave_ == Behave::DRAW_SHOT) == false)
	{
		return false;
	}

	if (controller_->IsButtonTrigger(Controller::Key::HEAVYAVOID) == false)
	{
		return false;
	}

	return true;
}

bool Player::IsReady_StraightShot(const D3DXVECTOR3& _cursorPosition_3D)
{
	if (currentCoolTime_ != 0)
	{
		return false;
	}

	if ((currentBehave_ == Behave::NEUTRAL || currentBehave_ == Behave::RUN) == false)
	{
		return false;
	}

	if (controller_->IsButtonTrigger(Controller::Key::STRAIGHTSHOT) == false)
	{
		return false;
	}

	if (CollisionCursorToPlayer(_cursorPosition_3D) == false)
	{
		return false;
	}

	return true;
}

bool Player::IsAttack_StraightShot(const D3DXVECTOR3& _cursorPosition_3D)
{
	if ((currentBehave_ == Behave::READY_STRAIGHT_SHOT) == false)
	{
		return false;
	}

	if (controller_->IsButtonRelease(Controller::Key::STRAIGHTSHOT) == false)
	{
		return false;
	}

	if (CollisionCursorToPlayer(_cursorPosition_3D) == true)
	{
		return false;
	}

	return true;
}

bool Player::IsCancel_StraightShot(const D3DXVECTOR3& _cursorPosition_3D)
{
	if ((currentBehave_ == Behave::READY_STRAIGHT_SHOT) == false)
	{
		return false;
	}

	if (controller_->IsButtonRelease(Controller::Key::STRAIGHTSHOT) == false)
	{
		return false;
	}

	if (CollisionCursorToPlayer(_cursorPosition_3D) == false)
	{
		return false;
	}

	return true;
}

bool Player::IsReady_DrawShot(const D3DXVECTOR3& _cursorPosition_3D)
{
	if (currentCoolTime_ != 0)
	{
		return false;
	}

	if ((currentBehave_ == Behave::NEUTRAL || currentBehave_ == Behave::RUN) == false)
	{
		return false;
	}

	if (controller_->IsButtonTrigger(Controller::Key::DRAWSHOT) == false)
	{
		return false;
	}

	if (CollisionCursorToPlayer(_cursorPosition_3D) == false)
	{
		return false;
	}

	return true;
}

bool Player::IsCreate_DrawLine()
{
	if ((currentBehave_ == Behave::READY_DRAW_SHOT) == false)
	{
		return false;
	}

	if (controller_->IsButtonPress(Controller::Key::DRAWSHOT) == false)
	{
		return false;
	}

	return true;
}

bool Player::IsAttack_DrawShot(const D3DXVECTOR3& _cursorPosition_3D)
{
	if ((currentBehave_ == Behave::READY_DRAW_SHOT) == false)
	{
		return false;
	}

	if (controller_->IsButtonRelease(Controller::Key::DRAWSHOT) == false)
	{
		return false;
	}

	if (CollisionCursorToPlayer(_cursorPosition_3D) == true)
	{
		return false;
	}

	return true;
}

bool Player::IsCancel_DrawShot(const D3DXVECTOR3& _cursorPosition_3D)
{
	if ((currentBehave_ == Behave::READY_DRAW_SHOT) == false)
	{
		return false;
	}

	if (controller_->IsButtonRelease(Controller::Key::DRAWSHOT) == false)
	{
		return false;
	}

	if (CollisionCursorToPlayer(_cursorPosition_3D) == false)
	{
		return false;
	}

	return true;
}

bool Player::IsBreakBasePoint(const D3DXVECTOR3& _cursorPosition_3D)
{
	if (currentCoolTime_ != 0)
	{
		return false;
	}

	if ((currentBehave_ == Behave::NEUTRAL || currentBehave_ == Behave::RUN) == false)
	{
		return false;
	}

	if (controller_->IsButtonPress(Controller::Key::BREAK) == false)
	{
		return false;
	}

	return true;
}

void Player::Run()
{
	D3DXVECTOR3 moveValue(0, 0, 0);

	// culcurate direction vector
	moveValue += camera_->GetVecX() * controller_->GetAxis(Controller::Key::RUN).x;
	moveValue += camera_->GetVecZ() * controller_->GetAxis(Controller::Key::RUN).y;
	D3DXVec3Normalize(&moveValue, &moveValue);

	// add move power
	moveValue *= runSpeed_;

	startFront_ = GetFront();
	endFront_ = GetPosition() + moveValue - GetPosition();
	D3DXVec3Normalize(&endFront_, &endFront_);

	// rotate
	countTime_Rotate_ = 0;
	startRotate_ = GetRotate();
	endRotate_ = GetRotateToPosition(GetPosition() + moveValue);

	// move position
	Unit::MovePosition(moveValue);

	// setting current state
	SetBehave(Behave::RUN);

	#ifdef _DEBUG
		OX::DebugFont::print(0, 20, 0xff00ff00, "PlayerPos : (%f, %f, %f)", GetPosition().x, GetPosition().y, GetPosition().z);
		OX::DebugFont::print(0, 40, 0xff00ff00, "moveValue : (%f, %f, %f)", moveValue.x, moveValue.y, moveValue.z);
	#endif
}

void Player::Avoid()
{
	moveVector_ = D3DXVECTOR3(0, 0, 0);

	// culcurate avoid vector
	if (controller_->IsButtonPress(Controller::Key::RUN) == true)
	{
		moveVector_ += camera_->GetVecX() * controller_->GetAxis(Controller::Key::RUN).x;
		moveVector_ += camera_->GetVecZ() * controller_->GetAxis(Controller::Key::RUN).y;
		D3DXVec3Normalize(&moveVector_, &moveVector_);
	}
	else
	{
		moveVector_ = endFront_;
	}

	currentSpeed_ = avoidPower_;

	SetRotateToPosition(GetPosition() + moveVector_ * currentSpeed_);

	SetBehave(Behave::AVOID);

	SetCoolTime(coolTimeAvoid_);

	// clear draw line
	if (drawLine_ != nullptr)
	{
		drawLine_->ClearParameter_List(drawLine_);
	}

}

void Player::HeavyAvoid()
{
	moveVector_ = D3DXVECTOR3(0, 0, 0);

	// culcurate avoid vector
	if (controller_->IsButtonPress(Controller::Key::RUN) == true)
	{
		moveVector_ += camera_->GetVecX() * controller_->GetAxis(Controller::Key::RUN).x;
		moveVector_ += camera_->GetVecZ() * controller_->GetAxis(Controller::Key::RUN).y;
		D3DXVec3Normalize(&moveVector_, &moveVector_);
	}
	else
	{
		moveVector_ = GetFront();
	}

	currentSpeed_ = heavyAvoidPower_;

	SetRotateToPosition(GetPosition() + moveVector_ * currentSpeed_);

	SetBehave(Behave::HEAVY_AVOID);

	SetCoolTime(coolTimeHeavyAvoid_);

	// clear draw line
	if (drawLine_ != nullptr)
	{
		drawLine_->ClearParameter_List(drawLine_);
	}
}

void Player::Ready_StraightShot()
{
	SetRotateToPosition(cursorPosition3D_);

	SetBehave(Behave::READY_STRAIGHT_SHOT);
}

void Player::Attack_StraightShot()
{
	SetRotateToPosition(cursorPosition3D_);

	StraightShot::SetStraightShot(*this);

	SetBehave(Behave::STRAIGHT_SHOT);

	SetCoolTime(coolTimeStraightShot_);
}

void Player::Cancel_StraightShot()
{
	SetBehave(Behave::NEUTRAL);
}

void Player::Ready_DrawShot()
{
	drawLine_ = DrawLine::SetStartLine(cursorPosition3D_, *this);

	drawLine_currentLength_ = 0;

	drawLine_preLength_ = drawLine_currentLength_;

	SetBehave(Player::Behave::READY_DRAW_SHOT);
}

void Player::Create_DrawLine(const D3DXVECTOR3& _cursorPosition_3D)
{
	D3DXVECTOR3 nextRelayPosition = _cursorPosition_3D;
	D3DXVECTOR3 preRelayPosition = drawLine_->GetTailPosition();

	if (IsOutOfRange(GetPosition(), nextRelayPosition, drawShotRange_) == true)
	{
		nextRelayPosition = WithinRange(GetPosition(), nextRelayPosition, drawShotRange_);
	}

	if (IsOutOfRange(preRelayPosition, nextRelayPosition, kThresholdLength_AddDraw_) == true)
	{
		drawLine_currentLength_ += sqrtf(Distance3D(nextRelayPosition, preRelayPosition));

		if (IsOutOfLength(drawLine_currentLength_, drawShotLength_) == true)
		{
			nextRelayPosition = FindEndPosition(drawLine_preLength_, drawShotLength_, preRelayPosition, nextRelayPosition);
			DrawLine::SetViaPoint(nextRelayPosition, drawLine_);
			Attack_DrawShot();
		}
		else
		{
			DrawLine::SetViaPoint(nextRelayPosition, drawLine_);
		}

		drawLine_preLength_ = drawLine_currentLength_;

	}

//#ifdef _DEBUG
//	// デバック表示
//	OX::DebugFont::print(0, 20, 0xff00ff00, "PlayerPos.x         : %f", BasePoint::GetPosition().x);
//	OX::DebugFont::print(0, 40, 0xff00ff00, "PlayerPos.y         : %f", BasePoint::GetPosition().y);
//	OX::DebugFont::print(0, 60, 0xff00ff00, "PlayerPos.z         : %f", BasePoint::GetPosition().z);
//	OX::DebugFont::print(0, 80, 0xff00ff00, "mousePos.x          : %f", mousePos.x);
//	OX::DebugFont::print(0, 100, 0xff00ff00, "mousePos.y          : %f", mousePos.y);
//	OX::DebugFont::print(0, 120, 0xff00ff00, "mousePos.z          : %f", mousePos.z);
//	OX::DebugFont::print(0, 140, 0xff00ff00, "m_drawLine_PrePos.x : %f", m_DL_PrePos.x);
//	OX::DebugFont::print(0, 160, 0xff00ff00, "m_drawLine_PrePos.y : %f", m_DL_PrePos.y);
//	OX::DebugFont::print(0, 180, 0xff00ff00, "m_drawLine_PrePos.z : %f", m_DL_PrePos.z);
//	OX::DebugFont::print(0, 200, 0xff00ff00, "m_drawLine_Length   : %f", m_DL_Length);
//#endif
//
}

void Player::Attack_DrawShot()
{
	DrawLine::SetDrawShot(drawLine_);

	SetRotateToPosition(drawLine_->GetNextPointer_DrawLine()->GetPosition());

	SetBehave(Behave::DRAW_SHOT);

	SetCoolTime(coolTimeDrawShot_);
}

void Player::Cancel_DrawShot()
{
	SetBehave(Behave::NEUTRAL);

	drawLine_->ClearParameter_List(drawLine_);
}

void Player::Break_BasePoint()
{
	Tower* tower = CollisionCursorToBasePoint();

	if (tower != nullptr)
	{
		D3DXVECTOR3 front(0, 0, 0);

		front = tower->GetPosition();

		tower->ReceiveDamage(breakPower_, this);

		SetBehave(Behave::BREAK_BASEPOINT);

		SetRotateToPosition(front);

		SetCoolTime(coolTimeBreakBasePoint_);
	}
}

void Player::Move()
{
	if (IsRun() == true)
	{
		Run();
	}

	if (IsAvoid() == true)
	{
		Avoid();
	}

	if (IsHeavyAvoid() == true)
	{
		HeavyAvoid();
	}

	Inirtia();

}

void Player::StraightShot()
{
	if (IsReady_StraightShot(cursorPosition3D_) == true)
	{
		Ready_StraightShot();
	}
	else if (IsAttack_StraightShot(cursorPosition3D_) == true)
	{
		Attack_StraightShot();
	}
	else if (IsCancel_StraightShot(cursorPosition3D_) == true)
	{
		Cancel_StraightShot();
	}
}

void Player::DrawShot()
{
	if (IsReady_DrawShot(cursorPosition3D_) == true)
	{
		Ready_DrawShot();
	}
	else if (IsCreate_DrawLine() == true)
	{
		Create_DrawLine(cursorPosition3D_);
	}
	else if (IsAttack_DrawShot(cursorPosition3D_) == true)
	{
		Attack_DrawShot();
	}
	else if (IsCancel_DrawShot(cursorPosition3D_) == true)
	{
		Cancel_DrawShot();
	}
}

void Player::Break()
{
	if (IsBreakBasePoint(cursorPosition3D_) == true)
	{
		Break_BasePoint();
	}
}

void Player::Inirtia()
{
	MoveInirtia();

	RotateInirtia();
}

void Player::MoveInirtia()
{
	if (currentSpeed_ != 0)
	{
		D3DXVECTOR3 move = moveVector_ * currentSpeed_;

		MovePosition(move);

		currentSpeed_ *= kInirtia_;

		if (currentSpeed_ < kMinThresholdSpeed_)
		{
			currentSpeed_ = 0;
		}
	}
}

void Player::RotateInirtia()
{
	D3DXVECTOR3 rotate;
	D3DXVECTOR3 front;

	if (countTime_Rotate_ < 1.0f)
	{
		countTime_Rotate_ += 1.0f / 15;

		if (countTime_Rotate_ >= 1.0f)
		{
			countTime_Rotate_ = 1.0f;
		}

		D3DXVec3Lerp(&rotate, &startRotate_, &endRotate_, countTime_Rotate_ * (2 - countTime_Rotate_));
		SetRotate(rotate);
		
		D3DXVec3Lerp(&front, &startFront_, &endFront_, countTime_Rotate_ * (2 - countTime_Rotate_));
		SetFront(front);
	}

}

void Player::UpdateInverseMatrix()
{
	if (camera_ == nullptr) return;

	D3DXMATRIX vpMat;
	D3DXMatrixIdentity(&vpMat);

	D3DVIEWPORT9 vp;
	Renderer::GetDevice()->GetViewport(&vp);

	vpMat._11 = (float)vp.Width / 2;
	vpMat._22 = -1.0f * (float)(vp.Height / 2);
	vpMat._33 = (float)vp.MaxZ - vp.MinZ;
	vpMat._41 = (float)(vp.X + vp.Width / 2);
	vpMat._42 = (float)(vp.Y + vp.Height / 2);
	vpMat._43 = vp.MinZ;

	D3DXMATRIX inv_proj, inv_view;
	D3DXMatrixInverse(&inverseMatrix_, 0, &vpMat);
	D3DXMatrixInverse(&inv_proj, 0, &camera_->GetMtxProj());
	D3DXMatrixInverse(&inv_view, 0, &camera_->GetMtxView());

	inverseMatrix_ *= inv_proj * inv_view;

}

bool Player::CollisionCursorToPlayer(const D3DXVECTOR3& _cursorPosition_3D)
{
	D3DXVECTOR3 cursorPosition_3D = _cursorPosition_3D;

	return CalcSphereRayCollision(GetRadius(), &GetPosition(), &cursorPosition_3D, &camera_->GetVecZ_UnNormal(), nullptr, nullptr);
}

Tower* Player::CollisionCursorToBasePoint()
{
	Tower* tower = (Tower*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_TOWER);

	if (tower != nullptr)
	{
		for (;;)
		{
			if (tower->GetGroup() != GetGroup())
			{
				if (tower->IsBrowedRange(this) == true)
				{
					POINT cursorPosition2D;
					GetCursorPos(&cursorPosition2D);
					ScreenToClient(GetHWnd(), &cursorPosition2D);

					D3DXVECTOR3 cursolPosition3D;
					cursolPosition3D.x = (float)cursorPosition2D.x;
					cursolPosition3D.y = (float)cursorPosition2D.y;

					for (int i = 9900000; i < 10000000; i++)
					{
						// 【座標の変換】カーソル座標 → 3D座標
						//transScreenToWorld(&mousePos, pDevice, cursorPos.x, cursorPos.y, (1.f / 10000000) * i, &camera_->GetMtxView(), &camera_->GetMtxProj());
						cursolPosition3D.z = (float)(1. / 10000000) * i;
						D3DXVec3TransformCoord(&cursolPosition3D, &cursolPosition3D, &inverseMatrix_);

						// 【当たり判定】カーソルと"HOLD"
						//if (CalcSphereRayCollision(tower->GetHold()->GetRadius(), &tower->GetHold()->GetPosition(), &mousePos, &camera_->GetVecZ_UnNormal(), nullptr, nullptr))
						if (tower->GetObjectCollider()->Collision(cursolPosition3D) == true)
						{
							break;
						}
					}
				}
			}

			tower = (Tower*)tower->GetNextPointer();

			if (tower == nullptr)
			{
				break;
			}
		}
	}

	return tower;
}

void Player::CollisionTower()
{
	Tower* pTower = (Tower*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_TOWER);

	if (pTower == nullptr) return;

	Tower* pCurrent = pTower;
	Tower* pNext    = (Tower*)pTower->GetNextPointer();

	for (;;) {
		if (Collision_SphereToSphere(GetPosition(), GetHalfSize().x, pCurrent->GetPosition(), pCurrent->GetHalfSize().x)) {
			D3DXVECTOR3 vec = GetPosition() - pCurrent->GetPosition();
			D3DXVec3Normalize(&vec, &vec);
			SetPosition(D3DXVECTOR3(pCurrent->GetPosition().x + vec.x * (GetHalfSize().x + pCurrent->GetHalfSize().x),
									0,
									pCurrent->GetPosition().z + vec.z * (GetHalfSize().x + pCurrent->GetHalfSize().x)));

			return;
		}

		pCurrent = pNext;

		if (pCurrent == nullptr)
				return;

		pNext = (Tower*)pCurrent->GetNextPointer();
	}

}

void Player::CollisionCastle()
{
	Castle* pCastle = (Castle*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_CASTLE);

	if (pCastle == nullptr) return;

	Castle* pCurrent = pCastle;
	Castle* pNext = (Castle*)pCastle->GetNextPointer();

	for (;;) {
		if (Collision_SphereToSphere(GetPosition(), GetHalfSize().x, pCurrent->GetPosition(), pCurrent->GetHalfSize().x)) {

			D3DXVECTOR3 vec = GetPosition() - pCurrent->GetPosition();
			D3DXVec3Normalize(&vec, &vec);
			SetPosition(D3DXVECTOR3(pCurrent->GetPosition().x + vec.x * (GetHalfSize().x + pCurrent->GetHalfSize().x),
									0,
									pCurrent->GetPosition().z + vec.z * (GetHalfSize().x + pCurrent->GetHalfSize().x)));

			return;
		}

		pCurrent = pNext;

		if (pCurrent == nullptr) return;

		pNext = (Castle*)pCurrent->GetNextPointer();
	}

}

void Player::CollisionWall()
{
	//Wall* pCurrent = (Wall*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_WALL);

	//if (pCurrent == nullptr) return;

	//Wall* pNext = (Wall*)pCurrent->GetNextPointer();

	//for (int i = 0; i < 4; i++)
	//{
	//	// Left Wall
	//	if (GetPosition().x - GetRadius() < pCurrent->GetPosition_RIGHT().x)
	//	{
	//		D3DXVECTOR3 pos = GetPosition();
	//		SetPosition(D3DXVECTOR3(pCurrent->GetPosition_RIGHT().x + GetRadius(), pos.y, pos.z));

	//		return;
	//	}

	//	pCurrent = pNext;
	//	pNext = (Wall*)pCurrent->GetNextPointer();

	//	// Right Wall
	//	if (GetPosition().x + GetRadius() > pCurrent->GetPosition_LEFT().x)
	//	{
	//		D3DXVECTOR3 pos = GetPosition();
	//		SetPosition(D3DXVECTOR3(pCurrent->GetPosition_LEFT().x - GetRadius(), pos.y, pos.z));

	//		return;
	//	}

	//	pCurrent = pNext;
	//	pNext = (Wall*)pCurrent->GetNextPointer();


	//	// Front Wall
	//	if (GetPosition().z - GetRadius() < pCurrent->GetPosition_BACK().z)
	//	{
	//		D3DXVECTOR3 pos = GetPosition();
	//		SetPosition(D3DXVECTOR3(pos.x, pos.y, pCurrent->GetPosition_BACK().z + GetRadius()));

	//		return;
	//	}

	//	pCurrent = pNext;
	//	pNext = (Wall*)pCurrent->GetNextPointer();

	//	// Back Wall
	//	if (GetPosition().z + GetRadius() > pCurrent->GetPosition_FRONT().z)
	//	{
	//		D3DXVECTOR3 pos = GetPosition();
	//		SetPosition(D3DXVECTOR3(pos.x, pos.y, pCurrent->GetPosition_FRONT().z - GetRadius()));

	//		return;
	//	}
	//	else
	//		return;

	//	//D3DXVECTOR3 collisionPoint;

	//	//if (CalcParticlePlaneCollision(GetRadius(), &prePosition_, &GetPosition(), &pCurrent->GetNormal_RIGHT(), &pCurrent->GetPosition_RIGHT(), nullptr, &collisionPoint))
	//	//{
	//	//	D3DXVECTOR3 moveVec = GetPosition() - prePosition_;
	//	//	D3DXVECTOR3 vec;
	//	//	calcWallScratchVector(&vec, moveVec, pCurrent->GetNormal_RIGHT());

	//	//	D3DXVECTOR3 vec = GetPosition() - pCurrent->GetPosition();
	//	//	D3DXVec3Normalize(&vec, &vec);
	//	//	SetPosition(D3DXVECTOR3(pCurrent->GetPosition().x + vec.x * (GetHalfSize().x + pCurrent->GetHalfSize().x),
	//	//							0,
	//	//							pCurrent->GetPosition().z + vec.z * (GetHalfSize().x + pCurrent->GetHalfSize().x)));

	//	//	return;
	//	//}

	//	//if (pNext == nullptr) return;

	//	//pCurrent = pNext;
	//	//pNext = (Wall*)pCurrent->GetNextPointer();
	//}

	//return;
}

void Player::CollisionField()
{
	float fieldHeight = MainGame::GetField()->GetHeight(GetPosition());

	if (GetPosition().y <= fieldHeight)
	{
		SetPosition(D3DXVECTOR3(GetPosition().x, fieldHeight, GetPosition().z));
	}
}

std::string Player::FindModelPass(const Player::Character& character)
{
	if (character >= Player::Character::MAX)
	{
		const char* errorMessage = "キャラクターが見つかりませんでした";
		const char* errorTitle = "Player::FindModelPass";
		_MSGERROR(errorMessage, errorTitle);
		return nullptr;
	}

	return modelPassList_.at(character);
}

void Player::AroundParts()
{
	AddPart("head", "data/model/MainGame/head01.x");
	AddPart("body", "data/model/MainGame/body01.x");
	AddPart("leftArm", "data/model/MainGame/arm_l01.x");
	AddPart("rightArm", "data/model/MainGame/arm_r01.x");
	AddPart("leftLeg", "data/model/MainGame/leg_l01.x");
	AddPart("rightLeg", "data/model/MainGame/leg_r01.x");

	SetParent("body", nullptr);
	SetParent("head", "body");
	SetParent("leftArm", "body");
	SetParent("rightArm", "body");
	SetParent("leftLeg", "body");
	SetParent("rightLeg", "body");

	// body
	std::string key = "body";
	Part* parent = GetPart(key)->GetParent();
	D3DXVECTOR3 position = D3DXVECTOR3(0, 20, 0);
	D3DXVECTOR3 rotate = D3DXVECTOR3(0, 0, 0);
	SetPartPosition(key, position);
	SetPartRotate(key, rotate);

	// head
	key = "head";
	parent = GetPart(key)->GetParent();
	position = D3DXVECTOR3(0, parent->GetSize().y * 0.75f, GetPart(key)->GetHalfSize().z * 0.5f);
	SetPartPosition(key, position);
	SetPartRotate(key, rotate);

	// leftArm
	key = "leftArm";
	parent = GetPart(key)->GetParent();
	position = D3DXVECTOR3(-parent->GetHalfSize().x, parent->GetHalfSize().y, 0);
	rotate = D3DXVECTOR3(0, 0, 0.9f);
	SetPartPosition(key, position);
	SetPartRotate(key, rotate);

	// rightArm
	key = "rightArm";
	parent = GetPart(key)->GetParent();
	position = D3DXVECTOR3(parent->GetHalfSize().x, parent->GetHalfSize().y, 0);
	rotate = D3DXVECTOR3(0, 0, 0.81f);
	SetPartPosition(key, position);
	SetPartRotate(key, rotate);

	// leftLeg
	key = "leftLeg";
	parent = GetPart(key)->GetParent();
	position = D3DXVECTOR3(-parent->GetHalfSize().x, -parent->GetHalfSize().y * 0.5f, 0);
	rotate = D3DXVECTOR3(0, 0, 1.56f);
	SetPartPosition(key, position);
	SetPartRotate(key, rotate);

	// rightLeg
	key = "rightLeg";
	parent = GetPart(key)->GetParent();
	position = D3DXVECTOR3(parent->GetHalfSize().x, -parent->GetHalfSize().y * 0.5f, 0);
	rotate = D3DXVECTOR3(0, 0, 0.33f);
	SetPartPosition(key, position);
	SetPartRotate(key, rotate);


	return;

	//m_Part[0] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_BODY);
	//m_Part[1] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_HEAD);
	//m_Part[2] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_RARM);
	//m_Part[3] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_RARM_EDGE);
	//m_Part[4] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_LARM);
	//m_Part[5] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_LARM_EDGE);
	//m_Part[6] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_RLEG);
	//m_Part[7] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_RLEG_EDGE);
	//m_Part[8] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_LLEG);
	//m_Part[9] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_LLEG_EDGE);

	//m_Part[0]->SetParent(nullptr);		// BODY
	//m_Part[1]->SetParent(m_Part[0]);	// HEAD
	//m_Part[2]->SetParent(m_Part[0]);	// RARM
	//m_Part[3]->SetParent(m_Part[2]);	// REDGE
	//m_Part[4]->SetParent(m_Part[0]);	// LARM
	//m_Part[5]->SetParent(m_Part[4]);	// LEDGE
	//m_Part[6]->SetParent(m_Part[0]);	// RLEG
	//m_Part[7]->SetParent(m_Part[6]);	// REDGE
	//m_Part[8]->SetParent(m_Part[0]);	// LLEG
	//m_Part[9]->SetParent(m_Part[8]);	// LEDGE

	//									// BODY
	//D3DXVECTOR3 pos = D3DXVECTOR3(0, 20, 0);
	//D3DXVECTOR3 rotate = D3DXVECTOR3(0, 0, 0);
	//m_Part[0]->SetPosition(pos);
	//m_Part[0]->SetRotate(rotate);

	//// HEAD
	//pos = D3DXVECTOR3(0, m_Part[1]->GetParent()->GetSize().y * 0.75f, m_Part[1]->GetHalfSize().z * 0.5f);
	//m_Part[1]->SetPosition(pos);

	//// RARM
	//pos = D3DXVECTOR3(m_Part[2]->GetParent()->GetHalfSize().x, m_Part[2]->GetParent()->GetHalfSize().y, 0);
	////rotate = D3DXVECTOR3(0, 0, 0.81f);
	//m_Part[2]->SetPosition(pos);
	//m_Part[2]->SetRotate(rotate);

	//// REDGE
	//pos = D3DXVECTOR3(m_Part[3]->GetParent()->GetSize().x, 0, 0);
	////rotate = D3DXVECTOR3(0, 0, 1.68f);
	//m_Part[3]->SetPosition(pos);
	//m_Part[3]->SetRotate(rotate);

	//// LARM
	//pos = D3DXVECTOR3(-m_Part[4]->GetParent()->GetHalfSize().x, m_Part[4]->GetParent()->GetHalfSize().y, 0);
	////rotate = D3DXVECTOR3(0, 0, 0.9f);
	//m_Part[4]->SetPosition(pos);
	//m_Part[4]->SetRotate(rotate);

	//// LEDGE
	//pos = D3DXVECTOR3(-m_Part[5]->GetParent()->GetSize().x, 0, 0);
	////rotate = D3DXVECTOR3(0, 0, 1.56f);
	//m_Part[5]->SetPosition(pos);
	//m_Part[5]->SetRotate(rotate);

	//// RLEG
	//pos = D3DXVECTOR3(m_Part[6]->GetParent()->GetHalfSize().x, -m_Part[6]->GetParent()->GetHalfSize().y * 0.5f, 0);
	////rotate = D3DXVECTOR3(0, 0, 0.33f);
	//m_Part[6]->SetPosition(pos);
	//m_Part[6]->SetRotate(rotate);

	//// REDGE
	//pos = D3DXVECTOR3(0, -m_Part[7]->GetParent()->GetSize().y, 0);
	//m_Part[7]->SetPosition(pos);

	//// LLEG
	//pos = D3DXVECTOR3(-m_Part[8]->GetParent()->GetHalfSize().x, -m_Part[8]->GetParent()->GetHalfSize().y * 0.5f, 0);
	////rotate = D3DXVECTOR3(0, 0, -0.15f);
	//m_Part[8]->SetPosition(pos);
	//m_Part[8]->SetRotate(rotate);

	//// LEDGE
	//pos = D3DXVECTOR3(0, -m_Part[9]->GetParent()->GetSize().y, 0);
	//m_Part[9]->SetPosition(pos);


	//// Initialize Key Frame
	//g_KeyFrameWalk[0].Frame = 20;
	//g_KeyFrameWalk[1].Frame = 20;

	//for (int j = 0; j < KEY_MAX; j++) {
	//	for (int i = 0; i < 10; i++) {
	//		g_KeyFrameWalk[j].Key[i].Position = m_Part[i]->GetPosition();
	//		//g_KeyFrameWalk[j].Key[i].Rotation = m_Part[i]->GetRotate();
	//	}
	//}

	//m_KeyFrame = g_KeyFrameWalk;
	//m_Key = 0;
	//m_Frame = 0;
}

D3DXVECTOR3 Player::FindCursorPosition_3D()
{
	POINT cursorPosition_2D;
	D3DXVECTOR3 cursorPosition_3D;
	GetCursorPos(&cursorPosition_2D);
	ScreenToClient(GameManager::GetWindowHandle(), &cursorPosition_2D);

	for (int i = 9960000; i < 10000000; i++)
	{
		cursorPosition_3D.x = (float)cursorPosition_2D.x;
		cursorPosition_3D.y = (float)cursorPosition_2D.y;
		cursorPosition_3D.z = (float)(1.0f / 10000000) * i;
		D3DXVec3TransformCoord(&cursorPosition_3D, &cursorPosition_3D, &inverseMatrix_);

		if (cursorPosition_3D.y < 0)
		{
			cursorPosition_3D.y = 0;
			break;
		}
	}

	return cursorPosition_3D;
}

bool Player::IsOutOfRange(const D3DXVECTOR3& _centerPosition, const D3DXVECTOR3& _nextPosition, float _range)
{
	return Distance3D(_nextPosition, _centerPosition) > _range * _range ? true : false;
}

D3DXVECTOR3 Player::WithinRange(const D3DXVECTOR3& _centerPosition, const D3DXVECTOR3& _comparePosition, float _range)
{
	D3DXVECTOR3 nextRelayPosition(0, 0, 0);

	// find vector
	D3DXVECTOR3 vector = _comparePosition - _centerPosition;
	D3DXVec3Normalize(&vector, &vector);

	// add power
	vector *= _range;

	// find position
	nextRelayPosition = vector + _centerPosition;

	return nextRelayPosition;
}

bool Player::IsOutOfLength(const float& _currentLength, const float& _maxLength)
{
	return _currentLength >= _maxLength ? true : false;
}

D3DXVECTOR3 Player::FindEndPosition(const float& _preLength, const float& _maxLength, const D3DXVECTOR3& _preRelayPosition, const D3DXVECTOR3& _nextRelayPosition)
{
	D3DXVECTOR3 endPosition(0, 0, 0);

	// find vector
	D3DXVECTOR3 drawLineVector = _nextRelayPosition - _preRelayPosition;
	D3DXVec3Normalize(&drawLineVector, &drawLineVector);

	// find power
	float addLength = sqrtf(_maxLength - _preLength);

	// find end position
	endPosition = drawLineVector * addLength + _preRelayPosition;

	return endPosition;
}

void Player::InitParamater(Character _character)
{
	switch (_character)
	{
		case Character::FIGHTER:
			// behave
			currentBehave_ = Behave::NEUTRAL;

			// speed
			currentSpeed_    = 0.0f;
			runSpeed_        = 2.0f;
			avoidPower_      = 15.0f;
			heavyAvoidPower_ = 10.0f;

			// cool time
			currentCoolTime_        = 0;
			coolTimeAvoid_          = 45;
			coolTimeHeavyAvoid_     = 120;
			coolTimeStraightShot_   = 30;
			coolTimeDrawShot_       = 75;
			coolTimeBreakBasePoint_ = 90;

			// hp
			maxHp_ = 80.0f;
			currentHp_ = maxHp_;

			// mp
			maxMp_ = 45.0f;
			currentMp_ = maxMp_;

			// straight shot
			straightShotDamage_ = 100.0f;
			straightShotSpeed_  = 5.0f;
			straightShotRange_  = 50.0f;

			// draw shot
			drawShotDamage_ = 150.0f;
			drawShotSpeed_  = 3.0f;
			drawShotRange_  = 150.0f;
			drawShotLength_ = 300.0f;

			// break power
			breakPower_ = 5.0f;
			break;

		case Character::ATTACKER:
			break;

		case Character::SUPPORTER:
			break;

	}


}
