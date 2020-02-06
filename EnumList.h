#pragma once

enum class E_OS
{
	E_UNKNOWN,
	E_PLATFORM_IOS,                
	E_PLATFORM_ANDROID,            
	E_PLATFORM_WIN32,              
	E_PLATFORM_MARMALADE,          
	E_PLATFORM_LINUX,              
	E_PLATFORM_BADA,               
	E_PLATFORM_BLACKBERRY,         
	E_PLATFORM_MAC,                
	E_PLATFORM_NACL,               
	E_PLATFORM_EMSCRIPTEN,        
	E_PLATFORM_TIZEN,             
	E_PLATFORM_QT5,               
	E_PLATFORM_WINRT,
	E_MAX
};

enum class E_MENUITEM
{
	E_NORMAL,
	E_SELECT,
	E_DISABLE,
	E_MAX
};

enum class E_MENU_STATE
{
	E_NONE,
	E_NORMAL,
	E_SELECT,
	E_DISABLE,
	E_MAX
};

enum class E_MENU_SCG
{
	E_MYEONGZEONG,
	E_GONYANG,
	E_MAX
};

enum class E_MUSIC_ITEM
{
	E_MAINMUSIC,
	E_FIELD_01,
	E_FIELD_02,
	E_FIELD_03,
	E_FIELD_04,
	E_FIELD_05,
	E_FIELD_06,
	E_FIELD_EX,
	E_BOSS_01,
	E_BOSS_02,
	E_BOSS_03,
	E_BOSS_04,
	E_BOSS_05,
	E_BOSS_06,
	E_BOSS_EX,
	E_MAX
};

enum class E_DATA_TYPE
{
	E_IMG,
	E_SOUND,
	E_MAX
};

enum class E_USE_TYPE
{
	E_NOT_USED,
	E_USED,
	E_MAX
};

enum class E_HP_TYPE
{
	E_NORMAL,
	E_SPELL_01,
	E_SPELL_02,
	E_MAX
};

enum class E_HP_BAR_TYPE
{
	E_NORMAL,
	E_SPELL,
	E_LASTSPELL,
	E_MAX
};

enum class E_BOSS
{
	E_AYEON,
	E_MAX
};

enum class E_ENEMY_TYPE
{
	E_NORMAL,
	E_BOSS,
	E_SUMMONER,
	E_MAX
};