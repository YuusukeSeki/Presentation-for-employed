// author : yusuke seki
// data   : 20181116
#include "castle.h"
#include "collision.h"
#include "SoldierGenerator.h"
#include "SoldierCommander.h"
#include "Part.h"

const unsigned int Castle::kNumSoldier_First_ = 9;
const unsigned int Castle::kNumSoldier_Subsequent_ = 9;

Castle::Castle(const Object::TYPE& _type) : BasePoint(_type, kNumSoldier_First_, kNumSoldier_Subsequent_)
{
	leftSoldierGenerator_ = nullptr;
	rightSoldierGenerator_ = nullptr;
}

Castle::~Castle()
{
	Uninit();
}

Castle* Castle::Create(const D3DXVECTOR3& _position, const Object::GROUP& _group
	, RelayPoint* _leftRelayPoint, RelayPoint* _rightRelayPoint)
{
	Castle* castle = new Castle(Object::TYPE::TYPE_MODEL_CASTLE);
	castle->Init(_position, _group, _leftRelayPoint, _rightRelayPoint);

	return castle;
}

void Castle::Init(const D3DXVECTOR3& _position, const Object::GROUP& _group
	, RelayPoint* _leftRelayPoint, RelayPoint* _rightRelayPoint)
{
	BasePoint::Init(_position, _group, _leftRelayPoint);

	AddPart("castle", "data/model/MainGame/castle03.x");

	SetBrowedRange(GetHalfSize().z + 3.5f);

	if (rightSoldierGenerator_ == nullptr)
	{
		rightSoldierGenerator_ = SoldierGenerator::Create(this
			, kNumFrame_CreateSoldierLine_First_, kNumSoldier_First_
			, kNumFrame_CreateSoldierLine_Subsequent_, kNumSoldier_Subsequent_);
	}
	else
	{
		rightSoldierGenerator_->Init(this);
	}

	rightSoldierGenerator_->SetNextRelayPoint(_rightRelayPoint);
	rightSoldierGenerator_->SetIsUpdateTimer(true);

}

void Castle::Uninit(void)
{
	BasePoint::Uninit();
}

void Castle::Update(void)
{
	BasePoint::Update();

	//// ���m�����p�t���[���J�E���^�[����
	//m_frameCounter++;

	//// �t���[���J�E���^�[���K��l�𒴂����畺�m�𐶐�����
	//if (m_frameCounter >= Castle::_INTERVAL_CREATE_SOLDIER) {
	//	D3DXVECTOR3 vecX;
	//	D3DXVec3Cross(&vecX, &vector_LEFT, &D3DXVECTOR3(0, 1, 0));
	//	vecX.y = 0.f;
	//	vecX.z = 0;

	//	D3DXVec3Normalize(&vecX, &vecX);

	//	vecX *= 20;

	//	// �����[��
	//	Commander::SetCommander(GetPosition() + vecX, vector_LEFT, GetGroup());

	//	// �E���[��
	//	Commander::SetCommander(GetPosition() + vecX * -1, vector_RIGHT, GetGroup());

	//	// �J�E���^�[�������߂�
	//	m_frameCounter = 0;

	//}

	//// �����͈͂̉�������
	//{
	//	// �v���C���[���X�g�̐擪�f�[�^���擾
	//	Player* pCurrent = (Player*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_PLAYER);

	//	// Error Command
	//	if (pCurrent == nullptr)
	//		return;

	//	// �G�L���X�g�Ǝ��g�Ƃ̋������K��l�ȓ��ŁA������Ԃ���������
	//	for (;;) {

	//		// �G�L���X�g���ǂ����𔻒�
	//		if (pCurrent->GetGroup() != GetGroup()) {

	//			// �������K��l�ȓ��Ȃ牣����Ԃł��邱�Ƃ���������
	//			if (Distance3D(pCurrent->GetPosition(), GetPosition()) <= m_browRange * m_browRange) {
	//				m_pHold->SetDrawHold(true);
	//			}

	//			// �͈͊O�Ȃ��\��
	//			else {
	//				m_pHold->SetDrawHold(false);
	//			}

	//		}

	//		// ���̃L���X�g��
	//		pCurrent = (Player*)pCurrent->GetNextPointer();

	//		// ���̃L���X�g�����Ȃ���Δ���I��
	//		if (pCurrent == nullptr) {
	//			break;
	//		}

	//	}

	//}


}

void Castle::Draw(void)
{
	BasePoint::Draw();
}

void Castle::ReceiveDamage(const float& _damage, Unit* _unit)
{
	// �`�[���Q�[�W�ƕR�Â��Č��炷
}

//void Castle::BrowCastle(float breakPower)
//{
//	TeamGaugeManager::GetTeamGauge_ENEMY()->MoveLife(-breakPower * CASTLE_RETOUCH_BREAKPOWER);
//}
//
//// �����͈͂Ƃ̓����蔻��
//// position : �ΏۃL���X�g�̈ʒu
//// �y�Ԃ�l�z	true  : �����
////				false : ����Ȃ�
//bool Castle::CollisionBrowRange(D3DXVECTOR3& position)
//{
//	if (Distance3D(GetPosition(), position) <= m_browRange * m_browRange)
//		return true;
//
//	return false;
//
//}
//
//

bool Castle::IsBreak()
{
	return false;
}
