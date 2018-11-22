// author : yusuke seki
// data   : 20181102
#include "Controller_KeyboardAndMouse.h"
#include "GameManager.h"

const int KeyboardAndMouse::kKeyFront_ = DIK_W;
const int KeyboardAndMouse::kKeyBack_ = DIK_S;
const int KeyboardAndMouse::kKeyLeft_ = DIK_A;
const int KeyboardAndMouse::kKeyRight_ = DIK_D;
const int KeyboardAndMouse::kKeyAvoid_ = DIK_E;
const int KeyboardAndMouse::kKeyStraightShot_ = Input::MOUSEBUTTON_LEFT;
const int KeyboardAndMouse::kKeyDrawShot_ = Input::MOUSEBUTTON_RIGHT;
const int KeyboardAndMouse::kKeyBreak_ = Input::MOUSEBUTTON_LEFT;

KeyboardAndMouse::KeyboardAndMouse() : Controller(kKeyFront_, kKeyBack_, kKeyLeft_, kKeyRight_, kKeyAvoid_, kKeyStraightShot_, kKeyDrawShot_, kKeyBreak_)
{

}

KeyboardAndMouse::~KeyboardAndMouse()
{

}

bool KeyboardAndMouse::IsButtonPress(Controller::Key key)
{
	switch (key)
	{
		case Controller::Key::RUN:
			if (input_->GetKeyboardPress(kKeyFront_) || input_->GetKeyboardPress(kKeyBack_) || input_->GetKeyboardPress(kKeyLeft_) || input_->GetKeyboardPress(kKeyRight_))
			{
				return true;
			}
			return false;

		case Controller::Key::AVOID:
			return input_->GetKeyboardPress(kKeyAvoid_);

		case Controller::Key::HEAVYAVOID:
			_MSGERROR("HEAVYAVOID はTriggerのみ使用可", "KeyboardAndMouse::IsButtonPress");
			return false;

		case Controller::Key::STRAIGHTSHOT:
			return input_->GetMousePress(kKeyStraightShot_);

		case Controller::Key::DRAWSHOT:
			return input_->GetMousePress(kKeyDrawShot_);

		case Controller::Key::BREAK:
			return input_->GetMousePress(kKeyBreak_);

		default:
			_MSGERROR("enum Controller::Key の値を確認してください", "KeyboardAndMouse::IsButtonPress");
			return false;
	}

}

bool KeyboardAndMouse::IsButtonTrigger(Controller::Key key)
{
	switch (key)
	{
		case Controller::Key::RUN:
			if (input_->GetKeyboardTrigger(kKeyFront_) || input_->GetKeyboardTrigger(kKeyBack_) || input_->GetKeyboardTrigger(kKeyLeft_) || input_->GetKeyboardTrigger(kKeyRight_))
			{
				return true;
			}
			return false;

		case Controller::Key::AVOID:
				return input_->GetKeyboardTrigger(kKeyAvoid_);

		case Controller::Key::HEAVYAVOID:
				return input_->GetKeyboardWTrigger(kKeyAvoid_);

		case Controller::Key::STRAIGHTSHOT:
			return input_->GetMouseTrigger(kKeyStraightShot_);

		case Controller::Key::DRAWSHOT:
			return input_->GetMouseTrigger(kKeyDrawShot_);

		case Controller::Key::BREAK:
			return input_->GetMouseTrigger(kKeyBreak_);

		default:
			_MSGERROR("enum Controller::Key の値を確認してください", "KeyboardAndMouse::IsButtonTrigger");
			return false;
	}

}

bool KeyboardAndMouse::IsButtonRelease(Controller::Key key)
{
	switch (key)
	{
		case Controller::Key::RUN:
			if (input_->GetKeyboardRelease(kKeyFront_) || input_->GetKeyboardRelease(kKeyBack_) || input_->GetKeyboardRelease(kKeyLeft_) || input_->GetKeyboardRelease(kKeyRight_))
			{
				return true;
			}
			return false;

		case Controller::Key::AVOID:
			return input_->GetKeyboardRelease(kKeyAvoid_);

		case Controller::Key::HEAVYAVOID:
			_MSGERROR("HEAVYAVOID はTriggerのみ使用可", "KeyboardAndMouse::IsButtonRelease");
			return false;

		case Controller::Key::STRAIGHTSHOT:
			return input_->GetMouseRelease(kKeyStraightShot_);

		case Controller::Key::DRAWSHOT:
			return input_->GetMouseRelease(kKeyDrawShot_);

		case Controller::Key::BREAK:
			return input_->GetMouseRelease(kKeyBreak_);

		default:
			_MSGERROR("enum Controller::Key の値を確認してください", "KeyboardAndMouse::IsButtonRelease");
			return false;
	}

}

D3DXVECTOR2 KeyboardAndMouse::GetAxis(Controller::Key key)
{
	D3DXVECTOR2 axisValue(0, 0);

	switch (key)
	{
		case Controller::Key::RUN:
			if (input_->GetKeyboardPress(kKeyFront_))
			{
				axisValue.y += 1;
			}

			if (input_->GetKeyboardPress(kKeyBack_))
			{
				axisValue.y -= 1;
			}

			if (input_->GetKeyboardPress(kKeyLeft_))
			{
				axisValue.x -= 1;
			}

			if (input_->GetKeyboardPress(kKeyRight_))
			{
				axisValue.x += 1;
			}

			break;

		default:
			_MSGERROR("enum Controller::Key の値を確認してください", "KeyboardAndMouse::GetAxis");
			break;

	}

	return axisValue;
}
