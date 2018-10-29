////*****************************************************************************
////
////		�Q�[�W
////													Author : Yusuke Seki
////*****************************************************************************
//#include "Gauge.h"
//#include "ObjectBillboard.h"
//
//
//// �R���X�g���N�^
//Gauge::Gauge()
//{
//	_bDraw     = false;	// �`�悷��H
//	_value      = 0;		// ���݂̃Q�[�W��
//	_initWidth = 0;		// �Q�[�W�̏�����
//
//	_pGauge      = nullptr;	// �Q�[�W
//	_pBackGround = nullptr;	// �Q�[�W�̔w�i
//
//}
//
//// �f�X�g���N�^
//Gauge::~Gauge()
//{
//	Uninit();
//}
//
//// ��������
//// position : ���W
//// size : �傫��
//Gauge* Gauge::Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
//{
//	Gauge* pGauge = new Gauge();
//	pGauge->Init(position, size, pCamera);
//
//	return pGauge;
//}
//
//// ����������
//void Gauge::Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
//{
//	D3DXVECTOR3 sizePlus(size * 0.1f);	// �w�i�傫���̕␳�l
//
//	_pBackGround = ObjectBillboard::Create(position, size + sizePlus, pCamera);
//	_pBackGround->SetColor(0x000000a0);
//	_pGauge = ObjectBillboard::Create(position, size, pCamera);
//
//	_initWidth = size.x;
//	_bDraw = false;
//
//}
//
//// �I������
//void Gauge::Uninit(void)
//{
//
//}
//
//// �X�V����
//void Gauge::Update(void)
//{
//
//}
//
//// �������
//void Gauge::Release(void)
//{
//	delete this;
//}
//
//// �Q�[�W�ʂ̑�������
//void Gauge::MoveValue(float moveLife)
//{
//	_value += moveLife;
//
//}
//
//// ���݂̃Q�[�W�ʂ̎擾
//float Gauge::GetValue(void)
//{
//	return _value;
//}
//
//// �`��̗L���X�C�b�`
//void Gauge::SetDraw(bool bDraw)
//{
//	// ���L�|���S���̕`��t���O��؂�ւ���
//	_pGauge->SetDraw(bDraw);
//	_pBackGround->SetDraw(bDraw);
//
//}
//
