#pragma once
#include "sdk.hpp"
#include "entity.hpp"
#include "../utils/padding.hpp"

// Generated using ReClass 2016

class weapon_info_t;
enum class weapons_t : uint16_t {
	none = 0,
	deagle = 1,
	elite = 2,
	fiveseven = 3,
	glock = 4,
	ak47 = 7,
	aug = 8,
	awp = 9,
	famas = 10,
	g3sg1 = 11,
	galil = 13,
	m249 = 14,
	m4a4 = 16,
	mac10 = 17,
	p90 = 19,
	mp5_sd = 23,
	ump45 = 24,
	xm1014 = 25,
	bizon = 26,
	mag7 = 27,
	negev = 28,
	sawedoff = 29,
	tec9 = 30,
	taser = 31,
	p2000 = 32,
	mp7 = 33,
	mp9 = 34,
	nova = 35,
	p250 = 36,
	scar20 = 38,
	sg553 = 39,
	ssg08 = 40,
	knife_ct = 42,
	flashbang = 43,
	hegrenade = 44,
	smoke = 45,
	molotov = 46,
	decoy = 47,
	firebomb = 48,
	c4 = 49,
	musickit = 58,
	knife_t = 59,
	m4a1s = 60,
	usps = 61,
	tradeupcontract = 62,
	cz75a = 63,
	revolver = 64,
	knife_bayonet = 500,
	knife_css = 503,
	knife_flip = 505,
	knife_gut = 506,
	knife_karambit = 507,
	knife_m9_bayonet = 508,
	knife_huntsman = 509,
	knife_falchion = 512,
	knife_bowie = 514,
	knife_butterfly = 515,
	knife_shadow_daggers = 516,
	knife_cord = 517,
	knife_canis = 518,
	knife_ursus = 519,
	knife_gypsy_jackknife = 520,
	knife_outdoor = 521,
	knife_stiletto = 522,
	knife_widowmaker = 523,
	knife_skeleton = 525,
	glove_studded_bloodhound = 5027,
	glove_t_side = 5028,
	glove_ct_side = 5029,
	glove_sporty = 5030,
	glove_slick = 5031,
	glove_leather_wrap = 5032,
	glove_motorcycle = 5033,
	glove_specialist = 5034,
	glove_studded_hydra = 5035
};

enum ItemDefinitionIndex : int {
	WEAPON_NONE = 0,

	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5_SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

enum weapon_type_t : uint32_t {
	knife = 0,
	pistol,
	smg,
	rifle,
	shotgun,
	sniper,
	lmg,
	c4,
	grenade,
	unknown
};

class weapon_info_t {
public:
	std::uint8_t pad_0x0000[0x4]; //0x0000
	char* m_weapon_name; //0x0004
	std::uint8_t pad_0x0008[0xC]; //0x0008
	std::uint32_t m_max_clip; //0x0014
	std::uint8_t pad_0x0018[0x68]; //0x0018
	char* m_ammo_name; //0x0080
	char* m_ammo_name_2; //0x0084
	char* m_hud_name; //0x0088
	char* m_weapon_id; //0x008C
	std::uint8_t pad_0x0090[0x3C]; //0x0090
	weapon_type_t m_type; //0x00CC
	std::uint32_t m_price; //0x00D0
	std::uint32_t m_reward; //0x00D4
	std::uint8_t pad_0x00D8[0x14]; //0x00D8
	std::uint8_t m_full_auto; //0x00EC
	std::uint8_t pad_0x00ED[0x3]; //0x00ED
	std::uint32_t m_dmg; //0x00F0
	float m_armor_ratio; //0x00F4
	std::uint32_t m_bullets; //0x00F8
	float m_penetration; //0x00FC
	std::uint8_t pad_0x0100[0x8]; //0x0100
	float m_range; //0x0108
	float m_range_modifier; //0x010C
	std::uint8_t pad_0x0110[0x20]; //0x0110
	float m_max_speed; //0x0130
	float m_max_speed_alt; //0x0134
	std::uint8_t pad_0x0138[0x108]; //0x0138

	bool is_gun( ) {
		std::string m_weapon_name = this->m_weapon_name;

		if ( m_weapon_name == "weapon_knife" )
			return false;
		else if ( m_weapon_name == "weapon_incgrenade" )
			return false;
		else if ( m_weapon_name == "weapon_decoy" )
			return false;
		else if ( m_weapon_name == "weapon_flashbang" )
			return false;
		else if ( m_weapon_name == "weapon_hegrenade" )
			return false;
		else if ( m_weapon_name == "weapon_smokegrenade" )
			return false;
		else if ( m_weapon_name == "weapon_molotov" )
			return false;
		return true;
	}
}; //Size=0x0240

class inferno_t {
public:
	NETVAR( int, get_effect_tick_begin, "DT_Inferno->m_nFireEffectTickBegin" );

	inline float get_max_time( ) {
		return 7.f;
	}
};

class weapon_t : public entity_t {
public:
	NETVAR(weapons_t, item_definition_index, "DT_BaseAttributableItem->m_iItemDefinitionIndex");
	NETVAR(float, next_primary_attack, "DT_BaseCombatWeapon->m_flNextPrimaryAttack");
	NETVAR(float, next_secondary_attack, "DT_BaseCombatWeapon->m_flNextSecondaryAttack");
	NETVAR(float, throw_strength, "DT_BaseCSGrenade->m_flThrowStrength");
	NETVAR(int, ammo, "DT_BaseCombatWeapon->m_iClip1");
	NETVAR(int, ammo2, "DT_BaseCombatWeapon->m_iClip2");
	NETVAR(bool, is_burst_mode, "CWeaponCSBase->m_bBurstMode");
	NETVAR(int, sequence, "DT_BaseViewModel->m_nSequence");
	NETVAR(float, postpone_fire_time, "DT_BaseCombatWeapon->m_flPostponeFireReadyTime");
	NETVAR(std::uint64_t, world_model_handle, "DT_BaseCombatWeapon->m_hWeaponWorldModel");
	NETVAR(float, throw_time, "DT_BaseCSGrenade->m_fThrowTime");
	NETVAR(uint32_t, owner_handle, "DT_BasePlayer->m_hOwner");
	NETVAR(bool, pin_pulled, "DT_BaseCSGrenade->m_bPinPulled");
	NETVAR(std::uint64_t, original_owner_xuid, "DT_BaseAttributableItem->m_OriginalOwnerXuidLow");
	NETVAR(std::uint32_t, original_owner_xuid_low, "DT_BaseAttributableItem->m_OriginalOwnerXuidLow");
	NETVAR(std::uint32_t, original_owner_xuid_high, "DT_BaseAttributableItem->m_OriginalOwnerXuidHigh");
	NETVAR(std::uint32_t, fallback_stattrak, "DT_BaseAttributableItem->m_nFallbackStatTrak");
	NETVAR(std::uint32_t, fallback_paintkit, "DT_BaseAttributableItem->m_nFallbackPaintKit");
	NETVAR(std::uint32_t, fallback_seed, "DT_BaseAttributableItem->m_nFallbackSeed");
	NETVAR(float, fallback_wear, "DT_BaseAttributableItem->m_flFallbackWear");
	NETVAR(std::uint32_t, fallback_quality, "DT_BaseAttributableItem->m_iEntityQuality");
	NETVAR(std::uint32_t, item_id_low, "DT_BaseAttributableItem->m_iItemIDLow");
	NETVAR(std::uint32_t, item_id_high, "DT_BaseAttributableItem->m_iItemIDHigh");
	NETVAR(std::uint32_t, account, "DT_BaseAttributableItem->m_iAccountID");
	NETVAR(const char*, name, "DT_BaseAttributableItem->m_szCustomName");
	NETVAR(int, mdl_idx, "DT_BaseAttributableItem->m_nModelIndex");
	NETVAR(float, last_shot_time, "DT_BaseCombatWeapon->m_fLastShotTime");
	NETVAR(float, cycle, "DT_BaseAnimating->m_flCycle");
	NETVAR(float, anim_time, "DT_BaseEntity->m_flAnimTime");
	NETVAR(int, zoom_level, "DT_WeaponCSBaseGun->m_zoomLevel");
	NETVAR(float, accuracy_penalty, "DT_WeaponCSBase->m_fAccuracyPenalty");
	NETVAR(float, recoil_index, "DT_WeaponCSBase->m_flRecoilIndex");

	weapon_info_t* data( );

	void update_accuracy( ) {
		using fn = void( __thiscall* )( void* );
		vfunc< fn >( this, 483 )( this );
	}

	float inaccuracy( );
	float spread( );
	float max_speed( );
};