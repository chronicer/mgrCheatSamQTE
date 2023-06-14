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
#include <sstream>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/* TODO #
*  Make functions for each cheat instead (since return will cancel the handle cheats instead of if condition  
*/

DWORD OneHitKillCaveExit = shared::base + 0x68EE3A;
DWORD InfiniteRocketsCaveExit = shared::base + 0x5499F9;
DWORD InfiniteGrenadeCaveExit = shared::base + 0x54D8D6;
DWORD GroundCheatCaveExit = shared::base + 0xE6B464;
DWORD VRTimerCaveExit = shared::base + 0x81B44A;

bool changedStyle = false;
bool changeOnce = true;

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
unsigned int FixMistralAndSliderAddress7 = shared::base + 0x1F5B0; //fix slow mo on boss Sam when finish button shows

/*************************** FOR FIX MISTRAL AND SUNDOWNER SLIDER ***************************/


bool fileLoadOnce = true;
const int MAX_LINES = 4;
std::string nums1[MAX_LINES]; // массив для первых чисел
std::string nums2[MAX_LINES]; // массив для вторых чисел
int oldModelId = -1;
int oldPlayer = false;
/*************************** SAM IN RAIDEN CAMPAIGN END***************************/


unsigned int ModelId = 0x0;
unsigned int HairId = 0x0;
unsigned int HeadId = 0x0;
unsigned int SheathId = 0x0;
unsigned int VisorId = 0x0;

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


int cheat::hex_to_int(const std::string& str)
{
	unsigned int hex_num = 0;
	for (size_t i = 0; i < str.length(); ++i) {
		hex_num = (hex_num << 8) | static_cast<unsigned int>(str[i]);
	}

	return ((hex_num & 0xFF000000) >> 24) | ((hex_num & 0x00FF0000) >> 8)
		| ((hex_num & 0x0000FF00) << 8) | ((hex_num & 0x000000FF) << 24);;
}
/*void cheat::LoadStyleSwitcher() {
	std::ifstream file("StyleSwitcher.txt");
	if (file.is_open())
	{
		// Создаем переменную для хранения значения pl1400
		std::string pl1400;

		int count = 0;
		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string num_str, eq, hex_str;

			if (!(iss >> num_str >> eq >> hex_str)) {
				if (iss.eof()) {
					continue;
				}
			}

			nums1[count] = num_str;
			nums2[count] = hex_str;
			count++;

			if (count >= MAX_LINES) {
				break;
			}
		}
	}
	fileLoadOnce = false;
}*/


void cheat::ChangePlayerOnce() noexcept
{

	if (PlayerIsSam) {

		//Fix enemy alerts
		injector::WriteMemory<int>(shared::base + 0x8202A5, 65552, true);

		injector::WriteMemory<int>(shared::base + 0x7F7197, 550, true); //fix one ninjarun animation

		//Fix animations for slider event in mission 4
		injector::WriteMemory<int>(shared::base + 0x7C798D, 539, true); //fix slider
		injector::WriteMemory<int>(shared::base + 0x7C7A74, 539, true); //fix slider
		injector::WriteMemory<int>(shared::base + 0x7C7A24, 540, true); //fix slider
		injector::WriteMemory<int>(shared::base + 0x7C7CD0, 541, true); //fix slider
		//


		injector::WriteMemory<int>(shared::base + 0x7D641F, 329, true); //fix Raiden blademode
		injector::WriteMemory<int>(shared::base + 0x7B6EF7, 331, true); //fix Raiden blademode
		injector::WriteMemory<int>(shared::base + 0x7B6EF0, 414, true); //fix Raiden blademode
		injector::WriteMemory<int>(shared::base + 0x7B7284, 358, true); //fix Raiden blademode

		injector::WriteMemory<int>(shared::base + 0x7B9B9E, 341, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9BC8, 333, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9BEC, 338, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9C34, 336, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9CA9, 339, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9CF4, 337, true); //fix Raiden blademode (ground)

		injector::WriteMemory<int>(shared::base + 0x7B9D17, 383, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9D41, 375, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9D65, 380, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9D89, 377, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9DAD, 378, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9DD4, 382, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9DFB, 374, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9E22, 381, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9E49, 376, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9E6D, 379, true); //fix Raiden blademode (air)

		//Fix Mistral and Sundowner slider event
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress1, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress2, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress3, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress4, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress5, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress6, shared::base + 0x1735B20, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress7, shared::base + 0x1735B20, true);

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
		//injector::WriteMemory<int>(shared::base + 0x017EA024, 15, true);
		//injector::WriteMemory<int>(shared::base + 0x017EA028, 15, true);
		//injector::WriteMemory<int>(shared::base + 0x017E9FB4, 15, true);
	}
	else {

		//Fix enemy alerts
		injector::WriteMemory<int>(shared::base + 0x8202A5, 70656, true);

		injector::WriteMemory<int>(shared::base + 0x7F7197, 181, true); //fix one ninjarun animation
		

		injector::WriteMemory<int>(shared::base + 0x7C798D, 333, true); //fix slider
		injector::WriteMemory<int>(shared::base + 0x7C7A74, 333, true); //fix slider
		injector::WriteMemory<int>(shared::base + 0x7C7A24, 334, true); //fix slider
		injector::WriteMemory<int>(shared::base + 0x7C7CD0, 335, true); //fix slider

		injector::WriteMemory<int>(shared::base + 0x7D641F, 235, true); //fix Raiden blademode
		injector::WriteMemory<int>(shared::base + 0x7B6EF7, 239, true); //fix Raiden blademode
		injector::WriteMemory<int>(shared::base + 0x7B6EF0, 323, true); //fix Raiden blademode
		injector::WriteMemory<int>(shared::base + 0x7B7284, 267, true); //fix Raiden blademode

		injector::WriteMemory<int>(shared::base + 0x7B9B9E, 250, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9BC8, 242, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9BEC, 247, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9C34, 245, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9CA9, 248, true); //fix Raiden blademode (ground)
		injector::WriteMemory<int>(shared::base + 0x7B9CF4, 246, true); //fix Raiden blademode (ground)


		injector::WriteMemory<int>(shared::base + 0x7B9D17, 292, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9D41, 284, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9D65, 289, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9D89, 286, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9DAD, 287, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9DD4, 291, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9DFB, 283, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9E22, 290, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9E49, 285, true); //fix Raiden blademode (air)
		injector::WriteMemory<int>(shared::base + 0x7B9E6D, 288, true); //fix Raiden blademode (air)

		//Fix Mistral and Sundowner slider event
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress1, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress2, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress3, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress4, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress5, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress6, shared::base + 0x17E9DB8, true);
		injector::WriteMemory<unsigned int>(FixMistralAndSliderAddress7, shared::base + 0x17E9DB8, true);
		
		//Change Players ID
		injector::WriteMemory<unsigned int>(shared::base + 0x014A1D70, 0x00010010, true);
		injector::WriteMemory<unsigned int>(shared::base + 0x014A4420, 0x00011400, true);


		//QTE Set Animations
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EBD4, shared::base + 0x46DF00, true);

		//Phase ID
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EB18, shared::base + 0x6C3900, true);

		//Raiden QTE Animations
		injector::WriteMemory<unsigned int>(shared::base + 0x0129EAE4, shared::base + 0x69D3D0, true);


		injector::WriteMemory<unsigned int>(SamUpdateAddress, SamUpdate, true);
	}
}

void cheat::EnableSamInCapmaign() noexcept 
{
	if (oldPlayer != PlayerIsSam) {
		cheat::ChangePlayerOnce();
		oldPlayer = PlayerIsSam;
	}
	
	byte skill = injector::ReadMemory<byte>(shared::base + 0x17EA030);


	int BossAction = injector::ReadMemory<unsigned int>(shared::base + 0x014A5024);
	if (BossAction) BossAction = injector::ReadMemory<unsigned int>(BossAction);
	if (BossAction) BossAction = injector::ReadMemory<unsigned int>(BossAction+0x618);
	
	BossHPShow = injector::ReadMemory<unsigned int>(shared::base + 0x014A5024);
	if (BossHPShow) BossHPShow = injector::ReadMemory<unsigned int>(BossHPShow);
	if (BossHPShow) BossHPShow = injector::ReadMemory<int>(BossHPShow + 0x870);


	if (PlayerIsSam) {
		int currentModelID = injector::ReadMemory<int>(shared::base + 0x17E9FB4, true);
		if (currentModelID != oldModelId) {
			cheat::changeModelID();
			oldModelId = currentModelID;
		}

		injector::WriteMemory<byte>(shared::base + 0x93B13C, skill, true);

		Pl0000* player = (Pl0000*)g_cGameUIManager.m_pPlayer;
		PhaseManager Phase = (PhaseManager)g_PhaseManager;
		
		unsigned int CurrentPhase = Phase.GetCurrentPhase();

		//if (GetAsyncKeyState('4') & 0x8000 && changedStyle) {
			//changedStyle = false;
			//player->field_2620 = 0;
		//}

//		if (GetAsyncKeyState('5') & 0x8000 && !changedStyle) {
	//		changedStyle = true;
		//	player->field_2620 = 0;
		//}


		//new code
		if (player) {
		
			unsigned int animationUnitAddr = player->field_75C;

			if (player->m_nCurrentAction != 0x0010000F) changedStyle = false;

		//	if (player->field_2620 == 3) {
				//changedStyle = !changedStyle;
				//player->field_2620 = 0;
			//}
/*
			if (changedStyle) {
				//injector::WriteMemory<int>(animationUnitAddr + 0x1048, hex_to_int(values[0].c_str()), true);
				//injector::WriteMemory<int>(animationUnitAddr + 0x1084, hex_to_int(values[1].c_str()), true);
				//injector::WriteMemory<int>(animationUnitAddr + 0x10C0, hex_to_int(values[2].c_str()), true);
			//	if (player->field_2620 == 3) player->field_2620 = 0;
			}
			else {
				injector::WriteMemory<int>(animationUnitAddr + 0x1048, hex_to_int(nums1[0]), true);
				injector::WriteMemory<int>(animationUnitAddr + 0x1084, hex_to_int(nums2[1]), true);
				injector::WriteMemory<int>(animationUnitAddr + 0x10C0, hex_to_int("2002"), true);
			}*/


			unsigned int PlayerAction = player->m_nCurrentAction;
			int EventTrigger = injector::ReadMemory<int>(shared::base + 0x14A9F04);
			unsigned int SundownerSlider = injector::ReadMemory<unsigned int>(shared::base + 0x18793A0);

			if (CurrentPhase == 0xA50) player->RaidenUpdateInput();

			/******************** FIX STANDING HERE *****************************/
			injector::WriteMemory<unsigned int>(shared::base + 0x129EDE0,
				(CurrentPhase == 0x740 || CurrentPhase == 0xA15 || CurrentPhase == 0x138 || CurrentPhase == 0xA50) ? shared::base + 0x6C0B00 : shared::base + 0x6C3920, true);
			/******************** FIX STANDING HERE *****************************/

			/******************** FIX MISTRAL *****************************/
			injector::WriteMemory<unsigned int>(shared::base + 0x129EBD0,
				(CurrentPhase == 0x170 || CurrentPhase == 0xA15 || CurrentPhase == 0xA50 || CurrentPhase == 0x138
					|| CurrentPhase == 0x150
					) ? shared::base + 0x78C900 : shared::base + 0x45CCA0, true);
			/******************** FIX MISTRAL *****************************/
			if (Trigger::GameFlags.GAME_ENABLE_INFINITE_RIPPER_MODE)
				Trigger::GameFlags.GAME_ENABLE_INFINITE_RIPPER_MODE = 0;
			

			if (PlayerAction == 232 && player->m_nCurrentActionId == 2 && Phase.m_nCurrentPhaseHash == stringhash32("P138_BREAKDOWN_1")) {

				Trigger::GameFlags.GAME_QTE_UI_DISABLE = 1;
				player->m_nCurrentAction = 232;
				player->m_nCurrentActionId = 10;
				injector::WriteMemory<int>(animationUnitAddr + 0x30A0, hex_to_int("a10c"), true);
				injector::WriteMemory<int>(animationUnitAddr + 0x30DC, hex_to_int("a10f"), true);
				
			}
			if (Phase.m_nCurrentPhaseHash != stringhash32("P138_BREAKDOWN_1")) 
				injector::WriteMemory<int>(animationUnitAddr + 0x30A0, hex_to_int("4201"), true);
			
			if (Phase.m_nCurrentPhaseHash != stringhash32("P138_BREAKDOWN_1") && Phase.m_nCurrentPhaseHash != stringhash32("P138_HELI01_START")
				&& Phase.m_nCurrentPhaseHash != stringhash32("P138_HELI01")
				)
			injector::WriteMemory<int>(animationUnitAddr + 0x30DC, hex_to_int("4220"), true);

			if (Phase.m_nCurrentPhaseHash == stringhash32("P138_HELI01") && PlayerAction == 232) {
				player->m_nCurrentAction = 0x00100000;
				player->m_nCurrentActionId = 1;
			}
			

			if (PlayerAction == 205 || PlayerAction == 206) player->m_nCurrentAction = 0x00100055;

			if (
				((EventTrigger &&
					((CurrentPhase == 0x380 && BossAction != 393219) || CurrentPhase == 0xA15 || CurrentPhase == 0x720
						|| CurrentPhase == 0x02D0 || CurrentPhase == 0x520 || CurrentPhase == 0x420 || CurrentPhase == 0x310
						|| CurrentPhase == 0xA50
						|| Phase.m_nCurrentPhaseHash == stringhash32("P150_HELI")
						)
					) //&& MonsoonBossAction!=393219 or ==65557
					|| (Trigger::GameFlags.GAME_SLIDER_NINJARUN_MODE
						|| Trigger::GameFlags.GAME_MISSILE_NINJYARUN_MODE
						|| Trigger::StaFlags.STA_NINJARUN
						//|| Trigger::StaFlags.STA_QTE
						|| Trigger::GameFlags.GAME_KOGEKKO_PLAY
						))
				&& (PlayerAction >= 0x00100000 && (PlayerAction != 205 && PlayerAction != 206) && player->m_nBladeModeType==0)
				) {
					player->m_nCurrentAction = 0;
					player->m_nCurrentActionId = 1;
			}
			else if ((PlayerAction == 0 && CurrentPhase != 0x720 
				&& CurrentPhase!=0x520 && CurrentPhase!=0x380 
				&& CurrentPhase != 0x430 && CurrentPhase != 0x02D0 
				&& CurrentPhase != 0xA15 
				&& CurrentPhase != 0x310
				&& CurrentPhase != 0x138
				&& CurrentPhase != 0xA50
				&& Phase.m_nCurrentPhaseHash != stringhash32("P150_HELI")
				&& !Trigger::GameFlags.GAME_KOGEKKO_PLAY) || PlayerAction == 35 ||
				(PlayerAction == 205 || PlayerAction == 206) || PlayerAction == 12
				|| (PlayerAction == 0 && (Phase.m_nCurrentPhaseHash == stringhash32("P720_LEG_1") || Phase.m_nCurrentPhaseHash == stringhash32("P720_LEG_2")))
				&& (PlayerAction != 69 && PlayerAction != 70))
			{
				player->m_nCurrentAction = 0x00100000;
				player->m_nCurrentActionId = 1;
			}

			
			if (PlayerAction >= 0x00100000)
			{
				injector::WriteMemory<unsigned int>(SamUnknownFunctionAddress, SamUnknownFunction, true);

				injector::WriteMemory<unsigned int>(SamUpdateAddress, ((!EventTrigger && CurrentPhase != 0x750 && CurrentPhase!=0x138) ? SamUpdate : RaidenUpdate), true);
				
				injector::WriteMemory<unsigned int>(SamStateMachineFactoryAddress, SamStateMachineFactory, true);
			}
			
			else if (PlayerAction < 0x00100000 || EventTrigger){
				

				if (CurrentPhase != 0x0A50) injector::WriteMemory<unsigned int>(SamUnknownFunctionAddress, RaidenUpdateInput, true);
				injector::WriteMemory<unsigned int>(SamUpdateAddress, RaidenUpdate, true);
				injector::WriteMemory<unsigned int>(SamStateMachineFactoryAddress, RaidenStateMachineFactory, true);
			}
		}
	}
	else {
		injector::WriteMemory<byte>(shared::base + 0x93B13C, 8, true);
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

void cheat::changeModelID() noexcept
{

		uintptr_t modelAddress = 0x00000000;
		uintptr_t hairAddress = 0x00000000;
		uintptr_t headAddress = 0x00000000;
		uintptr_t sheathAddress = 0x00000000;
		uintptr_t visorAddress = 0x00000000;

		int currentModelID = injector::ReadMemory<int>(shared::base + 0x17E9FB4, true);

		//if (!samModel) {

			//Standart armor
		if (currentModelID == 0) {
			modelAddress = shared::base + 0x14A9828;
			hairAddress = shared::base + 0x14A982C;
			headAddress = shared::base + 0x14A9838;
			sheathAddress = shared::base + 0x14A9834;
			visorAddress = shared::base + 0x14A9830;
		}

		//Blue
		if (currentModelID == 1) {
			modelAddress = shared::base + 0x14A983C;
			hairAddress = shared::base + 0x14A9840;
			headAddress = shared::base + 0x14A984C;
			sheathAddress = shared::base + 0x14A9848;
			visorAddress = shared::base + 0x14A9844;
		}


		//Red
		if (currentModelID == 2) {
			modelAddress = shared::base + 0x14A9850;
			hairAddress = shared::base + 0x14A9854;
			headAddress = shared::base + 0x14A9860;
			sheathAddress = shared::base + 0x14A985C;
			visorAddress = shared::base + 0x14A9858;
		}

		//Yellow
		if (currentModelID == 3) {
			modelAddress = shared::base + 0x14A9864;
			hairAddress = shared::base + 0x14A9868;
			headAddress = shared::base + 0x14A9874;
			sheathAddress = shared::base + 0x14A9870;
			visorAddress = shared::base + 0x14A986C;
		}

		//Desperado body
		if (currentModelID == 4) {
			modelAddress = shared::base + 0x14A9878;
			hairAddress = shared::base + 0x14A987C;
			headAddress = shared::base + 0x14A9888;
			sheathAddress = shared::base + 0x14A9884;
			visorAddress = shared::base + 0x14A9880;
		}

		//Costume
		if (currentModelID == 5) {
			modelAddress = shared::base + 0x14A988C;
			hairAddress = shared::base + 0x14A9890;
			headAddress = shared::base + 0x14A989C;
			sheathAddress = shared::base + 0x14A9898;
			visorAddress = shared::base + 0x14A9894;
		}

		//Mariachi
		if (currentModelID == 6) {
			modelAddress = shared::base + 0x14A98A0;
			hairAddress = shared::base + 0x14A98A4;
			headAddress = shared::base + 0x14A98B0;
			sheathAddress = shared::base + 0x14A98AC;
			visorAddress = shared::base + 0x14A98A8;
		}

		//Standart armor
		if (currentModelID == 7) {
			modelAddress = shared::base + 0x14A9918;
			hairAddress = shared::base + 0x14A991C;
			headAddress = shared::base + 0x14A9920;
			sheathAddress = shared::base + 0x14A9924;
			visorAddress = shared::base + 0x14A9928;
		}

		//Original
		if (currentModelID == 8) {
			modelAddress = shared::base + 0x14A9918;
			hairAddress = shared::base + 0x14A991C;
			headAddress = shared::base + 0x14A9920;
			sheathAddress = shared::base + 0x14A9924;
			visorAddress = shared::base + 0x14A9928;
		}

		//Gray Fox
		if (currentModelID == 9) {
			modelAddress = shared::base + 0x14A9918;
			hairAddress = shared::base + 0x14A991C;
			headAddress = shared::base + 0x14A9920;
			sheathAddress = shared::base + 0x14A9924;
			visorAddress = shared::base + 0x14A9928;
		}

		//White
		if (currentModelID == 10) {
			modelAddress = shared::base + 0x14A9918;
			hairAddress = shared::base + 0x14A991C;
			headAddress = shared::base + 0x14A9920;
			sheathAddress = shared::base + 0x14A9924;
			visorAddress = shared::base + 0x14A9928;
		}

		//Inferno
		if (currentModelID == 11) {
			modelAddress = shared::base + 0x14A9918;
			hairAddress = shared::base + 0x14A991C;
			headAddress = shared::base + 0x14A9920;
			sheathAddress = shared::base + 0x14A9924;
			visorAddress = shared::base + 0x14A9928;
		}


		//Commandos armor
		if (currentModelID == 12) {
			modelAddress = shared::base + 0x14A9918;
			hairAddress = shared::base + 0x14A991C;
			headAddress = shared::base + 0x14A9920;
			sheathAddress = shared::base + 0x14A9924;
			visorAddress = shared::base + 0x14A9928;
		}
		//}

		/*if (samModel) {
			modelAddress = shared::base + 0x14A9954;
			hairAddress = shared::base + 0x14A9958;
			headAddress = shared::base + 0x14A9964;
			sheathAddress = shared::base + 0x14A9960;
			visorAddress = shared::base + 0x14A995C;
		}*/
		ModelId = 0x11406;
		HairId = 0x11401;
		HeadId = 0x11405;
		SheathId = 0x11404;
		VisorId = 0x11402;

		if (modelAddress != 0x00000000 && ModelId)
		injector::WriteMemory<unsigned int>(modelAddress, ModelId, true);
		if (hairAddress != 0x00000000 && HairId)
		injector::WriteMemory<unsigned int>(hairAddress, HairId, true);
		if (headAddress != 0x00000000 && HeadId)     
		injector::WriteMemory<unsigned int>(headAddress, HeadId, true);
		if (sheathAddress != 0x00000000 && SheathId)   
		injector::WriteMemory<unsigned int>(sheathAddress, SheathId, true);
		if (visorAddress != 0x00000000 && VisorId)    
		injector::WriteMemory<unsigned int>(visorAddress, VisorId, true);
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
	//if (fileLoadOnce) LoadStyleSwitcher();
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