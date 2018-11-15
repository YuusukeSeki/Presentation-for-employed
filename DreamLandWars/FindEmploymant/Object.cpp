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
	// ���߂Đ������ꂽ��ނȂ�A�擪�Ƀ|�C���^�����B
	if (headObj_[type] == nullptr)
	{
		// �w�b�h�̍X�V
		headObj_[type] = this;
		headObj_[type]->nextObj_ = nullptr;
		headObj_[type]->prevObj_ = nullptr;

		// �e�C���̍X�V
		tailObj_[type] = headObj_[type];

		// �O��̃|�C���^�̏�����
		nextObj_ = nullptr;
		prevObj_ = nullptr;

	}
	// �Q�ڈȏ�ɐ������ꂽ��ނȂ�A�����ɒǉ��B
	else
	{
		// �e�C���̎��̃|�C���^��"����"�ɂ���
		tailObj_[type]->nextObj_ = this;

		// �O��̃|�C���^�̏�����
		nextObj_ = nullptr;
		prevObj_ = tailObj_[type];

		// �e�C���̍X�V
		tailObj_[type] = this;

	}

	// �����o�ϐ��̏�����
	position_ = D3DXVECTOR3(0, 0, 0);
	type_ = type;
	group_ = Object::GROUP::GROUP_NONE;
	isActive_ = true;

}


//-----------------------------------------------------------------------------
//	��������F��
//-----------------------------------------------------------------------------
void Object::Release()
{
	// �O��̃I�u�W�F�N�g�������Ă���A�O��̃|�C���^���X�V
	if (nextObj_ != nullptr) nextObj_->prevObj_ = prevObj_;
	if (prevObj_ != nullptr) prevObj_->nextObj_ = nextObj_;

	// �w�b�h�̃|�C���^���X�V
	if (headObj_[type_] == this) headObj_[type_] = nextObj_;

	// �e�C���̃|�C���^���X�V
	if (tailObj_[type_] == this) tailObj_[type_] = prevObj_;

	// �I������
	Uninit();

	// �������̉��
	delete this;

}


//-----------------------------------------------------------------------------
//	�X�V�����F�S��
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
//	�`�揈���F�S��
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
//	��������F�S��
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


