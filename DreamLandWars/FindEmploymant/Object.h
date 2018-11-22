// author : yusuke seki
// data   : 20181110
#ifndef OBJECT_H_
#define OBJECT_H_

#include "main.h"

class Object
{
public:
	union Color
	{
		unsigned int color;
		unsigned char rgba[4];
	};

	//struct Color
	//{
	//	int r;
	//	int g;
	//	int b;
	//	int a;
	//};

	enum TYPE
	{ // ��ށi�`�揇�����݁j
		TYPE_GENERATOR,
		TYPE_COMMANDER,					// �w�����i���W�̂݁j
		TYPE_3D_FIELD,					// �t�B�[���h
		TYPE_MODEL,						// �����f��
		TYPE_MODEL_PLAYER,				// �v���C���[
		TYPE_MODEL_SOLDIER,				// ���m
		TYPE_MODEL_CASTLE,				// ��
		TYPE_MODEL_TOWER,				// ��
		TYPE_MODEL_SKYDOME,				// �X�J�C�h�[��
		TYPE_3D_SKYBOX,					// �X�J�C�{�b�N�X
		TYPE_3D,						// ��3D�|���S��
		TYPE_3D_CUBE,					// �L���[�u
		TYPE_3D_WALL,					// ��
		TYPE_3D_DRAWRANGE,				// DS�˒��͈�
		TYPE_3D_DRAWLINE,				// �`���
		TYPE_3D_SOLDIERBULLET,			// ���m�e
		TYPE_3D_STRAIGHTSHOT,			// �X�g���[�g�V���b�g
		TYPE_3D_DRAWSHOT,				// �h���[�V���b�g
		TYPE_3D_BILLBOARD_LIFEGAUGE,	// �̗̓Q�[�W
		TYPE_3D_BILLBOARD_ICON,			// �A�C�R��
		TYPE_3D_BILLBOARD_HOLD,			// "HOLD"
		TYPE_3D_BILLBOARD,				// �r���{�[�h
		TYPE_3D_BULLET,					// �o���b�g�i3D�j
		TYPE_3D_BILLBOARD_BULLET,		// �o���b�g�i�r���{�[�h�j
		TYPE_SOLDIERGAUGE,
		TYPE_3D_BILLBOARD_EFFECT,		// �G�t�F�N�g�i�r���{�[�h�j
		TYPE_3D_GAMETITLE,				// �Q�[���^�C�g��
		TYPE_3D_TOUCHSCREEN,			// �^�b�`�X�N���[��
		TYPE_2D,						// ��2D�|���S��
		TYPE_2D_FRAMEBORDER,			// �g��
		COLLIDER,

		TYPE_MAX,	// �� ��ނ̍ő吔
	};

	enum GROUP
	{ // �w�c
		GROUP_NONE, // ������
		GROUP_A,	// A
		GROUP_B,	// B
	};

public:
	Object();
	Object(Object::TYPE type);
	virtual ~Object() {}

	// part of manager
	static  void UpdateAll();
	static  void DrawAll();
	static  void ReleaseAll();
	static Object* GetLDATA_HEAD(Object::TYPE type)
	{
		return headObj_[type];
	}

	// part of manager
	void SetNextPointer(Object* _nextObj)
	{
		nextObj_ = _nextObj;
	}

	void SetPrevPointer(Object* _prevObj)
	{
		prevObj_ = _prevObj;
	}

	Object* GetNextPointer()
	{
		return nextObj_;
	}

	Object* GetPrevPointer()
	{
		return prevObj_;
	}

	// part of one object
	virtual void Init(void) {}
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void);

	virtual void SetPosition(const D3DXVECTOR3& position)
	{
		position_ = position;
	}

	virtual void MovePosition(const D3DXVECTOR3& movePosition)
	{
		position_ += movePosition;
	}

	D3DXVECTOR3& GetPosition()
	{
		return position_;
	}

	Object::TYPE GetType()
	{
		return type_;
	}

	virtual void SetGroup(const Object::GROUP& group)
	{
		group_ = group;
	}

	Object::GROUP GetGroup()
	{
		return group_;
	}

	virtual void SetActive(const bool& _isActive)
	{
		isActive_ = _isActive;
	}

	bool GetActive()
	{
		return isActive_;
	}

#ifdef _DEBUG
	static void SetIsDrawCollider(const bool& _isDraw)
	{
		isDrawCollider_ = _isDraw;
	}

	static bool GetIsDrawCollider()
	{
		return isDrawCollider_;
	}
#endif

private:
	static Object* headObj_[Object::TYPE_MAX];
	static Object* tailObj_[Object::TYPE_MAX];
	Object* nextObj_;
	Object* prevObj_;

	D3DXVECTOR3 position_;
	Object::TYPE type_;
	Object::GROUP group_;
	bool isActive_;

#ifdef _DEBUG
	static bool isDrawCollider_;
#endif

};

#endif
