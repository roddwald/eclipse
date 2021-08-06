#include "../utils/vfunc.hpp"

class c_input_system {
public:
	void enable_input(bool input_enabled)
	{
		typedef void(__thiscall* OriginalFn)(void*, bool);
		return call_virtual< OriginalFn >(this, 11)(this, input_enabled);
	}
};