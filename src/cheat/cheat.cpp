#include "cheat.h"
#include "../injector/injector.hpp"
#include "../IniReader.h"
#include "../KeyBind.h"
#include <cSlowRateManager.h>
#include <Trigger.h>
#include <GameMenuStatus.h>
#include <Pl0000.h>
#include <cGameUIManager.h>
#include <PlayerManagerImplement.h>
#include <PhaseManager.h>
#include <string>

/* TODO #
*  Make functions for each cheat instead (since return will cancel the handle cheats instead of if condition  
*/

DWORD OneHitKillCaveExit = shared::base + 0x68EE3A;
DWORD InfiniteRocketsCaveExit = shared::base + 0x5499F9;
DWORD InfiniteGrenadeCaveExit = shared::base + 0x54D8D6;
DWORD GroundCheatCaveExit = shared::base + 0xE6B464;
DWORD VRTimerCaveExit = shared::base + 0x81B44A;



/*************************** SAM IN RAIDEN CAMPAIGN ***************************/


unsigned int SamUnknownFunctionAddress = shared::base + 0x129EE50;
unsigned int SamUpdateAddress = shared::base + 0x129EAD0;						//Sam update, for fix sword in sheath, etc

unsigned int SamStateMachineFactoryAddress = shared::base + 0x012492C0;			//Sam blade mode address
unsigned int SamQTEActionsAddress = shared::base + 0x0129EA88;					//For slider and fix Mistral


/*************************** SAM FUNCTIONS ***************************/

unsigned int SamUnknownFunction = shared::base + 0x493BD0;
unsigned int SamUpdate = shared::base + 0x45C700;								//Sam update, for fix sword in sheath, etc
unsigned int SamStateMachineFactory = shared::base + 0x468E60;					//Sam blade mode
unsigned int SamQTEActions = shared::base + 0x6C38F0;							//Another name - Pl1400::field_4, for slider and fix Mistral

/*************************** SAM FUNCTIONS ***************************/




/*************************** RAIDEN FUNCTIONS ***************************/

unsigned int RaidenStateMachineFactory = shared::base + 0x791CC0;				//Raiden blade mode
unsigned int RaidenUpdateInput = shared::base + 0x8104B0;						//Raiden update actions
unsigned int RaidenUpdate = shared::base + 0x8064C0;							//Raiden update, for lost sword, etc
unsigned int RaidenQTEActions = shared::base + 0x6C0A70;						//For slider and fix Mistral, crashes if you call Sam blade mode!!!


/*************************** RAIDEN FUNCTIONS ***************************/

/*************************** FOR FIX MISTRAL AND SUNDOWNER SLIDER ***************************/

unsigned int FixMistralAndSliderAddress1 = shared::base + 0x95B878;
unsigned int FixMistralAndSliderAddress2 = shared::base + 0x96DCF3;
unsigned int FixMistralAndSliderAddress3 = shared::base + 0x192407;
unsigned int FixMistralAndSliderAddress4 = shared::base + 0xC747B; //fix Mistral crash
unsigned int FixMistralAndSliderAddress5 = shared::base + 0x95B51A; //fix crash on slider
unsigned int FixMistralAndSliderAddress6 = shared::base + 0x9687FD; //fix final event on slider

/*************************** FOR FIX MISTRAL AND SUNDOWNER SLIDER ***************************/


/*************************** SAM IN RAIDEN CAMPAIGN END***************************/

void __declspec(naked) OneHitKillCave() noexcept
{
	__asm {
		mov eax, [ecx+0x870]
		sub [ecx+0x870], eax
		jmp OneHitKillCaveExit
	}
}

void __declspec(naked) InfiniteRocketsCave() noexcept
{
	__asm {
		mov [ecx + 0x68], eax
		mov edx, [ecx + 0x60]
		dec edx
		mov [ecx + 0x5C], edx
		jmp InfiniteRocketsCaveExit
	}
}

void __declspec(naked) InfiniteGrenadeCave() noexcept
{
	__asm {
		mov eax, [ecx + 0x58]
		mov [ecx + 0x54], eax
		mov eax, [ecx + 0x54]
		jmp InfiniteGrenadeCaveExit
	}
}

void __declspec(naked) GroundCheatCave() noexcept
{
	__asm {
			cmp cheat::groundEnabled, 1
			je enabled
			jmp otherwise

			enabled:
				mov [eax], 2
				jmp GroundCheatCaveExit

			otherwise:
				mov [eax], 0
				jmp GroundCheatCaveExit
		
	}
}

float vrTimer = 0.10f;

void __declspec(naked) InfiniteVRTimerCave() noexcept
{
	__asm {
		fld dword ptr [vrTimer]
		jmp VRTimerCaveExit
	}
}

// Infinite fuel container
void cheat::MugenZangekiCheat() noexcept
{
	Trigger::GameFlags.GAME_MUGEN_ZANGEKI = infiniteFc;
}

// You can't die in game
void cheat::InfiniteHealthCheat() noexcept
{
	static bool once = false;
	if (infiniteHealth && !once) // its just patches the game, but doesn't write the value into
	{
		injector::MakeNOP(shared::base + 0x787859, 6, true);
		injector::MakeNOP(shared::base + 0x787865, 6, true);
		once = true;
	}
	else if (!infiniteHealth && once)
	{
		unsigned char rawBytes1[6] = { 0x29, 0x81, 0x70, 0x08, 0x00, 0x00 };
		injector::WriteMemoryRaw(shared::base + 0x787859, rawBytes1, 6, true);

		unsigned char rawBytes2[6] = { 0x29, 0x91, 0x70, 0x08, 0x00, 0x00 };
		injector::WriteMemoryRaw(shared::base + 0x787865, rawBytes2, 6, true);
		once = false;
	}
}

// One hit kills enemies (bosses included)
void cheat::OneHitKillCheat() noexcept
{
	static bool once = false;
	if (oneHitKill && !once)
	{
		injector::MakeJMP(shared::base + 0x68EE34, &OneHitKillCave, true); // 29 81 70 08 00 00
		injector::MakeNOP(shared::base + 0x68EE39, 1, true);
		once = true;
	}
	else if (!oneHitKill && once)
	{
		unsigned char rawBytes[6] = { 0x29, 0x81, 0x70, 0x08, 0x00, 0x00 };
		injector::WriteMemoryRaw(shared::base + 0x68EE34, rawBytes, 6, true);
		once = false;
	}
}

// Infinite sub-weapon ammo
void cheat::InfiniteSubWeaponCheat() noexcept
{
	static bool once = false;
	if (infiniteSubWeapon && !once)
	{
		injector::MakeJMP(shared::base + 0x5499F3, &InfiniteRocketsCave, true);
		injector::MakeNOP(shared::base + 0x5499F8, 1, true);

		injector::MakeJMP(shared::base + 0x54D8D0, &InfiniteGrenadeCave, true);
		injector::MakeNOP(shared::base + 0x54D8D5, 1, true);
		once = true;
	}
	else if (!infiniteSubWeapon && once)
	{
		unsigned char rocketArray[6] = { 0x89, 0x41, 0x68, 0x89, 0x51, 0x5C };
		unsigned char grenadeArray[6] = { 0xFF, 0x49, 0x54, 0x8B, 0x41, 0x54 };

		injector::WriteMemoryRaw(shared::base + 0x5499F3, rocketArray, 6, true);
		injector::WriteMemoryRaw(shared::base + 0x54D8D0, grenadeArray, 6, true);
		once = false;
	}
}

// No damage status for battles
void cheat::NoDamageStatCheat() noexcept
{
	static bool once = false;
	if (noDamageStat && !once)
	{
		unsigned char patched[1] = { 0x00 };
		injector::WriteMemoryRaw(shared::base + 0x81B482, patched, 1, true);
		once = true;
	}
	else if (!noDamageStat && once)
	{
		unsigned char raw[1] = { 0x01 };
		injector::WriteMemoryRaw(shared::base + 0x81B482, raw, 1, true);
		once = false;
	}
}

// Stealth cheat, i think you get it
void cheat::StealthCheat() noexcept
{
	static bool once = false;
	if (stealth && !once)
	{
		unsigned char patched[2] = { 0xEB, 0x19 };
		injector::WriteMemoryRaw(shared::base + 0x849286, patched, 2, true);
		once = true;
	}
	else if (!stealth && once)
	{
		unsigned char original[2] = { 0x74, 0x19 };
		injector::WriteMemoryRaw(shared::base + 0x849286, original, 2, true);
		once = false;
	}
}

// Changes height
void cheat::HeightChangeCheat() noexcept
{
	if (g_GameMenuStatus != InGame || !OnFocus)
		return;

	if (heightChange)
	{
		Pl0000* player = (Pl0000*)g_cGameUIManager.m_pPlayer;

		if (!player)
			return;

		if (shared::IsKeyPressed(VK_ADD, VK_ADD, false))
			player->m_vecOffset.y += heightRate;
		else if (shared::IsKeyPressed(VK_SUBTRACT, VK_SUBTRACT, false))
			player->m_vecOffset.y -= heightRate;

		if (shared::IsKeyPressed(VK_SUBTRACT) || shared::IsKeyPressed(VK_ADD))
		{
			player->m_pCharacterControl->m_nOnGroundState = 2;
			player->m_vecVelocity.y = 0.0f;
		}
	}
}

// Set ground for specific height
void cheat::GroundCheat() noexcept
{
	static bool once = false;
	if (groundCheat)
	{
		if (KeyBind::IsKeyPressed(groundCheatHotkey) && g_GameMenuStatus == InGame && OnFocus)
			groundEnabled = !groundEnabled;
		if (!once)
		{
			injector::MakeJMP(shared::base + 0xE6B45E, &GroundCheatCave, true);
			injector::MakeNOP(shared::base + 0xE6B463, 1, true);
			injector::MakeNOP(shared::base + 0x4E98CD, 3, true);
			once = true;
		}
	}
	else if (!groundCheat && once)
	{
		unsigned char rawBytes[6] = { 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00 };
		unsigned char rawBytes1[3] = { 0x89, 0x46, 0x10 };

		injector::WriteMemoryRaw(shared::base + 0xE6B45E, rawBytes, 6, true);
		injector::WriteMemoryRaw(shared::base + 0x4E98CD, rawBytes1, 3, true);
		once = false;
	}
}

// Toggles visor
void cheat::TemporaryVisorCheat() noexcept
{
	if (KeyBind::IsKeyPressed(temporaryVisorHotkey) && OnFocus)
		Trigger::GameFlags.GAME_PLAYER_VISOR_ENABLED ^= true;
}

// Regenerates health
void cheat::AutoHPUpCheat() noexcept
{
	Trigger::GameFlags.GAME_AUTO_HPUP = autoHpUp;
}

// Ninja run speed, you just need to type speed
void cheat::NinjaRunSpeedCheat() noexcept
{
	Pl0000* player = (Pl0000*)g_cGameUIManager.m_pPlayer;

	if (!player)
		return;

	ninjaRunSpeedRate = player->m_fNinjaRunSpeedRate;
}

// Literally time stop
void cheat::TimeStop() noexcept
{
	static cSlowRateManager* SlowRateManager = GetcSlowRateManager();
	if (KeyBind::IsKeyPressed(timeStopHotkey) && timeStop && OnFocus && g_GameMenuStatus == InGame)
	{
		timeStopSwitch = SlowRateManager->GetSlowRate(0) == 1.0f;

		if (!timeStopSwitch)
		{
			SlowRateManager->SetSlowRate(0, 1.0f);
			SlowRateManager->SetSlowRate(1, 1.0f);
			SlowRateManager->SetSlowRate(2, 1.0f);
		}
	}
	if (timeStopSwitch)
	{
		SlowRateManager->SetSlowRate(0, 1 * 3.33564095e-9f);
		SlowRateManager->SetSlowRate(1, 1 / 3.33564095e-9f);
		SlowRateManager->SetSlowRate(2, 1 * 3.33564095e-9f);
	}
}

void cheat::Deal0Damage() noexcept
{
	static bool once = false;
	if (dealZeroDamage && !once)
	{
		injector::MakeNOP(shared::base + 0x68EE34, 6, true);
		once = true;
	}
	else if (!dealZeroDamage && once)
	{
		unsigned char original[6] = { 0x29, 0x81, 0x70, 0x08, 0x00, 0x00 };
		injector::WriteMemoryRaw(shared::base + 0x68EE34, original, 6, true);
		once = false;
	}
}

void cheat::InfVRTimer() noexcept
{
	static bool readOnce = false;
	static unsigned char original[6];
	if (!readOnce)
	{
		injector::ReadMemoryRaw(shared::base + 0x81B440, original, 6, true);
		readOnce = true;
	}
	static bool once = false;
	if (infTimer && !once)
	{
		injector::MakeNOP(shared::base + 0x81B440, 6, true);
		injector::MakeJMP(shared::base + 0x81B440, &InfiniteVRTimerCave, true);
		once = true;
	}
	else if (!infTimer && once)
	{
		injector::WriteMemoryRaw(shared::base + 0x81B440, original, 6, true);
		once = false;
	}
}

void cheat::BattlePointsChange() noexcept
{
	auto playerManager = g_pPlayerManagerImplement;
	if (!playerManager)
		return;

	battlePoints = playerManager->m_nBattlePoints;
}

int __cdecl cheat::stringhash32(const char* a1)
{
	return ((int (__cdecl*)(const char*))(shared::base + 0xA03EA0))(a1);
}

DWORD *__cdecl cheat::changeBGM(const char* a1) {
	return ((DWORD*(__cdecl*)(const char*))(shared::base + 0xA5E1B0))(a1);
}


void cheat::ChangePlayerSlowRate() noexcept
{
	Pl0000* player = (Pl0000*)g_cGameUIManager.m_pPlayer;

	if (!player)
		return;

	playerSlowRate = player->m_pEntity->m_pSlowRateUnit->m_fCurrentSlowRate;
}
void cheat::ChangeBossHP(int bossHP) noexcept {
	unsigned int address = injector::ReadMemory<unsigned int>(shared::base + 0x014A5024);
	if (address) address = injector::ReadMemory<unsigned int>(address);


	injector::WriteMemory<int>(address + 0x870, bossHP, true);
}

void cheat::ChangePlayerOnce() noexcept
{

	if (PlayerIsSam) {
		//Fix animations for slider event in mission 4
		injector::WriteMemory<int>(shared::base + 0x7C798D, 539, true);
		injector::WriteMemory<int>(shared::base + 0x7C7A74, 539, true);
		injector::WriteMemory<int>(shared::base + 0x7C7A24, 540, true);
		injector::WriteMemory<int>(shared::base + 0x7C7CD0, 541, true);

		//Fix Mistral and Sundowner slider event
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress1, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress2, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress3, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress4, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress5, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress6, shared::base + 0x1735B20, true);

		//Change Players ID
		injector::WriteMemory<unsigned int>(shared::base + 0x014A1D70, 0x00011400, true);
		injector::WriteMemory<unsigned int>(shared::base + 0x014A4420, 0x00010010, true);


		//QTE Set Animations
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EBD4, shared::base + 0x7F5690, true);

		//Phase ID
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EB18, shared::base + 0xD850, true);

		//Raiden QTE Animations
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EAE4, shared::base + 0x805000, true);

		//Change Model
		injector::WriteMemory<int>(shared::base + 0x017EA024, 15, true);
		injector::WriteMemory<int>(shared::base + 0x017EA028, 15, true);
		injector::WriteMemory<int>(shared::base + 0x017E9FB4, 15, true);
	}
	else {
		
		injector::WriteMemory<int>(shared::base + 0x7C798D, 333, true);
		injector::WriteMemory<int>(shared::base + 0x7C7A74, 333, true);
		injector::WriteMemory<int>(shared::base + 0x7C7A24, 334, true);
		injector::WriteMemory<int>(shared::base + 0x7C7CD0, 335, true);

		//Fix Mistral and Sundowner slider event
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress1, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress2, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress3, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress4, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress5, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress6, shared::base + 0x17E9DB8, true);
		
		//Change Players ID
		injector::WriteMemory<unsigned int>(shared::base + 0x014A1D70, 0x00010010, true);
		injector::WriteMemory<unsigned int>(shared::base + 0x014A4420, 0x00011400, true);


		//QTE Set Animations
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EBD4, shared::base + 0x46DF00, true);

		//Phase ID
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EB18, shared::base + 0x6C3900, true);

		//Raiden QTE Animations
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EAE4, shared::base + 0x69D3D0, true);
	}
}

void cheat::EnableSamInCapmaign() noexcept 
{

	cheat::ChangePlayerOnce();

	int BossAction = injector::ReadMemory<unsigned int>(shared::base + 0x014A5024);
	if (BossAction) BossAction = injector::ReadMemory<unsigned int>(BossAction);
	if (BossAction) BossAction = injector::ReadMemory<unsigned int>(BossAction+0x618);
	
	BossHPShow = injector::ReadMemory<unsigned int>(shared::base + 0x014A5024);
	if (BossHPShow) BossHPShow = injector::ReadMemory<unsigned int>(BossHPShow);
	if (BossHPShow) BossHPShow = injector::ReadMemory<int>(BossHPShow + 0x870);


	if (PlayerIsSam) {


		Pl0000* player = (Pl0000*)g_cGameUIManager.m_pPlayer;
		PhaseManager Phase = (PhaseManager)g_PhaseManager;
		
		unsigned int CurrentPhase = Phase.GetCurrentPhase();




		//new code
		if (player) {
			unsigned int PlayerAction = player->m_nCurrentAction;
			int EventTrigger = injector::ReadMemory<int>(shared::base + 0x14A9F04);
			unsigned int SundownerSlider = injector::ReadMemory<unsigned int>(shared::base + 0x18793A0);


			/******************** FIX STANDING HERE *****************************/
			injector::WriteMemory<unsigned int>(shared::base + 0x129EDE0,
				(CurrentPhase == 0x740) ? shared::base + 0x6C0B00 : shared::base + 0x6C3920, true);
			/******************** FIX STANDING HERE *****************************/

			/******************** FIX MISTRAL *****************************/
			injector::WriteMemory<unsigned int>(shared::base + 0x129EBD0,
				(CurrentPhase == 0x170) ? shared::base + 0x78C900 : shared::base + 0x45CCA0, true);
			/******************** FIX MISTRAL *****************************/
			
			
			/******************** EXCELSUS AND NINJARUN FIX  *****************************/
			if (
				((EventTrigger && 
					((CurrentPhase == 0x380 && BossAction!=393219) || CurrentPhase == 0xA15 || CurrentPhase == 0x720)
					) //&& MonsoonBossAction!=393219 or ==65557
				|| (Trigger::GameFlags.GAME_SLIDER_NINJARUN_MODE 
				|| Trigger::GameFlags.GAME_MISSILE_NINJYARUN_MODE 
				|| Trigger::StaFlags.STA_NINJARUN
				|| Trigger::StaFlags.STA_QTE
				))
				&& (PlayerAction >= 0x00100000)
				) {
				player->m_nCurrentAction = 0;
				player->m_nCurrentActionId = 1;
			}
			else if ( (PlayerAction==0 && CurrentPhase!=0x720) || PlayerAction == 35 ||
				(PlayerAction == 205 || PlayerAction == 206) || PlayerAction == 12 
				|| (PlayerAction==0 && (Phase.m_nCurrentPhaseHash == stringhash32("P720_LEG_1") || Phase.m_nCurrentPhaseHash == stringhash32("P720_LEG_2")))
				&& (PlayerAction != 69 && PlayerAction != 70))
			{
				if (PlayerAction == 205 || PlayerAction == 206) {
					player->m_nCurrentAction = 0x00100055;
				}
				else player->m_nCurrentAction = 0x00100000;
					
				player->m_nCurrentActionId = 0;
			}
			/******************** EXCELSUS AND NINJARUN FIX  *****************************/
			
			if (PlayerAction >= 0x00100000)
			{
				injector::WriteMemory<unsigned int>(SamUnknownFunctionAddress, SamUnknownFunction, true);

				//injector::WriteMemory<unsigned int>(SamQTEActionsAddress, SamQTEActions, true);

				if (!EventTrigger && CurrentPhase!=0x750) {
					injector::WriteMemory<unsigned int>(SamUpdateAddress, SamUpdate, true);
				}
				else {
				injector::WriteMemory<unsigned int>(SamUpdateAddress, RaidenUpdate, true);
				}
				
				injector::WriteMemory<unsigned int>(SamStateMachineFactoryAddress, SamStateMachineFactory, true);
			}
			
			else if (PlayerAction < 0x00100000 || EventTrigger){
				
				if ((CurrentPhase == 0x610 && PlayerAction != 206) || Trigger::GameFlags.GAME_SLIDER_NINJARUN_MODE
					|| Trigger::GameFlags.GAME_MISSILE_NINJYARUN_MODE
					|| CurrentPhase == 0x720 || CurrentPhase == 0x380
					|| CurrentPhase == 0xA15 || SundownerSlider
					)
					injector::WriteMemory<unsigned int>(SamUnknownFunctionAddress, RaidenUpdateInput, true);
				
				//if (CurrentPhase == 0x470)
					//injector::WriteMemory<unsigned int>(SamQTEActionsAddress, RaidenQTEActions, true);

				//if (Phase.GetCurrentPhase() == 0x750 || Phase.GetCurrentPhase() == 0x470 || Phase.GetCurrentPhase() == 0x610 || Phase.GetCurrentPhase() == 0xA15)
				injector::WriteMemory<unsigned int>(SamUpdateAddress, RaidenUpdate, true);
				
				injector::WriteMemory<unsigned int>(SamStateMachineFactoryAddress, RaidenStateMachineFactory, true);
			}
		}
	}
}

void cheat::ChangeMission(unsigned int phaseId, static const char* phaseName, bool isDLCPhase) noexcept 
{

	uintptr_t phaseIdAddress = shared::base + 0x1764670;
	uintptr_t phaseNameAddress = shared::base + 0x1764674;

	if (isDLCPhase) {
		phaseIdAddress = shared::base + 0x1766004;
		phaseNameAddress = shared::base + 0x1766008;
	}

	//fix for change mission after DLC
	if (!isDLCPhase) {
		injector::MemoryFill(shared::base + 0x1766000, 0, 4, true);
		injector::MemoryFill(shared::base + 0x1766004, 0, 30, true);
	}

	injector::WriteMemory<unsigned int>(phaseIdAddress, phaseId, true);

	injector::MemoryFill(phaseNameAddress, 0, 30, true);


	for (int i = 0;i < strlen(phaseName);i++)
		if (phaseName[i] != NULL) injector::MemoryFill(phaseNameAddress + i, phaseName[i], 1, true);
}



// Handles all cheats at once
void cheat::HandleCheats() noexcept
{
	static bool once = false;
	if (!once)
	{
		LoadConfig();
		once = true;
	}
	// Player
	MugenZangekiCheat();
	InfiniteHealthCheat();
	InfiniteSubWeaponCheat();
	HeightChangeCheat();
	TemporaryVisorCheat();
	AutoHPUpCheat();
	NinjaRunSpeedCheat();
	TimeStop();
	BattlePointsChange();
	ChangePlayerSlowRate();

	// Enemies
	OneHitKillCheat();
	Deal0Damage();

	// Battle
	NoDamageStatCheat();
	StealthCheat();
	InfVRTimer();
	
	// Entities
	GroundCheat();
	EnableSamInCapmaign();
}

// Loads config (ini file)
void cheat::LoadConfig() noexcept
{
	CIniReader iniReader("ModMenu.ini");

	infiniteFc = iniReader.ReadInteger("Player", "InfFuelContainer", 0) == 1;
	infiniteHealth = iniReader.ReadInteger("Player", "InfHealth", 0) == 1;
	infiniteSubWeapon = iniReader.ReadInteger("Player", "InfSubWeapon", 0) == 1;
	heightChange = iniReader.ReadInteger("Player", "HeightChange", 0) == 1;
	heightRate = iniReader.ReadFloat("Player", "HeightRate", 0.0f);
	temporaryVisorHotkey = iniReader.ReadInteger("Player", "VisorHotkey", 80);
	autoHpUp = iniReader.ReadInteger("Player", "AutoHpUp", 0) == 1;
	dealZeroDamage = iniReader.ReadInteger("Player", "DealZeroDamage", 0) == 1;

	timeStop = iniReader.ReadInteger("Entities", "TimeStop", 0) == 1;
	timeStopHotkey = iniReader.ReadInteger("Entities", "TimeStopHotkey", 84);
	groundCheat = iniReader.ReadInteger("Entities", "GroundCheatEnabled", 0) == 1;
	groundCheatHotkey = iniReader.ReadInteger("Entities", "GroundCheatHotkey", 75);

	oneHitKill = iniReader.ReadInteger("Enemies", "OneHitKill", 0) == 1;

	noDamageStat = iniReader.ReadInteger("Battle", "NoDamageStat", 0) == 1;
	stealth = iniReader.ReadInteger("Battle", "Stealth", 0) == 1;
	infTimer = iniReader.ReadInteger("Battle", "InfiniteTimer", 0) == 1;
}

// Saves config (ini file)
void cheat::SaveConfig() noexcept
{
	CIniReader iniReader("ModMenu.ini");

	iniReader.WriteInteger("Player", "InfFuelContainer", infiniteFc);
	iniReader.WriteInteger("Player", "InfHealth", infiniteHealth);
	iniReader.WriteInteger("Player", "InfSubWeapon", infiniteSubWeapon);
	iniReader.WriteInteger("Player", "HeightChange", heightChange);
	iniReader.WriteFloat("Player", "HeightRate", heightRate);
	iniReader.WriteInteger("Player", "VisorHotkey", temporaryVisorHotkey);
	iniReader.WriteInteger("Player", "AutoHpUp", autoHpUp);
	iniReader.WriteInteger("Player", "DealZeroDamage", dealZeroDamage);

	iniReader.WriteInteger("Entities", "TimeStop", timeStop);
	iniReader.WriteInteger("Entities", "TimeStopHotkey", timeStopHotkey);
	iniReader.WriteInteger("Entities", "GroundCheatEnabled", groundCheat);
	iniReader.WriteInteger("Entities", "GroundCheatHotkey", groundCheatHotkey);

	iniReader.WriteInteger("Enemies", "OneHitKill", oneHitKill);

	iniReader.WriteInteger("Battle", "NoDamageStat", noDamageStat);
	iniReader.WriteInteger("Battle", "Stealth", stealth);
	iniReader.WriteInteger("Battle", "InfiniteTimer", infTimer);
}

// Resets cheats
void cheat::Reset() noexcept
{
	infiniteFc = false;
	infiniteHealth = false;
	oneHitKill = false;
	infiniteSubWeapon = false;
	noDamageStat = false;
	stealth = false;
	heightRate = 0.0f;
	heightChange = false;
	groundCheat = false;
	groundEnabled = false;
	groundCheatHotkey = 75;
	temporaryVisorHotkey = 80;
	visorSwitch = false;
	autoHpUp = false;
	ninjaRunSpeedRate = 0.9f;
	timeStop = false;
	timeStopHotkey = 84;
	dealZeroDamage = false;
	infTimer = false;
	playerSlowRate = 1.0f;
}