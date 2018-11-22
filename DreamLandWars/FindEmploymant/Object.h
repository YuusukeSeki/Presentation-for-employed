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
	{ // 種類（描画順序込み）
		TYPE_GENERATOR,
		TYPE_COMMANDER,					// 指揮官（座標のみ）
		TYPE_3D_FIELD,					// フィールド
		TYPE_MODEL,						// ★モデル
		TYPE_MODEL_PLAYER,				// プレイヤー
		TYPE_MODEL_SOLDIER,				// 兵士
		TYPE_MODEL_CASTLE,				// 城
		TYPE_MODEL_TOWER,				// 塔
		TYPE_MODEL_SKYDOME,				// スカイドーム
		TYPE_3D_SKYBOX,					// スカイボックス
		TYPE_3D,						// ★3Dポリゴン
		TYPE_3D_CUBE,					// キューブ
		TYPE_3D_WALL,					// 壁
		TYPE_3D_DRAWRANGE,				// DS射程範囲
		TYPE_3D_DRAWLINE,				// 描画線
		TYPE_3D_SOLDIERBULLET,			// 兵士弾
		TYPE_3D_STRAIGHTSHOT,			// ストレートショット
		TYPE_3D_DRAWSHOT,				// ドローショット
		TYPE_3D_BILLBOARD_LIFEGAUGE,	// 体力ゲージ
		TYPE_3D_BILLBOARD_ICON,			// アイコン
		TYPE_3D_BILLBOARD_HOLD,			// "HOLD"
		TYPE_3D_BILLBOARD,				// ビルボード
		TYPE_3D_BULLET,					// バレット（3D）
		TYPE_3D_BILLBOARD_BULLET,		// バレット（ビルボード）
		TYPE_SOLDIERGAUGE,
		TYPE_3D_BILLBOARD_EFFECT,		// エフェクト（ビルボード）
		TYPE_3D_GAMETITLE,				// ゲームタイトル
		TYPE_3D_TOUCHSCREEN,			// タッチスクリーン
		TYPE_2D,						// ★2Dポリゴン
		TYPE_2D_FRAMEBORDER,			// 枠線
		COLLIDER,

		TYPE_MAX,	// ※ 種類の最大数
	};

	enum GROUP
	{ // 陣営
		GROUP_NONE, // 無所属
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
