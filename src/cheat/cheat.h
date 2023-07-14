#pragma once
#include <Windows.h>
#include <Psapi.h>
#include "shared.h"
#include <string>

namespace cheat
{
	inline bool infiniteFc = false;
    inline bool infiniteHealth = false;
	inline bool oneHitKill = false;
	inline bool infiniteSubWeapon = false;
	inline bool noDamageStat = false;
	inline bool stealth = false;
	inline float heightRate = 0.0f;
	inline bool heightChange = false;
	inline bool groundCheat = false;
	inline bool groundEnabled = false;
	inline unsigned int groundCheatHotkey = 75;
	inline unsigned int temporaryVisorHotkey = 80;
	inline bool visorSwitch = false;
	inline bool autoHpUp = false;
	inline float ninjaRunSpeedRate = 0.9f;
	inline bool timeStop = false;
	inline bool timeStopSwitch = false;
	inline unsigned int timeStopHotkey = 84;
	inline bool dealZeroDamage = false;
	inline bool &OnFocus = *(bool*)(shared::base + 0x19D509C);
	inline bool infTimer = false;
	inline int battlePoints = 0;
	inline float playerSlowRate = 1.0f;

	inline bool PlayerIsSam = false;
	inline int BossHPShow = 0;
	inline int BossHPModify = 0;
	inline bool EnableInstantCharges = false;

	inline unsigned int test = 0x0;
	inline std::string testStyle = "";
	
	
	std::string getStyles(std::string Source);
	void AnimationsToArrays();

	void HandleCheats() noexcept;
	void LoadConfig() noexcept;
	void SaveConfig() noexcept;
	void Reset() noexcept;
	void MugenZangekiCheat() noexcept;
	void InfiniteHealthCheat() noexcept;
	void OneHitKillCheat() noexcept;
	void InfiniteSubWeaponCheat() noexcept;
	void NoDamageStatCheat() noexcept;
	void StealthCheat() noexcept;
	void HeightChangeCheat() noexcept;
	void GroundCheat() noexcept;
	void TemporaryVisorCheat() noexcept;
	void AutoHPUpCheat() noexcept;
	void NinjaRunSpeedCheat() noexcept;
	void TimeStop() noexcept;
	void Deal0Damage() noexcept;
	void InfVRTimer() noexcept;
	void BattlePointsChange() noexcept;
	void ChangePlayerSlowRate() noexcept;
	
	void ChangePlayerOnce() noexcept;
	void EnableSamInCapmaign() noexcept;
	void ChangeBossHP(int bossHP) noexcept;

	int hex_to_int(const std::string& str);
	inline std::string error = "";
	void InfEntity() noexcept;

	void changeModelID() noexcept;
	unsigned int ReadDoublePointer(unsigned int baseAddress, unsigned int offset) noexcept;
	unsigned int ReadSinglePointer(unsigned int baseAddress, unsigned int offset) noexcept;
	void WriteDoublePointer(unsigned int baseAddress, unsigned int offset, unsigned int value) noexcept;

	void ResetActions() noexcept;
	const char* GetAnimationNameById(void* pAnimUnit, int animId);
	//unsigned int AnimationChanger(unsigned int pAnimUnit, int animId);
	void AnimationChanger(unsigned int pAnimUnit);
	void InstantCharges(unsigned int pAnimUnit);


	//mission change
	void ChangeMission(unsigned int phaseId, static const char* phaseName, bool isDLCPhase) noexcept;
	void* KamaititiCreate(int *player, float a2) noexcept;

	int __cdecl stringhash32(const char* a1);

	

	DWORD* __cdecl changeBGM(const char* a1);

	signed __int32 __cdecl QTEButtonsCall(signed __int32 a1, signed __int32 a2);

}