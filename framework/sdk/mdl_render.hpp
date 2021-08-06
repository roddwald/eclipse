#pragma once
#include "../utils/vfunc.hpp"
#include "../utils/padding.hpp"

class c_mdlrender {
public:
	void force_mat(class material_t* mat) {
		using forcedmatoverride_fn = void(__thiscall*)(void*, material_t*, void*, void*);
		vfunc< forcedmatoverride_fn >(this, 1)(this, mat, nullptr, nullptr);
	}

	bool is_forced_mat_override() {
		using forcedmatoverride_fn = bool(__thiscall*)(void*);
		return vfunc< forcedmatoverride_fn >(this, 2)(this);
	}
};