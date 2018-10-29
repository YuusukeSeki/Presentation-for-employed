//*****************************************************************************
//
//		ƒLƒƒƒ‰ƒNƒ^[ƒpƒ‰ƒ[ƒ^•\
//												Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _LIST_CHARAPARAMETERS_H_
#define _LIST_CHARAPARAMETERS_H_

#include "main.h"
#include "soldier.h"
#include "list_LoadTexture_MainGame.h"
#include "MainGame.h"



class List_CharacterParameters
{
public:
	template <typename T>
	static void InitParameter(T* pPlayer)
	//static void InitParameter(OLMDL_Player* pPlayer)
	{
		switch (pPlayer->GetCharacter()) {
		case T::CHARACTER::CHARACTER_KIBITSU:
			InitKibitsu(pPlayer);
			break;
		}


	}
//	static void InitParameter(Soldier* pSoldier)
//	{
//		switch (pSoldier->GetCharacter()) {
//		case Soldier::CHARACTER::CHARACTER_NORMAL:
////			InitSoldier_Normal(pSoldier);
//			break;
//		}
//
//
//	}


protected:


private:
	template <typename T>
	static void InitKibitsu(T* pPlayer)
	{
		// İ’èƒpƒ‰ƒ[ƒ^
		const float initialVelosity    =  0.f;	// ‰‘¬
		const float acceleration       =  0.f;	// ‰Á‘¬“x
		const float avoidDistance      = 15.f;	// ‰ñ”ğ‹——£

		const int   straightShotDamage =   150;	// SS‚ÌˆĞ—Í
		const float straightShotSpeed  =   5.f;	// SS‚Ì‘¬“x
		const float straightShotRange  = 100.f;	// SS‚ÌË’ö

		const int   drawShotDamage     =   70;	// DS‚ÌˆĞ—Í
		const float drawShotSpeed      =   3.f;	// DS‚Ì‘¬“x
		const float drawShotRange      = 150.f;	// DS‚ÌË’ö
		const float drawShotLength     = 300.f;	// DS‚Ì•`‚¯‚é’·‚³

		const int   breakPower         =   5;	// ‹’“_‚ğ‰£‚é‹­‚³

		const int   cooldown_Avoid     =  45;	// yd’¼ŠÔz”ğ‚¯‚é
		const int   cooldown_EmAvoid   = 160;	// yd’¼ŠÔz‹Ù‹}‰ñ”ğ
		const int   cooldown_StShot    =  40;	// yd’¼ŠÔzSS
		const int   cooldown_DrShot    =  90;	// yd’¼ŠÔzDS

		const int   maxLife            = 100;	// yÅ‘å’lz‘Ì—Í
		const float maxMoveSpeed       =  2.f;	// yÅ‘å’lz‘¬“x

		// ÀÛ‚Éİ’è‚·‚é
		pPlayer->SetInirtialVelocity(initialVelosity);		// ‰‘¬
		pPlayer->SetAcceleration(acceleration);				// ‰Á‘¬“x
		pPlayer->SetAvoidDiatance(avoidDistance);			// ‰ñ”ğ‹——£

		pPlayer->SetStraightShotDamage(straightShotDamage);	// SS‚ÌˆĞ—Í
		pPlayer->SetStraightShotSpeed(straightShotSpeed);	// SS‚Ì‘¬“x
		pPlayer->SetStraightShotRange(straightShotRange);	// SS‚ÌË’ö

		pPlayer->SetDrawShotDamage(drawShotDamage);			// DS‚ÌˆĞ—Í
		pPlayer->SetDrawShotSpeed(drawShotSpeed);			// DS‚Ì‘¬“x
		pPlayer->SetDrawShotRange(drawShotRange);			// DS‚ÌË’ö
		pPlayer->SetDrawShotLength(drawShotLength);			// DS‚Ì•`‚¯‚é’·‚³

		pPlayer->SetBreakPower(breakPower);					// ‹’“_‚ğ‰£‚é‹­‚³
	
		pPlayer->SetCooldown_Avoid(cooldown_Avoid);			// yd’¼ŠÔz”ğ‚¯‚é
		pPlayer->SetCooldown_EmAvoid(cooldown_EmAvoid);		// yd’¼ŠÔz‹Ù‹}‰ñ”ğ
		pPlayer->SetCooldown_StShot(cooldown_StShot);		// yd’¼ŠÔzSS
		pPlayer->SetCooldown_DrShot(cooldown_DrShot);		// yd’¼ŠÔzDS
		pPlayer->SetMaxLife(maxLife);						// yÅ‘å’lz‘Ì—Í
		pPlayer->SetMaxMoveSpeed(maxMoveSpeed);				// yÅ‘å’lz‘¬“x


		pPlayer->SetPosture(T::POSTURE::POSTURE_NONE);		// ‘Ì¨
		pPlayer->SetLife(maxLife);							// Œ»İ‚Ì‘Ì—Í


	}


};


#endif
