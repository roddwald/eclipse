#pragma once

#include <intrin.h>

#include "../globals.hpp"

#include "../sdk/sdk.hpp"
#include "../utils/utils.hpp"

#include "../features/features.hpp"
#include "../features/prediction.hpp"

#include "../minhook/minhook.h"

static WNDPROC g_wndproc = nullptr;
namespace hooks {
	enum class vtable_idx : uintptr_t {
		endscene = 42 ,
		fireevent = 59 ,
		reset = 16 ,
		drawmodelexecute = 21 ,
		painttraverse = 41 ,
		sceneend = 9 ,
		listleavesinbox = 6 ,
	};

	bool init( );
}