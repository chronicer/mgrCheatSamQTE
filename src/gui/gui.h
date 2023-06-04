#pragma once

namespace gui
{
	inline float width = 900.0f;
	inline float height = 600.0f;
	inline unsigned int menuKey = 45;
	inline bool show = false;
	inline bool usesPause = true;
	inline bool ShowBattleStats = false;

	void RenderGUI() noexcept;
	void LoadConfig() noexcept;
	void Reset() noexcept;
	void SaveConfig() noexcept;
	void RenderBattleStatsWindow();
}