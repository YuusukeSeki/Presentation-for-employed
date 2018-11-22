// author : yusuke seki
// data   : 20181102
#include "Controller.h"
#include "GameManager.h"
#include "Controller_KeyboardAndMouse.h"

Controller::Controller(const unsigned int& kKeyFront, const unsigned int& kKeyBack, const unsigned int& kKeyLeft, const unsigned int& kKeyRight
	, const unsigned int& kKeyAvoid, const unsigned int& kKeyStraightShot, const unsigned int& kKeyDrawShot, const unsigned int& kKeyBreak)
	: kKeyFront_(kKeyFront), kKeyBack_(kKeyBack), kKeyLeft_(kKeyLeft), kKeyRight_(kKeyRight),
	  kKeyAvoid_(kKeyAvoid), kKeyStraightShot_(kKeyStraightShot), kKeyDrawShot_(kKeyDrawShot), kKeyBreak_(kKeyBreak)
{
	//input_ = Input::Create(GameManager::GetInstanceHandle(), GameManager::GetWindowHandle());
	input_ = GameManager::GetInput();
}

Controller::~Controller()
{
	if (input_ != nullptr)
	{
		Release();
	}
}

Controller* Controller::Create(Controller::Device useDevice)
{
	switch (useDevice)
	{
		case Controller::Device::KEYBOARDANDMOUSE:
			return new KeyboardAndMouse;

		case Controller::Device::JOYSTICKANDTOUCHPEN:
			return nullptr;

		case Controller::Device::ALL:
			return nullptr;

		default:
			_MSGERROR("error", "Controller::Create");
			return nullptr;
	}

}

void Controller::Release()
{
	if (input_ != nullptr)
	{
		input_->Uninit();
		input_ = nullptr;
	}

	delete this;
}
