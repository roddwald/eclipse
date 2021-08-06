#pragma once
#include "../utils/padding.hpp"
#include "vec3.hpp"

#define in_attack		(1 << 0)
#define in_jump			(1 << 1)
#define in_duck			(1 << 2)
#define in_forward		(1 << 3)
#define in_back			(1 << 4)
#define in_use			(1 << 5)
#define in_cancel		(1 << 6)
#define in_left			(1 << 7)
#define in_right		(1 << 8)
#define in_moveleft		(1 << 9)
#define in_moveright	(1 << 10)
#define in_attack2		(1 << 11)
#define in_run			(1 << 12)
#define in_reload		(1 << 13)
#define in_alt1			(1 << 14)
#define in_alt2			(1 << 15)
#define in_score		(1 << 16)   // used by client.dll for when scoreboard is held down
#define in_speed		(1 << 17)	// player is holding the speed key
#define in_walk			(1 << 18)	// player holding walk key
#define in_zoom			(1 << 19)	// zoom key for hud zoom
#define in_weapon1		(1 << 20)	// weapon defines these bits
#define in_weapon2		(1 << 21)	// weapon defines these bits
#define in_bullrush		(1 << 22)
#define in_grenade1		(1 << 23)	// grenade 1
#define in_grenade2		(1 << 24)	// grenade 2
#define	in_attack3		(1 << 25)


struct ucmd_t {
	PAD( 4 );
	int	m_cmdnum;
	int m_tickcount;
	vec3_t m_angs;
	vec3_t m_aimdir;
	float m_fmove;
	float m_smove;
	float m_umove;
	int m_buttons;
	std::uint8_t m_impulse;
	int m_weaponselect;
	int m_weaponsubtype;
	int m_randseed;
	short m_mousedx;
	short m_mousedy;
	bool m_hasbeenpredicted;
	PAD( 24 );

	void reset( ) {
		this->m_cmdnum = 0;
		this->m_tickcount = 0;
		this->m_angs.init( );
		this->m_fmove = 0.0f;
		this->m_smove = 0.0f;
		this->m_umove = 0.0f;
		this->m_buttons = 0;
		this->m_impulse = 0;
		this->m_weaponselect = 0;
		this->m_weaponsubtype = 0;
		this->m_randseed = 0;
		this->m_mousedx = 0;
		this->m_mousedy = 0;
		this->m_hasbeenpredicted = false;
	}
};