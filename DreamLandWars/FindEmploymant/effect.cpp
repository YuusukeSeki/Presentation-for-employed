////*****************************************************************************
////
////		�G�t�F�N�g
////												Autohr	:	Yusuke Seki
////*****************************************************************************
//#include "effect.h"
//#include "renderer.h"
//#include "MainGame.h"
//#include "camera.h"
//
//
//
////-----------------------------------------------------------------------------
////	�R���X�g���N�^
////-----------------------------------------------------------------------------
//Effect::Effect(int priority, TYPE type) :Scene(priority, type)
//{
//	m_bUse = false;
//
//	m_pVtxBuff = nullptr;
//	m_pTexture = nullptr;
//
//}
//
//
////-----------------------------------------------------------------------------
////	�f�X�g���N�^
////-----------------------------------------------------------------------------
//Effect::~Effect()
//{
//	Effect::Uninit();
//}
//
//
////-----------------------------------------------------------------------------
////	����
////-----------------------------------------------------------------------------
//Effect* Effect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
//{
//	Effect* pEffect = new Effect(1, TYPE_3D);
//	pEffect->Init(pos, size, rot, scl);
//
//	return pEffect;
//}
//
//
////-----------------------------------------------------------------------------
////	������
////-----------------------------------------------------------------------------
//HRESULT Effect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
//{
//	D3DXMATRIX mtxTrans, mtxRot, mtxScall;
//	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
//	VERTEX_3D* pVtx;
//	D3DXVECTOR3 halfSize = size * 0.5f;
//
//
//	// �����o�ϐ��ݒ�
//	SetPos(pos);
//	m_size = size;
//	m_rot = rot;
//	m_scl = scl;
//	m_color.color = 0xffffffff;
//
//	m_radius = sqrtf((m_size.x * 0.5f) * (m_size.x * 0.5f) + (m_size.y * 0.5f) * (m_size.y * 0.5f));
//	m_angle = atan2f(m_size.y, m_size.x);
//
//	m_life = 0;
//	m_radiusValue = m_colorAlphaValue = 0;
//
//	m_move = D3DXVECTOR3(0, 0, 0);
//
//	m_bUse = false;
//
//
//
//	// ���_�o�b�t�@�̈�̐���
//	if (m_pVtxBuff == nullptr) {
//		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
//			D3DPOOL_MANAGED, &m_pVtxBuff, nullptr))) {
//			MessageBox(nullptr, "���_�o�b�t�@�������s", "Billboard::Init", MB_OK);
//		}
//	}
//
//
//	// VRAM�̉��z�A�h���X�擾
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// ���_���W�̐ݒ�
//	pVtx[0].pos = D3DXVECTOR3(-halfSize.x,  halfSize.y,  halfSize.z);	// ����
//	pVtx[1].pos = D3DXVECTOR3( halfSize.x,  halfSize.y,  halfSize.z);	// �E��
//	pVtx[2].pos = D3DXVECTOR3(-halfSize.x, -halfSize.y, -halfSize.z);	// ����
//	pVtx[3].pos = D3DXVECTOR3( halfSize.x, -halfSize.y, -halfSize.z);	// �E��
//
//	// �@���̐ݒ�
//	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//
//	// ���_�F�̐ݒ�
//	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//
//	// �e�N�X�`��UV�l�̐ݒ�
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	// VRAM�̉��z�A�h���X���
//	m_pVtxBuff->Unlock();
//
//
//	// �e�s��v�Z
//	D3DXMatrixScaling(&mtxScall, m_scl.x, m_scl.y, m_scl.z);
//	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//
//	// �s��̍���
//	D3DXMatrixIdentity(&m_mtxWorld);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
//
//
//	return S_OK;
//}
//
//
////-----------------------------------------------------------------------------
////	�I��
////-----------------------------------------------------------------------------
//void Effect::Uninit(void)
//{
//	// ���_�o�b�t�@�̔j��
//	if (m_pVtxBuff != nullptr) {
//		m_pVtxBuff->Release();
//		m_pVtxBuff = nullptr;
//	}
//}
//
//
////-----------------------------------------------------------------------------
////	�X�V
////-----------------------------------------------------------------------------
//void Effect::Update(void)
//{
//	// ���g�p�ŏ�������
//	if (!m_bUse) return;
//
//
//
//	// �ʒu�̍X�V
//	this->MovePos(m_move);
//
//	// �F�̍X�V
//	m_color.rgba[3] = m_color.rgba[3] - 1 <= 0 ? 0 : m_color.rgba[3] - 1;
//	m_color.rgba[2] = m_color.rgba[2] - 1 <= 0 ? 0 : m_color.rgba[2] - 1;
//	m_color.rgba[1] = m_color.rgba[1] - 1 <= 0 ? 0 : m_color.rgba[1] - 1;
//	m_color.rgba[0] = m_color.rgba[0] -	m_colorAlphaValue * 2 <= 0 ? 0 : (unsigned char)(m_color.rgba[0] - m_colorAlphaValue * 2);
//
//	// �傫���̏k��
//	m_radius = m_radius - m_radiusValue < 0 ? 0 : m_radius - m_radiusValue;
//
//	// �����̌���
//	m_life--;
//
//
//	// VRAM�̉��z�A�h���X�擾
//	VERTEX_3D* pVtx;
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// ���_���W�̐ݒ�
//	pVtx[0].pos = D3DXVECTOR3(-m_radius,  m_radius, 0);	// ����
//	pVtx[1].pos = D3DXVECTOR3( m_radius,  m_radius, 0);	// �E��
//	pVtx[2].pos = D3DXVECTOR3(-m_radius, -m_radius, 0);	// ����
//	pVtx[3].pos = D3DXVECTOR3( m_radius, -m_radius, 0);	// �E��
//
//	// ���_�F�̐ݒ�
//	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//
//	// VRAM�̉��z�A�h���X���
//	m_pVtxBuff->Unlock();
//
//
//	// �������s������A�g�p�t���OOFF
//	if (m_life <= 0) m_bUse = false;
//
//}
//
//
////-----------------------------------------------------------------------------
////	�`��
////-----------------------------------------------------------------------------
//void Effect::Draw(void)
//{
//	// ���g�p�ŏ�������
//	if (!m_bUse) return;
//
//
//
//	D3DXVECTOR3 pos = GetPos();
//	D3DXMATRIX mtxTrans, mtxRot, mtxScall, mtxView, mtxViewInverse;
//	Camera* pCamera = MainGame::GetCamera();
//	mtxView = pCamera->GetMtxView();
//
//
//	// �f�o�C�X�̎擾
//	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
//
//	// ���[���h�s������߂�
//	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//	D3DXMatrixScaling(&mtxScall, m_scl.x, m_scl.y, m_scl.z);
//	D3DXMatrixTranspose(&mtxViewInverse, &mtxView);
//	mtxViewInverse._14 = mtxViewInverse._24 = mtxViewInverse._34 = 0.0f;
//
//	D3DXMatrixIdentity(&m_mtxWorld);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxViewInverse);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
//
//	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
//
//	// �X�g���[���̐ݒ�
//	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
//
//	// ���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// �e�N�X�`���\��t��
//	pDevice->SetTexture(0, m_pTexture->GetTexture());
//
//	// ���e�X�g�̐ݒ�
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�n�m
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);				// ����l
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���Z�q
//
//	// ���C�g�𖳌��ɂ���
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// �u�����h���@�ύX
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	// Z�o�b�t�@���������܂Ȃ�
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//	// �`��
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//
//	// Z�o�b�t�@����������
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//
//	// �`����@��߂�
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	// ���C�g��L���ɖ߂�
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//
//	// ���e�X�g�I��
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//
//
//}
//
//
////-----------------------------------------------------------------------------
////	�ݒ�
////-----------------------------------------------------------------------------
//bool Effect::SetEffect(D3DXVECTOR3 pos)
//{
//	if (m_bUse) return false;
//
//	pos.x += rand() % 100 - 50;
//	pos.y += rand() % 100 - 20;
//
//	this->SetPos(pos);
//
//	m_color.color = 0xff8040ff;
//
//	m_move.x = 0;
//	m_move.y = (float)(rand() % 10001 / 2000);
//	m_radius = 120.0f;
//	m_angle = atan2f(m_radius, m_radius);
//	m_colorAlphaValue = 255 / ELASE_TIME;
//	m_radiusValue = m_radius / ELASE_TIME;
//	m_life = ELASE_TIME;
//
//	m_bUse = true;
//
//
//	return true;
//
//}
//
