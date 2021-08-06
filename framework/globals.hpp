#pragma once

#include <windows.h>
#include "../framework/menu/config.hpp"

struct ucmd_t;
class vec3_t;
class player_t;

namespace g {
	inline HMODULE module = nullptr;
	inline player_t* local = nullptr;
	inline ucmd_t* ucmd = nullptr;
	inline bool send_packet = true;
}