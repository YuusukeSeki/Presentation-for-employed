// author : yusuke seki
// data   : 20181102
#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <vector>

#include "ObjectModel.h"
#include "Controller.h"
#include "DrawLine.h"
#include "camera.h"
#include "tower.h"
#include "castle.h"
#include "Part.h"

#include "Unit.h"

class Collider;

class Player : public Unit
{
public:
	enum Character
	{
		FIGHTER,
		ATTACKER,
		SUPPORTER,
		MAX
	};

	enum Behave
	{
		NEUTRAL,
		RUN,
		AVOID,
		HEAVY_AVOID,
		READY_STRAIGHT_SHOT,
		READY_DRAW_SHOT,
		STRAIGHT_SHOT,
		DRAW_SHOT,
		BREAK_BASEPOINT,
		BENDING_BACKWARD,	// 仰け反る
		DOWN,
	};

	Player();
	Player(Object::TYPE type, const Player::Character& character);
	~Player();

	static Player* Create(const D3DXVECTOR3& position, const Player::Character& character, const Object::GROUP& _group);

	void Init(const D3DXVECTOR3& position, const Player::Character& character, const Object::GROUP& _group);
	void Uninit();
	void Update();
	void Draw();

	void ReceiveDamage(const float& _damage, Unit* _unit = nullptr);

	void SetBehave(const Player::Behave& _behave);
	Player::Behave GetBehave();

	void SetCoolTime(const unsigned int _coolTime);

	float GetStraightShotDamage();
	float GetStraightShotSpeed();
	float GetStraightShotRange();

	float GetDrawShotDamage();
	float GetDrawShotSpeed();
	float GetDrawShotRange();

	Camera* GetCamera();

private:
	// method
	void Move();
	void StraightShot();
	void DrawShot();
	void Break();
	void Inirtia();
	void MoveInirtia();
	void RotateInirtia();

	void Run();
	void Avoid();
	void HeavyAvoid();
	void Ready_StraightShot();
	void Attack_StraightShot();
	void Cancel_StraightShot();
	void Ready_DrawShot();
	void Create_DrawLine(const D3DXVECTOR3& _cursorPosition_3D);
	void Attack_DrawShot();
	void Cancel_DrawShot();
	void Break_BasePoint();

	bool IsRun();
	bool IsAvoid();
	bool IsHeavyAvoid();
	bool IsReady_StraightShot(const D3DXVECTOR3& _cursorPosition_3D);
	bool IsAttack_StraightShot(const D3DXVECTOR3& _cursorPosition_3D);
	bool IsCancel_StraightShot(const D3DXVECTOR3& _cursorPosition_3D);
	bool IsReady_DrawShot(const D3DXVECTOR3& _cursorPosition_3D);
	bool IsCreate_DrawLine();
	bool IsAttack_DrawShot(const D3DXVECTOR3& _cursorPosition_3D);
	bool IsCancel_DrawShot(const D3DXVECTOR3& _cursorPosition_3D);
	bool IsBreakBasePoint(const D3DXVECTOR3& _cursorPosition_3D);

	// around draw shot
	bool IsOutOfRange(const D3DXVECTOR3& _centerPosition, const D3DXVECTOR3& _comparePosition, float _range);
	bool IsOutOfLength(const float& _currentLength, const float& _maxLength);
	D3DXVECTOR3 WithinRange(const D3DXVECTOR3& _centerPosition, const D3DXVECTOR3& _nextPosition, float _range);
	D3DXVECTOR3 FindEndPosition(const float& _preLength, const float& _maxLength, const D3DXVECTOR3& _preRelayPosition, const D3DXVECTOR3& _nextRelayPosition);

	// collision
	void CollisionTower();
	void CollisionCastle();
	void CollisionWall();
	void CollisionField();
	void UpdateInverseMatrix();
	bool CollisionCursorToPlayer(const D3DXVECTOR3& _cursorPosition_3D);
	Tower* CollisionCursorToBasePoint();

	std::string FindModelPass(const Player::Character& character);
	D3DXVECTOR3 FindCursorPosition_3D();
	void InitParamater(Character _character);

	// パーツ関連。
	void AroundParts();





	static const std::vector<std::string> modelPassList_;
	static const float kInirtia_;
	static const float kMinThresholdSpeed_;
	static const float kThresholdLength_AddDraw_;

	// キャラクターとカメラ
	const Player::Character kCharacter_;
	const D3DXVECTOR3 kInitCameraPositionEye_;
	const D3DXVECTOR3 kInitCameraPositionAt_;
	const D3DXVECTOR3 kInitCameraVectorUp_;
	const float kInitCameraNear_;
	const float kInitCameraFar_;

	// 描画、コントロール関係
	Camera* camera_;
	D3DXMATRIX inverseMatrix_;
	Controller* controller_;
	D3DXVECTOR3 cursorPosition3D_;

	// 移動
	D3DXVECTOR3 moveVector_;
	float currentSpeed_;
	float runSpeed_;
	float avoidPower_;
	float heavyAvoidPower_;
	D3DXVECTOR3 prePosition_;

	// 徐々に回転
	float countTime_Rotate_;
	D3DXVECTOR3 startRotate_;
	D3DXVECTOR3 endRotate_;
	D3DXVECTOR3 startFront_;
	D3DXVECTOR3 endFront_;

	// ふるまい
	Behave currentBehave_;
	unsigned int currentCoolTime_;
	unsigned int coolTimeAvoid_;
	unsigned int coolTimeHeavyAvoid_;
	unsigned int coolTimeStraightShot_;
	unsigned int coolTimeDrawShot_;
	unsigned int coolTimeBreakBasePoint_;

	// 描画線
	DrawLine* drawLine_;
	float drawLine_currentLength_;
	float drawLine_preLength_;

	// パラメータ
	float currentHp_;
	float maxHp_;
	float currentMp_;
	float maxMp_;
	float straightShotDamage_;
	float straightShotSpeed_;
	float straightShotRange_;
	float drawShotDamage_;
	float drawShotSpeed_;
	float drawShotRange_;
	float drawShotLength_;
	float breakPower_;


public:
	struct KEY
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Rotation;
	};

	struct KEY_FRAME
	{
		int Frame;
		KEY Key[10];
	};

private:
	Part * m_Part[10];

	KEY_FRAME * m_KeyFrame;
	int m_Key;
	int m_Frame;

	void TempInput();

};


#endif
