#include "draw_model_execute.hpp"
#include "../features/chams.hpp"

decltype(&hooks::draw_model_execute) hooks::original::draw_model_execute = nullptr;

void __fastcall hooks::draw_model_execute(void* ecx, void* edx, void* ctx, void* state, const mdlrender_info_t& info, matrix3x4_t* bone_to_world) {
	player_t* pl = csgo::i::ent_list->get<player_t*>(info.m_entity_index);

	if (csgo::i::mdl_render->is_forced_mat_override()) {
		original::draw_model_execute(ecx, edx, ctx, state, info, bone_to_world);
		return;
	}

	if (pl)
		features::chams::run(ecx, edx, ctx, state, info, bone_to_world);

	original::draw_model_execute(ecx, edx, ctx, state, info, bone_to_world);
	csgo::i::mdl_render->force_mat(nullptr);
}