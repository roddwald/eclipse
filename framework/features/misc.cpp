#include "misc.hpp"
#include "esp.hpp"

void features::misc::bhop(ucmd_t* ucmd) {
	static auto& option_bhop = options::vars["misc.bhop"].val.b;

	if (!option_bhop)
		return;

	if (options::vars["misc.jumpbug"].val.b == true && GetAsyncKeyState(options::vars["misc.jumpbug.key"].val.i)) // allows us to bhop whilst jumpbugging by binding +jump to our bhop key
		return;

	if (!g::local || !g::local->alive() || g::local->movetype() == movetypes::movetype_ladder || g::local->movetype() == movetypes::movetype_noclip)
		return;

	if (!(g::local->flags() & FL_ONGROUND))
		ucmd->m_buttons &= ~in_jump;
}

void features::misc::duck_stamina(ucmd_t* ucmd) {
	static auto& option_unlimited_duck_stamina = options::vars["misc.unlimited_duck_stamina"].val.b;

	if (option_unlimited_duck_stamina)
		ucmd->m_buttons |= in_bullrush;
}

void features::misc::edgebug(ucmd_t* ucmd, int unpredicted_flags, int post_flags, float unpredicted_velocity) {
	if (options::vars["misc.edgebug"].val.b && GetAsyncKeyState(options::vars["misc.edgebug.key"].val.i)) {
		bool caneb = !(unpredicted_velocity >= -7 || floor(g::local->vel().z) != -7 || (g::local->flags() & FL_ONGROUND));
		const bool shouldduck = !(unpredicted_flags & FL_ONGROUND) && post_flags & FL_ONGROUND;
		if (shouldduck && !caneb) ucmd->m_buttons |= in_duck;

		else if (shouldduck && caneb) ucmd->m_buttons &= ~in_duck;

		if (caneb) {
			csgo::i::input_sys->enable_input(false);
		}
		else {
			csgo::i::input_sys->enable_input(true);
		}
	}
	csgo::i::input_sys->enable_input(true); // probably not the best way, idc..
}

void features::misc::jumpbug(ucmd_t* ucmd, int unpredicted_flags) {
	if (options::vars["misc.jumpbug"].val.b && GetAsyncKeyState(options::vars["misc.jumpbug.key"].val.i)) {

		if (g::local->flags() & FL_ONGROUND)
		{
			ucmd->m_buttons &= ~in_jump;
			if (!(unpredicted_flags & FL_ONGROUND))
				ucmd->m_buttons |= in_duck;
		}
	}
}

void features::misc::aspect_ratio() {
	static const auto aspectratio = csgo::i::cvar->find_var("r_aspectratio");
	if (options::vars["misc.aspect_ratio.enable"].val.b)
		aspectratio->set_value(options::vars["misc.aspect_ratio"].val.i / 10.0f);
	else
		aspectratio->set_value(0);
}

void features::misc::edgebug_detection(ucmd_t* ucmd, float unpredicted_velocity) {
	static auto& option_eb_detect = options::vars["misc.edgebug_detection"].val.b;
	static auto& option_jumpbug = options::vars["misc.jumpbug"].val.b;
	static auto& option_edgebug = options::vars["misc.edgebug"].val.b;
	static auto& option_jumpbug_key = options::vars["misc.jumpbug.key"].val.i;
	static auto& option_edgebug_key = options::vars["misc.edgebug.key"].val.i;

	if (!option_eb_detect)
		return;

	if (!g::local || !g::local->alive() || g::local->movetype() == movetypes::movetype_ladder || g::local->movetype() == movetypes::movetype_noclip)
		return;

	float cooldown = 0.f;

	auto predicted_velocity = std::floor(g::local->vel().z);
	bool did_edgebug;
	if (unpredicted_velocity >= -7 || predicted_velocity != -7 || g::local->flags() & 1) {
		did_edgebug = false;
	}
	else {
		if ((!(GetAsyncKeyState(option_edgebug_key)) && (!(GetAsyncKeyState(option_jumpbug_key))) && csgo::i::globals->m_realtime > cooldown)) {
			csgo::i::hud_chat->chat_print(_(" \x08 [\x01 eclipse \x08] \x01 | edgebugged"));
			cooldown = csgo::i::globals->m_realtime + 0.5;
			did_edgebug = true;
		}

		if (option_edgebug && GetAsyncKeyState(option_edgebug_key) && csgo::i::globals->m_realtime > cooldown) {
			csgo::i::hud_chat->chat_print(_(" \x08 [\x01 eclipse \x08] \x01 | edgebugged (assisted)"));
			cooldown = csgo::i::globals->m_realtime + 0.5;
			did_edgebug = true;
		}

		if ((GetAsyncKeyState(option_jumpbug_key) && !(GetAsyncKeyState(option_edgebug_key)) && csgo::i::globals->m_realtime > cooldown)) {
			csgo::i::hud_chat->chat_print(_(" \x08 [\x01 eclipse \x08] \x01 | edgebugged (jumpbugged)"));
			cooldown = csgo::i::globals->m_realtime + 0.5;
			did_edgebug = true;
		}
	}
}

void features::misc::no_flash() {
	if (!g::local)
		return;

	static auto& option_no_flash = options::vars["misc.no_flash"].val.b;

	if (option_no_flash && g::local)
		g::local->flash_duration() = 0.f;
}

void features::misc::no_smoke() {
	static auto& option_no_smoke = options::vars["misc.no_smoke"].val.b;

	static bool set = true;
	static std::vector<const char*> smoke_materials =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	if (!option_no_smoke)
	{
		if (set)
		{
			for (auto material_name : smoke_materials)
			{
				auto mat = csgo::i::mat_sys->findmat(material_name, TEXTURE_GROUP_OTHER);
				mat->set_material_var_flag(MATERIAL_VAR_WIREFRAME, false);
			}
			set = false;
		}
		return;
	}

	set = true;
	for (auto material_name : smoke_materials)
	{
		auto mat = csgo::i::mat_sys->findmat(material_name, TEXTURE_GROUP_OTHER);
		mat->set_material_var_flag(MATERIAL_VAR_WIREFRAME, true);
		static auto smokecount = pattern::search(client_dll, _("A3 ? ? ? ? 57 8B CB")).add(1).deref().get< std::uintptr_t* >();
		if (option_no_smoke)
			*smokecount = 0;
	}
}

void features::misc::sniper_crosshair() {
	static auto& option_sniper_crosshair = options::vars["misc.sniper_crosshair"].val.b;

	if (!g::local || !csgo::i::cvar)
		return;

	static auto cross = csgo::i::cvar->find_var("weapon_debug_spread_show");
	cross->set_value(option_sniper_crosshair && !g::local->scoped() ? 3 : 0);
}

void features::misc::spectator_list() {
	if (!options::vars["misc.spectatorlist"].val.b)
		return;
	if (!g::local && g::local->alive() && csgo::i::engine->is_in_game() && csgo::i::engine->is_connected())
		return;
	int spectator_index = 0;
	int screen_w, screen_h;
	csgo::i::engine->get_screen_size(screen_w, screen_h);
	for (int i = 1; i < csgo::i::globals->m_max_clients; i++) {
		auto local_player = reinterpret_cast<player_t*>(csgo::i::ent_list->get<player_t*>(csgo::i::engine->get_local_player()));
		if (!local_player)
			return;
		auto entity = reinterpret_cast<player_t*>(csgo::i::ent_list->get<player_t*>(i));
		if (!entity)
			return;
		player_info_t info;
		if (entity && entity != local_player) {
			csgo::i::engine->get_player_info(i, &info);
			if (!entity->alive() && !entity->dormant()) {
				auto target = entity->observertarget();
				if (!target)
					return;
				if (target) {
					auto spectator_target = csgo::i::ent_list->get_by_handle<player_t*>(target);
					if (spectator_target == local_player) {
						std::string player_name = info.m_name;
						player_info_t spectator_info;
						csgo::i::engine->get_player_info(i, &spectator_info);
						auto textsize = draw::get_text_size_vec(visual_fonts.watermark_font, player_name.c_str());
						draw::string(screen_w - textsize.x - 5, 5 + (15 * spectator_index), color(255, 255, 255, 255), visual_fonts.watermark_font, false, player_name.c_str());
						spectator_index++;
					}
				}
			}
		}
	}
}
