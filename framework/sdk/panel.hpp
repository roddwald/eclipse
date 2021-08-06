#pragma once
#include "..\utils\vfunc.hpp"

class c_panel {
public:
	const char* get_name(int ipanel) {
		using getname_fn = const char* (__thiscall*)(void*, int);
		return vfunc< getname_fn >(this, 36)(this, ipanel);
	}
};