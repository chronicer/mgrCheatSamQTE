#include "gui.h"
#include <Windows.h>
#include <string>
#include "../cheat/cheat.h"
#include "../injector/injector.hpp"
#include "../IniReader.h"
#include "../KeyBinds.h"
#include "../KeyBind.h"
#include "../kiero.h"
#include "../minhook/include/MinHook.h"

#include <Pl0000.h>
#include <cGameUIManager.h>
#include <GameMenuStatus.h>
#include <PlayerManagerImplement.h>
#include <Trigger.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"

#include <string>
#include <BehaviorEmBase.h>

#pragma warning(disable : 4996)

/*************************** Player Info ***************************/
unsigned int CurrentPlayerModelParts = shared::base + 0x17EA01C;
bool LoopPlayerAction = false;
bool LoopPlayerActionId = false;
bool LoopBossAction = false;
bool LoopBossActionId = false;


int PlayerActionForLoop = 0;

unsigned int hairValue = 0x0;
unsigned int visorValue = 0x0;
unsigned int sheathValue = 0x0;
unsigned int headValue = 0x0;
unsigned int weaponValue = 0x0;
int AnimationForTest = 4;
/*************************** Player Info ***************************/


/*************************** MISSIONS ***************************/

bool isDLCPhase = false;

static const char* missions[10] = { "Prologue", "1","2","3","4","5","6","7","Sam DLC","Wolf DLC" };
static const char* selectedMission = missions[0];
int selectedMissionID = 0;

static const char* phases[15][15] = {
	{"0A10","0A15","0A50","0A70","0A80"},						//Prologue

	{"0118","0128","0138","0140","0150","0168","0170"},			//Mission 1
	{"0210","0220","0230","02D0",},								//Mission 2
	{"0310","0320","0330","0340","0350","0360","0370","0380"},	//Mission 3
	{"0410","0420","0430","0440","0448","0458","0468","0470"},	//Mission 4
	{"0510","0520"},											//Mission 5
	{"0610"},													//Mission 6
	{"0710","0720","0730","0740","0750"},						//Mission 7

	{"0C10","0C20","0C30","0C40","0C50","0C60"},				//Sam DLC

	{"0D10","0D20","0D30","0D40","0D50","0D60"}					//Wolf DLC
};

static const char* subPhases[30][30][30] =
{
	/*************************** PROLOGUE ***************************/
	{
		{"start","btl_01_start","btl_01","btl_01_end","road_start","road","road_end","end"},
		{"start","btl_ray01_movie","btl_ray01","btl_ray_ev","btl_ray01_end","city_start","city","city_2","city_end","waterway_start","waterway_start_readpark","waterway_start_readray","waterway","waterway_end","btl_ray02_1_start","btl_ray02_0","btl_ray02_1","btl_ray02_1_end","btl_ray02_2_start","btl_ray02_2","btl_ray02_2_end","btl_ray02_3_start","btl_ray02_3","btl_ray02_3_end","end","test00","E3_ray_bt_start","E3_ray_bt","E3_ray_bt_end"},
		{"start","btl_sam_movie","btl_sam","btl_sam_end","btl_sam_2_start","btl_sam_2","btl_sam_2_end","btl_sam_3_start","btl_sam_3","btl_sam_3_end","end"},
		{"start","train_start","train","train_end","end"},
		{"start"}
	},
	/*************************** PROLOGUE END***************************/

	/*************************** MISSION 1 ***************************/
	{
		{"P118_START","P118_BEACH","P118_BTL","P118_BTL_IN","P118_BTL_END","P118_CYB_EVENT","P118_TUTORIAL","P118_EVENT_END","P118_END"},
		{"P128_START","P128_GATE","P128_GATE_2F","P128_GATE2","P128_STREET","P128_LOAD","P128_STREET_RESTART","P128_BTL","P128_BTL_START","P128_BTL_ADD","P128_BTL_END","P128_STREET2","P128_EVENT","P128_END"},
		{"P138_START","P138_MOVIE","P138_WOLF","P138_WOLF_SHOW","P138_WOLF_FADE","P138_IN","P138_RE_START","P138_BREAKDOWN_1","P138_BREAKDOWN_2","P138_HELI01_START","P138_HELI01","P138_HELI01_END","P138_SET"},
		{"P140_START","P140_IN","P140_DOOR_CHECK","P140_ADD_SET","P140_DOGTAG_GET","P140_DOGTAG_OUT","P140_GOOD_OPEN","P140_BAD_OPEN","P140_HOTEL_BTL","P140_HOTEL_BTL_END","P140_HOTEL_IN","P140_EVENT"},
		{"P150_START","P150_RESTART","P150_BTL","P150_BTL_SET","P150_BTL_END","P150_HELI"},
		{"P168_START","P168_FACTORY","P168_BTL_END","P168_IN","P168_FLOOR","P168_FLOOR2","P168_FLOOR3","P168_END"},
		{"P170_START","P170_MOVIE","P170_MISTRAL","P170_MISTRAL01","P170_MISTRAL02","P170_MISTRAL03","P170_MIST_RESULT","P170_MIST_DEAD","P170_END","P170_TEST"}
	},
	/*************************** MISSION 1 END***************************/

	/*************************** MISSION 2 ***************************/
	{
		{"P210_SEWER_START","P210_SEWER_MOVIE","P210_SEWER_START_RADIO","P210_SEWER","P210_MASTIFF_EV","P210_MASTIFF","P210_SEWER_2","P210_SEWER_BURAN","P210_EVENT","P210_SEWER_END"},
		{"P220_START","P220_MOVIE","P220_RAPTOR","P220_GEORGE_EVENT","P220_GEORGE_EV_ED","P210_MASTIFF","P220_GEORGE_EV_MOVIE","P220_RADIO","P220_SEWER_3","P220_SEWER_4","P220_SUB_BTL_1","P220_SEWER_4_AFTER","P220_SEARCH_GATE","P220_SEARCH_GATE_2","P220_SEARCH_GATE_3","P220_FIND_GATE","P220_BURAN_CLEAR","P220_RABO","P220_RABO_2","P220_RABO_SIGNAL","P220_RABO_BTL_END","P220_BRAIN_EVENT","P220_BRAIN_EV_ED","P220_BRAIN_EV_MOVIE","P220_END","P220_BTL_2"},
		{"P230_RADIO","P230_RADIO_ED","P230_KOGECKO_SEARCH","P230_KOGECKO_ST","P230_KOGECKO","P230_KOGECKO_ED","P230_EVENT","P230_END","P230_KEVIN_RADIO","P230_KEVIN_RADIO_ED"},
		{"P2D0_GEOGE_RADIO","P2D0_GEOGE_RADIO_END","P2D0_START_EVENT","P2D0_BOX_START","P2D0_EVENT","P2D0_ROBO_START","P2D0_BOSS_END","P2D0_CODEC_END","P2D0_PASSAGE","P2D0_LIFE_RADIO","P2D0_EVENT2","P2D0_END"}
	},
	/*************************** MISSION 2 END***************************/


	/*************************** MISSION 3 ***************************/
	{
		{"P310_START","P310_BTL1","P310_RESTART","P310_STREET","P310_BTL1_2","P310_WOLF","P310_BTL2_START","P310_BTL2","P310_BTL2_LOAD","P310_BTL2_OFF","P310_STREET2","P310_STREET2_END","P310_END"},
		{"P320_START","P320_ENTER","P320_BTL","P320_BTL_END","P320_OFFICE","P320_ELV","P320_ELV_END","P320_ELV_IN","P320_END"},
		{"P330_START","P330_LOAD","P330_IN","P330_2ND","P330_BTL","P330_BTL_IN","P330_BTL_END","P330_3RD","P330_4TH","P330_ELV","P330_FALL","P330_FALL2","P330_END"},
		{"P340_START","P340_IN","P340_SUBWAY","P340_SUBWAY2","P340_SUBWAY3","P340_FREE_BTL","P340_FREE_BTL_1","P340_FREE_BTL_2","P340_FREE_END","P340_RESULT_END","P340_END"},
		{"P350_START","P350_IN","P350_STREET","P350_STREET_DOWN","P350_STREET2","P350_STREET2_BTL","P350_MALL","P350_MALL_WOLF","P350_MALL_BTL","P350_HOLOGRAM","P350_SAM","P350_SAM_CODEC","P350_SAM2","P350_SAM_CHANGE","P350_SAM_DELETE","P350_PLAZA_BTL","P350_PLAZA_RE","P350_PLAZA_END","P350_END"},
		{"P360_START","P360_RESTART","P360_IN","P360_BUILD","P360_BUILD_BTL","P360_SAM_TALK","P360_MON"},
		{"P370_TALK","P370_RESTART","P370_IN","P370_EVENT","P370_TEST"},
		{"P380_START","P380_MOVIE","P380_MONSOON","P380_FINISH_QTE","P380_MON_RESULT","P380_MON_DEAD"}
	},
	/*************************** MISSION 3 END***************************/

	/*************************** MISSION 4***************************/
	{
		{"P410_START","P410_IN","P410_CODEC_END","P410_BTL","P410_BTL_TUTO","P410_BTL_END","P410_DEFENSE","P410_DEFENSE_01","P410_ELV","P410_DEFENSE_END","P410_ELV_START"},
		{"P420_START","P420_IN","P420_ENM_ON","P420_GATE","P420_IN2","P420_GATE_2","P420_IN3","P420_GATE_3","P420_GATE_OPEN","P420_GATE_OPEN2","P420_GATE_END"},
		{"P430_RUN_INIT","P430_RUN_FIRST_HALF","P430_RUN_SECOND_HALF","P430_SLIDER_RUN_INIT","P430_SLIDER_RUN","P430_OUTER_WALL_RESTART","P430_OUTER_WALL","P430_END","P430_START"},
		{"P440_START"},
		{"P448_START","P448_LOAD","P448_CODEC_END","P448_IN","P448_AREA_CLEAR","P448_CLEAR_ENM","P448_IN2","P448_IN_ROOF","P448_IN3","P448_END"},
		{"P458_START","P458_CODEC_END","P458_IN","P458_LIFT_01_UP","P458_LIFT_00_UP","P458_BTL_01","P458_LIFT_01_DOWN","P458_BTL_02","P458_RPG","P458_BOMB","P458_KOGEKKO","P458_DEAD","P458_KOGE_CLEAR","P458_RIDE","P458_RIDE_SET","P458_BTL_03","P458_LAST","P458_BTL_END","P458_LAST_SET","P458_END"},
		{"P468_START","P468_SET","P468_IN","P468_BTL","P468_MIST","P468_MIST_DEAD","P468_MON","P468_BTL_END","P468_RESTART","P468_EVENT","P468_EV_END","P468_EV_END_RE","P468_FREE_END","P468_END"},
		{"P470_START","P470_MOVIE","P470_SUNDOWNER","P470_SUNDOWNER_2","P470_QTE","P470_QTE_DEAD","P470_SUN_RESULT","P470_SUN_DEAD","P470_EVENT"}
	},
	/*************************** MISSION 4 END***************************/


	/*************************** MISSION 5 ***************************/

	{
		{"P510_START","P510_IN","P510_BTL","P510_BTL_END","P510_MOVE","P510_BTL2","P510_BTL2_IN","P510_BTL2_SET","P510_BTL2_END","P510_END"},
		{"P520_START","P520_IN","P520_BTL","P520_BTL_IN","P520_BTL_END","P520_RUN","P520_RUN_SAVE","P520_STREET","P520_SUB","P520_SUB_BTL","P520_SUB_END","P520_EVENT","P520_END"}
	},

	/*************************** MISSION 5 END***************************/

	/*************************** MISSION 6 ***************************/

	{
		{"P610_START","P610_MOVIE","P610_BOSS","P610_BOSS_END","P610_END"}
	},

	/*************************** MISSION 6 END ***************************/


	/*************************** MISSION 7 ***************************/

	{
		{"P710_START","P710_IN","P710_RESTART","P710_BTL","P710_CODEC","P710_BASE","P710_HANGAR","P710_RUNWAY","P710_END"},
		{"P720_START","P720_MOVIE","P720_EXCELSUS","P720_LEG_1","P720_EXCELSUS_2","P720_LEG_2","P720_QTE","P720_RESULT","P720_BOSS","P720_EVENT"},
		{"P730_START","P730_MOVIE","P730_ARMSTRONG","P730_EVENT"},
		{"P740_START","P740_MOVIE","P740_ARMSTRONG","P740_QTE","P740_EVENT"},
		{"P750_START","P750_MOVIE","P750_ARMSTRONG","P750_QTE","P750_EVENT"}
	},

	/*************************** MISSION 7 END ***************************/

	/*************************** Sam DLC ***************************/
	{
		{"PC10_START","PC10_START2","PC10_MOVIE","PC10_SAM","PC10_SEWER","PC10_ROOM","PC10_ROOM_END","PC10_END"},
		{"PC20_START","PC20_MOVIE","PC20_WOLF","PC20_WOLF_DEAD","PC20_WOLF_END","PC20_IN","PC20_LIFT_LOAD","PC20_LIFT_ROOM","PC20_BTL_END","PC20_LIFT_UP","PC20_LIFT_STOP","PC20_LIFT_BTL","PC20_LIFT_END","PC20_TOP","PC20_END"},
		{"PC30_START","PC30_MOVIE","PC30_RAY","PC30_RAY_HEAD","PC30_RAY_DEAD","PC30_RAY_RESULT","PC30_RAY_END","PC30_2ND","PC30_PASS","PC30_PASS_RES","PC30_PASS_END","PC30_END"},
		{"PC40_START","PC40_CODEC_END","PC40_IN","PC40_IN_FREE","PC40_CENTER","PC40_CENTER_FREE","PC40_IKE_BTL","PC40_IKE","PC40_IKE_FREE","PC40_PASS","PC40_ELV","PC40_ELV_BTL","PC40_ELV_END","PC40_END"},
		{"PC50_START","PC50_CODEC_END","PC50_ROOM","PC50_DOOR_ROCK","PC50_KEY_GET","PC50_KEY_GET2","PC50_DOOR_OPEN","PC50_PASS","PC50_ROOF","PC50_ROOF_BTL","PC50_ROOF_END","PC50_END"},
		{"PC60_START","PC60_MOVIE","PC60_ARMSTRONG","PC60_ARMSTRONG2","PC60_QTE","PC60_ARM_RESULT","PC60_ARM_END","PC60_END"},
		{"PC71_START","PC71_GAME","PC71_END"},
		{"PC72_START","PC72_GAME","PC72_END"},
		{"PC73_START","PC73_GAME","PC73_END"},
		{"PC74_START","PC74_GAME","PC74_GAME1","PC74_GAME2","PC74_GAME3","PC74_GAME4","PC74_GAME_LOAD","PC74_GAME5","PC74_GAME6","PC74_GAME7","PC74_END"},
		{"PC75_START","PC75_GAME","PC75_GAME2","PC75_GAME3","PC75_END"},
		{"PC08_START","PC08_GAME","PC08_END"},
		{"PC09_START","PC09_BTL1","PC09_FADE","PC09_BTL2","PC09_END"}
	},
	/*************************** SAM DLC END ***************************/


	/*************************** WOLF DLC ***************************/
	{
		{"PD10_START","PD10_CODEC_END","PD10_TERGET","PD10_NMANI","PD10_END"},
		{"PD20_START","PD20_MOVIE","PD20_EVE","PD20_EVE2","PD20_VR","PD20_VR_01","PD20_END"},
		{"PD30_START","PD30_MOVIE","PD30_MISSION1","PD30_M1_ON","PD30_M1_RESULT","PD30_MISSION2","PD30_M2_ON","PD30_M2_RESULT","PD30_M2_CLEAR","PD30_M2_CLEAR2","PD30_M2_BTL","PD30_M2_END","PD30_MISSION3","PD30_M3_ON","PD30_MISSION3_2","PD30_MISSION3_3","PD30_M3_RESULT"},
		{"PD40_START","PD40_MOVIE","PD40_WOLF","PD40_WOLF2","PD40_WOLF_END","PD40_WOLF_END2","PD40_CITY","PD40_CITY2","PD40_CITY_END","PD40_CITY_END2","PD40_HELI","PD40_HELI_BTL","PD40_HELI_LOAD","PD40_HELI_END","PD40_BRIDGE","PD40_BRIDGE_END","PD40_END"},
		{"PD50_START","PD50_LOAD","PD50_CODEC_END","PD50_BTL","PD50_SET","PD50_SET_LOAD","PD50_SET_LOAD2","PD50_SET_CODEC","PD50_SET_END","PD50_STREET","PD50_STREET0","PD50_STREET1","PD50_STREET2","PD50_STREET_END","PD50_GATE","PD50_GATE_IN","PD50_GATE_END","PD50_STOP","PD50_END"},
		{"PD60_START","PD60_MOVIE","PD60_KHAMSIN","PD60_RESULT","PD60_KHAM_DEAD","PD60_CODEC","PD60_BOMB","PD60_END"}
	}
	/*************************** WOLF DLC END ***************************/
};


static const char* selectedPhaseId = phases[0][0];
static const char* selectedPhaseName = subPhases[0][0][0];
int selectedPhase = 0;
int selectedPhaseN = 0;

/*************************** MISSIONS END***************************/


bool once1 = false;
// Renders gui for cheats
void gui::RenderGUI() noexcept
{
	Pl0000* player = (Pl0000*)g_cGameUIManager.m_pPlayer;

	if (!once1)
	{
		LoadConfig();
		once1 = true;
	}

	static bool paused = false;

	if (KeyBind::IsKeyPressed(menuKey) && cheat::OnFocus)
		show = !show;

	if (usesPause)
	{
		if (show && g_GameMenuStatus == InGame)
		{
			Trigger::StaFlags.STA_PAUSE = true;
			paused = true;
		}

		if (!show && paused && g_GameMenuStatus == InGame)
		{
			Trigger::StaFlags.STA_PAUSE = false;
			paused = false;
		}

		if (!show)
			Trigger::StpFlags.STP_GAME_UPDATE = false;
	}
	else
	{
		Trigger::StpFlags.STP_MOUSE_UPDATE = show && g_GameMenuStatus == InGame;
		Trigger::StpFlags.STP_PL_CAM_KEY = show && g_GameMenuStatus == InGame;
		Trigger::StpFlags.STP_PL_ATTACK_KEY = show && g_GameMenuStatus == InGame;
	}

	/* :: REMOVED, REASON: Its lagging game because of hooking EndScene
	if (!show)
		Sleep(20);
	*/

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderBattleStatsWindow();

	if (show)
	{
		if (usesPause) 
			Trigger::StpFlags.STP_GAME_UPDATE = g_GameMenuStatus == 1;
		ImGui::Begin("Mod Menu", NULL, ImGuiWindowFlags_NoCollapse);
		ImGui::SetNextWindowSize({width, height});
		ImGuiIO io = ImGui::GetIO();
		io.MouseDrawCursor = true;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		if (ImGui::BeginTabBar("NOTITLE", ImGuiTabBarFlags_NoTooltip))
		{
			if (ImGui::BeginTabItem("Player"))
			{
				ImGui::Checkbox("Infinite Fuel Container", &cheat::infiniteFc);
				ImGui::Checkbox("Infinite Health", &cheat::infiniteHealth);
				ImGui::Checkbox("Infinite Subweapon Ammo & Supplies", &cheat::infiniteSubWeapon);
				ImGui::Checkbox("Height Change (numpad +, -)", &cheat::heightChange);
				ImGui::SliderFloat("Height Change Rate", &cheat::heightRate, 0.0f, 100.0f, "%.3f", 1.0f);
				KeyBind::Hotkey("Visor Hotkey: ", &cheat::temporaryVisorHotkey);
				ImGui::Checkbox("Auto HP Up", &cheat::autoHpUp);
				if (ImGui::InputFloat("Ninja Run Speed Rate", &cheat::ninjaRunSpeedRate) && player)
						player->m_fNinjaRunSpeedRate = cheat::ninjaRunSpeedRate;
				if (ImGui::Button("Toggle ripper mode") && player)
					if (player->m_nRipperModeEnabled)
						player->DisableRipperMode(false);
					else
						player->EnableRipperMode();
				if (ImGui::Button("Toggle ripper mode effect") && player)
				{
					static bool ripperModeEffectSwitch = false;
					ripperModeEffectSwitch ^= true;
					if (ripperModeEffectSwitch)
						player->CallEffect(100, &player->field_3470);
					else
						player->field_3470.Fade(0.1f, 0.0f);
				}
				if (ImGui::InputInt("Battle points", &cheat::battlePoints, 100, 500) && g_pPlayerManagerImplement)
					g_pPlayerManagerImplement->m_nBattlePoints = cheat::battlePoints;

				ImGui::InputInt("Battle points in customize menu", &*(int*)(shared::base + 0x177589C), 100, 500);
				if (player)
				{
					ImGui::InputFloat("Player speed", &cheat::playerSlowRate);
					player->m_pEntity->m_pSlowRateUnit->m_fCurrentSlowRate = cheat::playerSlowRate;
				}

				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem("Player Info")) {

				Pl0000* player = (Pl0000*)g_cGameUIManager.m_pPlayer;
				if (player) {

					ImGui::PushItemWidth(100);
					ImGui::Text("Player Model ID: %X", cheat::ReadDoublePointer(CurrentPlayerModelParts, 0x0));
					ImGui::Text("Player Hair ID: %X", cheat::ReadDoublePointer(CurrentPlayerModelParts, 0x4));
					ImGui::SameLine();
					if (ImGui::InputScalar("Set Hair Id", ImGuiDataType_U64, &hairValue, nullptr, nullptr, "%08X"))
						cheat::WriteDoublePointer(CurrentPlayerModelParts, 0x4, hairValue);
					
					ImGui::Text("Player Visor ID: %X", cheat::ReadDoublePointer(CurrentPlayerModelParts, 0x8));
					ImGui::SameLine();
					if (ImGui::InputScalar("Set Visor Id", ImGuiDataType_U64, &visorValue, nullptr, nullptr, "%08X"))
						cheat::WriteDoublePointer(CurrentPlayerModelParts, 0x8, visorValue);
					
					
					ImGui::Text("Player Sheath ID: %X", cheat::ReadDoublePointer(CurrentPlayerModelParts, 0xC));
					ImGui::SameLine();
					if (ImGui::InputScalar("Set Sheath Id", ImGuiDataType_U64, &sheathValue, nullptr, nullptr, "%08X"))
						cheat::WriteDoublePointer(CurrentPlayerModelParts, 0xC, sheathValue);


					ImGui::Text("Player Head ID: %X", cheat::ReadDoublePointer(CurrentPlayerModelParts, 0x10));
					ImGui::SameLine();
					if (ImGui::InputScalar("Set Head Id", ImGuiDataType_U64, &headValue, nullptr, nullptr, "%08X"))
						cheat::WriteDoublePointer(CurrentPlayerModelParts, 0x10, headValue);

					ImGui::Text("Player Weapon ID: %X", cheat::ReadDoublePointer(shared::base+0x17E9FF4, 0x0));
					

					ImGui::Value("Player Current Action",player->m_nCurrentAction);
					ImGui::SameLine();
				//	if (!LoopPlayerAction)
					ImGui::InputScalar("Set action", ImGuiDataType_S32,  &player->m_nCurrentAction);
					/*if (LoopPlayerAction)
						ImGui::InputScalar("Set action", ImGuiDataType_S32, &PlayerActionForLoop);

					ImGui::SameLine();
					ImGui::Checkbox("Loop", &LoopPlayerAction);
					if (LoopPlayerAction) {
						player->m_nCurrentAction = PlayerActionForLoop;
					}*/

					ImGui::Value("Player Current Action ID",player->m_nCurrentActionId);
					ImGui::SameLine();
					ImGui::InputScalar("Set action Id", ImGuiDataType_S32, &player->m_nCurrentActionId);
				
					ImGui::Value("Player Current Animation ID", cheat::ReadSinglePointer(player->field_774, 0x14));
					ImGui::Value("Player Max Health", player->m_nMaxHealth);
					ImGui::Value("Player Current Health", player->m_nHealth);

					
					
					ImGui::InputScalar("Set Animation For Test", ImGuiDataType_S32, &AnimationForTest);
					if (ImGui::Button("Call Animation by ID")) {
						player->FindAnimation(AnimationForTest, 0, 0.2, 1.0, 0x8000000, -1.0, 1.0);
					}

					ImGui::Separator();
					Entity* targetEnemyEntity = *(Entity**)(shared::base + 0x19BFF60);
					if (targetEnemyEntity) {
						BehaviorEmBase* targetEnemy = (BehaviorEmBase*)targetEnemyEntity->m_pInstance;
						
						ImGui::Value("Boss Current Action", targetEnemy->m_nCurrentAction);
						ImGui::SameLine();
						
						ImGui::InputScalar("Set Boss Action", ImGuiDataType_S32, &targetEnemy->m_nCurrentAction);
						
						ImGui::Value("Boss Current Action ID", targetEnemy->m_nCurrentActionId);
						ImGui::SameLine();
						ImGui::InputScalar("Set Boss Action Id", ImGuiDataType_S32, &targetEnemy->m_nCurrentActionId);
						
						ImGui::Value("Boss Current Animation ID", cheat::ReadSinglePointer(targetEnemy->field_774, 0x14));
						ImGui::Value("Boss Max Health", targetEnemy->m_nMaxHealth);
						
						ImGui::Value("Boss Current Health", targetEnemy->m_nHealth);
						ImGui::SameLine();
						ImGui::InputScalar("Set Boss Health", ImGuiDataType_S32, &targetEnemy->m_nHealth);

					}

				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Entities"))
			{
				ImGui::Checkbox("Ground Cheat", &cheat::groundCheat);
				if (cheat::groundCheat)
					KeyBind::Hotkey("Hotkey: ", &cheat::groundCheatHotkey);
				ImGui::Checkbox("Time Stop", &cheat::timeStop);
				if (cheat::timeStop)
					KeyBind::Hotkey("Time Stop hotkey: ", &cheat::timeStopHotkey);

				ImGui::Checkbox("Change Players", &cheat::PlayerIsSam);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Enemies"))
			{
				ImGui::Checkbox("One Hit Kill", &cheat::oneHitKill);
				ImGui::Checkbox("Deal Zero Damage", &cheat::dealZeroDamage);

				ImGui::InputScalar("Boss HP", ImGuiDataType_U32, &cheat::BossHPModify);

				if (ImGui::Button("Change boss HP")) {
					cheat::ChangeBossHP(cheat::BossHPModify);
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Battle"))
			{
				ImGui::Checkbox("No Damage Status for battle", &cheat::noDamageStat);
				ImGui::Checkbox("Stealth Cheat", &cheat::stealth);
				ImGui::Checkbox("Infinite battle/VR timer", &cheat::infTimer);
				ImGui::Checkbox("Show battle stats", &gui::ShowBattleStats);
				ImGui::EndTabItem();
			}



			/*************************** MISSIONS ***************************/
			if (ImGui::BeginTabItem("Missions"))
			{

				/*************************** MISSION SELECT ***************************/
				if (ImGui::BeginCombo("Mission", selectedMission))
				{
					for (int n = 0; n < IM_ARRAYSIZE(missions); n++)
					{
						bool is_selected = (selectedMission == missions[n]);
						if (ImGui::Selectable(missions[n], is_selected)) {

							selectedMission = missions[n];
							selectedMissionID = n;

							selectedPhase = 0;
							selectedPhaseId = phases[selectedMissionID][0];

							selectedPhaseName = subPhases[selectedMissionID][0][0];
							selectedPhaseN = 0;
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				/*************************** MISSION SELECT END ***************************/


				/*************************** PHASE SELECT ***************************/

				if (ImGui::BeginCombo("Phase", selectedPhaseId))
				{
					for (int n = 0; n < IM_ARRAYSIZE(phases[selectedMissionID]); n++)
					{
						if (phases[selectedMissionID][n] == "" || phases[selectedMissionID][n] == NULL) break;

						bool is_selected = (selectedPhaseId == phases[selectedMissionID][n]);
						if (ImGui::Selectable(phases[selectedMissionID][n], is_selected)) {

							selectedPhaseId = phases[selectedMissionID][n];
							selectedPhase = n;

							selectedPhaseName = subPhases[selectedMissionID][selectedPhase][0];
							selectedPhaseN = 0;
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				/*************************** PHASE SELECT END ***************************/


				/*************************** SUBPHASE SELECT ***************************/

				if (ImGui::BeginCombo("Sub Phase", selectedPhaseName))
				{
					for (int n = 0; n < IM_ARRAYSIZE(subPhases[selectedMissionID][selectedPhase]); n++)
					{

						if (subPhases[selectedMissionID][selectedPhase][n] == "" || subPhases[selectedMissionID][selectedPhase][n] == NULL) break;

						bool is_selected = (selectedPhaseName == subPhases[selectedMissionID][selectedPhase][n]);

						if (ImGui::Selectable(subPhases[selectedMissionID][selectedPhase][n], is_selected)) {

							selectedPhaseName = subPhases[selectedMissionID][selectedPhase][n];
							selectedPhaseN = n;

						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				/*************************** SUBPHASE SELECT END***************************/


				/*************************** FUNCTIONS FOR SELECT ***************************/
				ImGui::Checkbox("Is DLC Phase (for Sam and Wolf only!)", &isDLCPhase);

				if (ImGui::Button("Change mission")) {
					cheat::ChangeMission(std::strtoul(selectedPhaseId, nullptr, 16), selectedPhaseName, isDLCPhase);
				}

				ImGui::EndTabItem();
			}
			/*************************** MISSIONS END ***************************/


			if (ImGui::BeginTabItem("Menu"))
			{
				KeyBind::Hotkey("Menu Key: ", &menuKey);
				if (ImGui::Checkbox("Pause game when shown", &usesPause) && g_GameMenuStatus == InGame)
				{
						Trigger::StaFlags.STA_PAUSE = usesPause;
						Trigger::StpFlags.STP_GAME_UPDATE = usesPause;
						Trigger::StpFlags.STP_MOUSE_UPDATE = !usesPause;
						Trigger::StpFlags.STP_PL_CAM_KEY = !usesPause;
						Trigger::StpFlags.STP_PL_ATTACK_KEY = !usesPause;
				}
				if (ImGui::Button("Save Config"))
				{
					cheat::SaveConfig();
					SaveConfig();
				}
				if (ImGui::Button("Load Config"))
				{
					cheat::LoadConfig();
					LoadConfig();
				}
				if (ImGui::Button("Reset Config"))
				{
					cheat::Reset();
					Reset();

					cheat::SaveConfig();
					SaveConfig();
				}
				if (ImGui::Button("Donate"))
					ShellExecute(0, "open", "https://donatello.to/Frouk3", NULL, NULL, 0);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void gui::SaveConfig() noexcept
{
	CIniReader iniReader("ModMenu.ini");

	iniReader.WriteInteger("Menu", "OpenMenuHotkey", menuKey);
	iniReader.WriteInteger("Menu", "PauseGameWhenMenu", usesPause);
	iniReader.WriteInteger("Menu", "ShowBattleStats", ShowBattleStats);
}

void gui::LoadConfig() noexcept
{
	CIniReader iniReader("ModMenu.ini");

	menuKey = iniReader.ReadInteger("Menu", "OpenMenuHotkey", 45);
	usesPause = iniReader.ReadInteger("Menu", "PauseGameWhenMenu", 1) == 1;
	ShowBattleStats = iniReader.ReadInteger("Menu", "ShowBattleStats", 0) == 1;
}

// Resets gui variables
void gui::Reset() noexcept
{
	width = 900.0f;
	height = 600.0f;
	menuKey = 45;
	usesPause = true;
	ShowBattleStats = false;
}

void gui::RenderBattleStatsWindow()
{
	if (!ShowBattleStats)
		return;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar;

	if (!show)
		windowFlags |= ImGuiWindowFlags_NoInputs;

	ImGui::Begin("Battle Stats", NULL, windowFlags);
	ImGui::LabelText("Alerts", "%d", *(int*)(shared::base + 0x1776174));
	float fBattleTimer = *(float*)(shared::base + 0x1776204);
	float fBattleSeconds = fBattleTimer;
	float fBattleMinutes = fBattleSeconds / 60.0f;
	ImGui::LabelText("Battle Timer", "%02.0f:%02.0f", floor(fBattleMinutes), floor(fmod(fBattleSeconds, 60.0f)));
	ImGui::LabelText("In Battle", "%s", *(bool*)(shared::base + 0x1777DE0) ? "true" : "false");
	ImGui::LabelText("Took Damage", "%s", *(bool*)(shared::base + 0x1776218) ? "true" : "false");
	ImGui::End();
}