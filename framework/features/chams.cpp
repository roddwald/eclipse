#include "chams.hpp"
#include "../hooks/draw_model_execute.hpp"

// ill add bt chams at some point

struct cham_models_t {
	struct {
		material_t* m_textured;
		material_t* m_flat;
		material_t* m_glow;
	} enemy;

	struct {
		material_t* m_textured;
		material_t* m_flat;
		material_t* m_glow;
	} team;

	struct {
		material_t* m_textured;
		material_t* m_flat;
		material_t* m_glow;
	} local;
}; cham_models_t cham_models;

void features::glow::run() {
	static auto& option_glow = options::vars["esp.glow"].val.b;
	static auto& option_glow_enemy_color = options::vars["esp.enemy_glow_color"].val.c;
	static auto& option_glow_enemy = options::vars["esp.enemy_glow"].val.b;

	if (!option_glow)
		return;

	player_t* local = csgo::i::ent_list->get<player_t*>(csgo::i::engine->get_local_player());
	if (!local)
		return;

	for (auto i = 0; i < csgo::i::glow_obj_mgr->m_size; i++) {
		if (csgo::i::glow_obj_mgr->m_glow_object_definitions[i].is_unused() || !csgo::i::glow_obj_mgr->m_glow_object_definitions[i].get_entity())
			continue;

		auto& glow_object = csgo::i::glow_obj_mgr->m_glow_object_definitions[i];

		player_t* pl = reinterpret_cast<player_t*>(glow_object.m_ent);

		if (!pl
			|| !pl->valid()
			)
			continue;

		bool is_enemy = pl->team() != local->team() && pl != local;
		bool is_team = pl->team() == local->team() && pl != local;
		bool is_local = pl->team() == local->team() && pl == local;

		if (pl->client_class()->m_class_id == 40) {
			if (option_glow_enemy && is_enemy)
				glow_object.set(option_glow_enemy_color.r = 65.f, option_glow_enemy_color.g = 105.f, option_glow_enemy_color.b = 105.f, option_glow_enemy_color.a = 150.f);
		}
	}
}

bool create_materials() {
	auto ikv = [](void* kv, const char* name) {
		static auto ikv_fn = pattern::search(_("client.dll"), _("55 8B EC 51 33 C0 C7 45")).get< void(__thiscall*)(void*, const char*) >();
		ikv_fn(kv, name);
	};

	auto lfb = [](void* kv, const char* name, const char* buf) {
		using lfb_fn = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
		static auto lfb = pattern::search(_("client.dll"), _("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89")).get< lfb_fn >();
		lfb(kv, name, buf, nullptr, nullptr, nullptr, nullptr);
	};

	auto find_key = [](void* kv, const char* name, bool create) {
		using find_key_fn = void* (__thiscall*)(void*, const char*, bool);
		static auto findkey = pattern::search(_("client.dll"), _("55 8B EC 83 EC 1C 53 8B D9 85 DB")).get< find_key_fn >();
		return findkey(kv, name, create);
	};

	auto set_int = [find_key](void* kv, const char* name, int val, bool create = true) {
		auto k = find_key(kv, name, create);

		if (k) {
			*(int*)((uintptr_t)k + 0xC) = val;
			*(char*)((uintptr_t)k + 0x10) = 2;
		}
	};

	auto set_string = [find_key](void* kv, const char* name, const char* val, bool create = true) {
		auto k = find_key(kv, name, create);

		if (k) {
			using setstring_fn = void(__thiscall*)(void*, const char*);
			static auto setstring = pattern::search(_("client.dll"), _("55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01")).get< setstring_fn >();
			setstring(k, val);
		}
	};

	auto set_vec = [](void* var, float x, float y, float z) {
		vfunc< void(__thiscall*)(void*, float, float, float) >(var, 11)(var, x, y, z);
	};

	auto create_mat = [&](bool xqz, bool flat, bool wireframe, bool reflective, bool glow = false) {
		static auto created = 0;
		std::string type = flat ? _("UnlitGeneric") : _("VertexLitGeneric");

		auto kv = malloc(36);

		ikv(kv, type.c_str());

		if (!glow) {
			set_string(kv, _("$basetexture"), _("vgui/white_additive"));
			set_string(kv, _("$bumpmap"), _("de_nuke/hr_nuke/pool_water_normals_002"));
			set_string(kv, _("$bumptransform"), _("center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"));
			set_string(kv, _("$envmaptint"), _("[ 0 0 0 ]"));
			set_string(kv, _("$envmap"), _("env_cubemap"));
			set_int(kv, _("$phong"), 10);
			set_int(kv, _("$phongboost"), 0);
			set_int(kv, _("$pearlescent"), 0);
			set_int(kv, _("$phongexponent"), 15);
			set_int(kv, _("$normalmapalphaenvmask"), 1);
			set_string(kv, _("$phongboost"), _("[ 0 0 0 ]"));
			set_int(kv, _("$BasemapAlphaPhongMask"), 1);
			set_int(kv, _("$model"), 1);
			set_int(kv, _("$flat"), 1);
			set_int(kv, _("$selfillum"), 1);
			set_int(kv, _("$halflambert"), 1);
			set_int(kv, _("$ignorez"), 1);
			set_int(kv, _("$znearer"), 1);
		}
		else {
			set_int(kv, _("$additive"), 1);
			set_string(kv, _("$envmap"), "models/effects/cube_white");
			set_string(kv, _("$envmaptint"), "[1 1 1]");
			set_int(kv, _("$envmapfresnel"), 1);
			set_string(kv, _("$envmapfresnelminmaxexp"), "[0 1 2]");
			set_string(kv, _("$alpha"), "0.8");
		}

		if (reflective)
			set_string(kv, _("$reflectivity"), _("[ 0.9 0.9 0.9 ]"));

		auto matname = _("mat_") + std::to_string(created);
		auto mat = csgo::i::mat_sys->createmat(matname.c_str(), kv);
		//mat->increment_reference_count( );
		created++;

		return mat;
	};

	cham_models.enemy.m_flat = create_mat(false, true, false, false);
	cham_models.enemy.m_textured = create_mat(false, false, false, false);
	cham_models.enemy.m_glow = create_mat(false, false, false, false, true);
	cham_models.team.m_flat = create_mat(false, true, false, false);
	cham_models.team.m_textured = create_mat(false, false, false, false);
	cham_models.team.m_glow = create_mat(false, false, false, false, true);
	cham_models.local.m_flat = create_mat(false, true, false, false);
	cham_models.local.m_textured = create_mat(false, false, false, false);
	cham_models.local.m_glow = create_mat(false, false, false, false, true);

	return true;
}

void update_materials() {
	auto set_vec = [](void* var, float x, float y, float z) {
		vfunc< void(__thiscall*)(void*, float, float, float) >(var, 11)(var, x, y, z);
	};

	auto set_int = [](void* var, int val) {
		if (var) {
			*(int*)((uintptr_t)var + 0xC) = val;
			*(char*)((uintptr_t)var + 0x10) = 2;
		}
	};

	auto found = false;

	/* illuminate */
	static auto& option_illuminate_color_enemy = options::vars["chams.enemy.illuminate_color"].val.c;

	auto illuminate_enemy = cham_models.enemy.m_glow->find_var("$envmaptint", &found);

	set_vec(illuminate_enemy, option_illuminate_color_enemy.r / 255.f, option_illuminate_color_enemy.g / 255.f, option_illuminate_color_enemy.b / 255.f);

	/* pearlescent */
	auto pearlescent_enemy = cham_models.enemy.m_textured->find_var("$pearlescent", &found);

	static auto& option_pearlescent_enemy = options::vars["chams.enemy.pearlescent"].val.i;

	set_vec(pearlescent_enemy, option_pearlescent_enemy / 10.f, option_pearlescent_enemy / 10.f, option_pearlescent_enemy / 10.f);

	/* phong */
	static auto& option_phong_enemy = options::vars["chams.enemy.phong"].val.i;

	auto phongboost_enemy = cham_models.enemy.m_textured->find_var("$phongboost", &found);

	set_vec(phongboost_enemy, option_phong_enemy / 10.f, option_phong_enemy / 10.f, option_phong_enemy / 10.f);
}

void features::chams::run(void* ecx, void* edx, void* ctx, void* state, const mdlrender_info_t& info, matrix3x4_t* bone_to_world) {
	player_t* local = csgo::i::ent_list->get<player_t*>(csgo::i::engine->get_local_player());
	if (!local)
		return;

	player_t* pl = csgo::i::ent_list->get<player_t*>(info.m_entity_index);
	if (!pl)
		return;

	static auto& option_player_color_enemy = options::vars["chams.enemy.visible_color"].val.c;
	static auto& option_player_enemy = options::vars["chams.enemy.visible"].val.b;
	static auto& option_xqz_color_enemy = options::vars["chams.enemy.xqz_color"].val.c;
	static auto& option_xqz_enemy = options::vars["chams.enemy.xqz"].val.b;
	static auto& option_illuminate_enemy = options::vars["chams.enemy.illuminate"].val.b;
	static auto& option_illuminate_color_enemy = options::vars["chams.enemy.illuminate_color"].val.c;
	static auto& option_flat_enemy = options::vars["chams.enemy.flat"].val.b;

	auto draw_textured_material = [&](material_t* mat, color mat_color, bool ignorez) -> void {
		csgo::i::render_view->set_alpha(mat_color.a);
		csgo::i::render_view->set_color(mat_color.r, mat_color.g, mat_color.b);
		mat->set_material_var_flag(0x8000, ignorez);
		csgo::i::mdl_render->force_mat(mat);
		mat->increment_reference_count();
		hooks::original::draw_model_execute(ecx, edx, ctx, state, info, bone_to_world);
	};

	const auto& mdl_name = csgo::i::mdl_info->mdl_name(info.m_model);
	const bool& is_model = strstr(mdl_name, "models/player");

	if (!cham_models.local.m_flat
		|| !cham_models.local.m_textured
		|| !cham_models.local.m_glow
		)
		create_materials();

	update_materials();

	bool is_enemy = pl->team() != local->team() && pl != local;
	bool is_team = pl->team() == local->team() && pl != local;
	bool is_local = pl->team() == local->team() && pl == local;

	if ((is_model && pl->valid()) && is_enemy) {
		if (option_player_enemy)
			draw_textured_material(option_flat_enemy ? cham_models.enemy.m_flat : cham_models.enemy.m_textured, color(65, 105, 255), true);

		if (option_player_enemy)
			draw_textured_material(option_flat_enemy ? cham_models.enemy.m_flat : cham_models.enemy.m_textured, color(65, 105, 255), false);

		if (option_illuminate_enemy)
			draw_textured_material(cham_models.enemy.m_glow, color(65, 105, 255), true);
	}
}