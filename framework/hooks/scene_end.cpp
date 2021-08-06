#include "scene_end.hpp"
#include "../features/chams.hpp"

decltype(&hooks::scene_end) hooks::original::scene_end = nullptr;

void __fastcall hooks::scene_end(void* ecx, void* edx) {
	original::scene_end(ecx, edx);

	features::glow::run();

}