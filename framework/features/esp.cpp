#include "esp.hpp"
#include <iomanip>
#include <sstream>

bool features::esp::create_bbox(player_t* pl, box_t* box) {
	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	int left, top, right, bottom;

	origin = pl->abs_origin();
	min = pl->mins() + origin;
	max = pl->maxs() + origin;

	vec3_t points[] = {
	  vec3_t(min.x, min.y, min.z),
	  vec3_t(min.x, max.y, min.z),
	  vec3_t(max.x, max.y, min.z),
	  vec3_t(max.x, min.y, min.z),
	  vec3_t(max.x, max.y, max.z),
	  vec3_t(min.x, max.y, max.z),
	  vec3_t(min.x, min.y, max.z),
	  vec3_t(max.x, min.y, max.z)
	};

	if (!csgo::render::world_to_screen(flb, points[3]) || !csgo::render::world_to_screen(brt, points[5]) ||
		!csgo::render::world_to_screen(blb, points[0]) || !csgo::render::world_to_screen(frt, points[4]) ||
		!csgo::render::world_to_screen(frb, points[2]) || !csgo::render::world_to_screen(brb, points[1]) ||
		!csgo::render::world_to_screen(blt, points[6]) || !csgo::render::world_to_screen(flt, points[7]))
		return false;

	vec3_t arr[] = {
	  flb,
	  brt,
	  blb,
	  frt,
	  frb,
	  brb,
	  blt,
	  flt
	};

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	box->x = (int)left;
	box->y = (int)top;
	box->w = int(right - left);
	box->h = int(bottom - top);

	return true;
}

void features::esp::paint() {
	static auto& option_dormant = options::vars["esp.enable"].val.b;
	static auto& option_box_color = options::vars["esp.box_color"].val.c;
	static auto& option_box = options::vars["esp.box"].val.b;
	static auto& option_name_color = options::vars["esp.name_color"].val.c;
	static auto& option_name = options::vars["esp.enable"].val.b;
	static auto& option_health = options::vars["esp.enable"].val.b;
	static auto& option_flags = options::vars["esp.enable"].val.b;
	static auto& option_ammo_color = options::vars["esp.ammo_color"].val.c;
	static auto& option_ammo = options::vars["esp.enable"].val.b;
	static auto& option_weapon = options::vars["esp.enable"].val.b;
	static auto& option_skeleton_color = options::vars["esp.skeleton_color"].val.c;
	static auto& option_skeleton = options::vars["esp.skeleton"].val.b;
	static auto& option_custom_hp_color = options::vars["esp.custom_hp_color"].val.b;
	static auto& option_health_color = options::vars["esp.health_color"].val.c;

	watermark();
	indicators();
	velocity_indicator();
	arrows(g::ucmd);

	player_t* local = csgo::i::ent_list->get<player_t*>(csgo::i::engine->get_local_player()); // get local player
	for (int i = 1; i <= csgo::i::globals->m_max_clients; i++) {
		player_t* pl = csgo::i::ent_list->get<player_t*>(i);

		if (!pl
			|| !pl->alive()
			|| pl->team() == local->team()
			)
			continue;

		// let's prepare to store info
		player_info_t player_info;
		if (!csgo::i::engine->get_player_info(i, &player_info))
			continue;

		auto weapon = pl->weapon();

		if (!weapon)
			continue;

		auto data = weapon->data();

		if (!data)
			continue;

		offset.flags = 0;
		offset.ammo = 0;

		auto wstr = std::wstring(csgo::i::localize->find(data->m_hud_name));
		auto str = std::string(wstr.begin(), wstr.end());
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);

		// store info for our esp data displayed
		esp_info[pl->idx()].name = player_info.m_name;
		esp_info[pl->idx()].weapon = str.c_str();
		esp_info[pl->idx()].health = pl->health();
		esp_info[pl->idx()].ammo = pl->weapon()->ammo();
		esp_info[pl->idx()].armor = pl->armor();
		esp_info[pl->idx()].account = pl->account();
		esp_info[pl->idx()].has_heavy_armor = pl->has_heavy_armor();
		esp_info[pl->idx()].has_helmet = pl->has_helmet();
		esp_info[pl->idx()].dormant = pl->dormant();
		esp_info[pl->idx()].scoped = pl->scoped();
		esp_info[pl->idx()].opacity = esp_info[pl->idx()].dormant ? 140 : 200;

		// check related stuff
		if (!option_dormant && esp_info[pl->idx()].dormant)
			continue;

		// paint
		box_t box;
		if (create_bbox(pl, &box)) {
			if (option_box) {
				color col = color(option_box_color, esp_info[pl->idx()].opacity);

				draw::outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, color(0, 0, 0, esp_info[pl->idx()].opacity));
				draw::outline(box.x, box.y, box.w, box.h, col);
				draw::outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, color(0, 0, 0, esp_info[pl->idx()].opacity));
			}

			if (option_flags) {
				auto flag = [&](color col, const char* name) -> void {
					draw::string(box.x + box.w + 3, box.y + offset.flags, color(col, esp_info[pl->idx()].opacity), visual_fonts.small_font, false, name);
					offset.flags += draw::get_text_size(visual_fonts.small_font, name).bottom - 1;
				};

				if (option_flags) {
					if (esp_info[pl->idx()].armor > 0) {
						if (esp_info[pl->idx()].has_helmet)
							flag(color(255, 255, 255), "HK");
						else
							flag(color(255, 255, 255), "K");
					}
				}

				if (option_flags) {
					char buffer[256];
					sprintf_s(buffer, "$%i", esp_info[pl->idx()].account);
					flag(color(133, 187, 101), buffer);
				}

				if (option_flags) {
					if (esp_info[pl->idx()].scoped)
						flag(color(74, 128, 255), "ZOOM");
				}

				if (option_flags) {
					if (pl->has_defuser())
						flag(color(135, 177, 250), "KIT");
				}

				if (option_flags) {
					animlayer_t* layer = &pl->overlay()[1];

					if (layer->m_weight != 0.f && pl->get_sequence_activity(layer->m_sequence) == 967)
						flag(color(255, 255, 255), "RELOAD");
				}
			}

			if (option_name) {
				color col = color(255, 255, 255, esp_info[pl->idx()].opacity);
				draw::string(box.x + box.w / 2, box.y - 14, col, visual_fonts.watermark_font, true, esp_info[pl->idx()].name);
			}

			if (option_health) {
				int g = esp_info[pl->idx()].health * 2.55;
				int r = 255 - g;
				int b = 0;

				color col = option_custom_hp_color ? color(option_health_color, esp_info[pl->idx()].opacity) : color(r, g, b, esp_info[pl->idx()].opacity);
				int delta = esp_info[pl->idx()].health * box.h / 100;

				char buffer[255];
				sprintf_s(buffer, "%i", esp_info[pl->idx()].health);

				draw::rectangle(box.x - 7, box.y, 4, box.h, color(10, 10, 10, (esp_info[pl->idx()].opacity / 2)));
				draw::outline(box.x - 6, box.y + (box.h - delta) + 1, 2, delta - 1, col);

				if (esp_info[pl->idx()].health != 100)
					draw::string(box.x - 7, box.y + (box.h - delta) - 3, color(255, 255, 255, esp_info[pl->idx()].opacity), visual_fonts.small_font, true, buffer);
			}

			if (option_ammo) {
				animlayer_t* layer = &pl->overlay()[1];

				int delta = esp_info[pl->idx()].ammo * box.w / data->m_max_clip;
				if (layer->m_weight != 0.f && pl->get_sequence_activity(layer->m_sequence) == 967)
					delta = layer->m_cycle * box.w / 1;
				else
					delta = esp_info[pl->idx()].ammo * box.w / data->m_max_clip;

				color col = color(65, 105, 225, esp_info[pl->idx()].opacity);

				draw::rectangle(box.x, box.y + box.h + 2, box.w, 4, color(10, 10, 10, (esp_info[pl->idx()].opacity / 2)));
				draw::outline(box.x + 1, box.y + box.h + 3, delta - 2, 2, col);

				offset.ammo += 6;
			}

			if (option_skeleton && !esp_info[pl->idx()].dormant) {
				if (const auto mdl = pl->mdl(); mdl != nullptr) {
					if (const auto hdr = csgo::i::mdl_info->studio_mdl(mdl); hdr != nullptr) {
						for (int j = 0; j < hdr->m_numbones; j++) {
							mstudiobone_t* bone = hdr->bone(j);
							vec3_t screen_child, screen_parent;
							vec3_t child, parent;
							if (bone && (bone->m_flags & 0x00000100) && (bone->m_parent != -1)) {
								child = pl->get_bone_position(j);
								parent = pl->get_bone_position(bone->m_parent);

								vec3_t breast_bone;
								vec3_t upper_direction = pl->get_bone_position(7) - pl->get_bone_position(6);
								breast_bone = pl->get_bone_position(6) + upper_direction / 2;
								vec3_t delta_child = child - breast_bone;
								vec3_t delta_parent = parent - breast_bone;

								if ((delta_parent.length() < 9 && delta_child.length() < 9))
									parent = breast_bone;

								if (j == 5)
									child = breast_bone;

								if (abs(delta_child.z) < 5 && (delta_parent.length() < 5 && delta_child.length() < 5) || j == 6)
									continue;

								if (!csgo::render::world_to_screen(screen_parent, parent)
									|| !csgo::render::world_to_screen(screen_child, child))
									continue;

								draw::line(screen_parent[0], screen_parent[1], screen_child[0], screen_child[1], color(option_skeleton_color, esp_info[pl->idx()].opacity));
							}
						}
					}
				}
			}

			if (option_weapon) {
				color col = color(255, 255, 255, esp_info[pl->idx()].opacity);
				draw::string(box.x + box.w / 2, box.y + box.h + 1 + offset.ammo, col, visual_fonts.small_font, true, esp_info[pl->idx()].weapon);
			}
		}
	}
}

void features::esp::watermark() {
	static auto& option_watermark_enabled = options::vars["misc.watermark"].val.b;

	if (!option_watermark_enabled)
		return;

	auto fps = static_cast<int>(1.f / csgo::i::globals->m_abs_frametime);

	draw::string(78, 10, color(230, 230, 250, 255), visual_fonts.watermark_font, true, ("eclipse / stable / " + std::to_string(fps) + " fps").c_str());
}

void features::esp::indicators() {
	static auto& option_indicators = options::vars["misc.indicators"].val.b;
	static auto& option_jumpbug = options::vars["misc.jumpbug"].val.b;
	static auto& option_edgebug = options::vars["misc.edgebug"].val.b;
	static auto& option_edgejump = options::vars["misc.edgejump"].val.b;
	static auto& option_jumpbug_key = options::vars["misc.jumpbug.key"].val.i;
	static auto& option_edgebug_key = options::vars["misc.edgebug.key"].val.i;
	static auto& option_edgejump_key = options::vars["misc.edgejump.key"].val.i;

	if (option_indicators && g::local && g::local->alive() && csgo::i::engine->is_in_game() && csgo::i::engine->is_connected()) {
		if (option_jumpbug && GetAsyncKeyState(option_jumpbug_key)) {
			int screen_w, screen_h;
			csgo::i::engine->get_screen_size(screen_w, screen_h);
			draw::string(screen_w / 2 + 2, screen_h - 140, color(230, 230, 250, 255), visual_fonts.vel_font, true, "jb");
		}
		if (option_edgebug && GetAsyncKeyState(option_edgebug_key)) {
			int screen_w, screen_h;
			csgo::i::engine->get_screen_size(screen_w, screen_h);
			draw::string(screen_w / 2 + 2, screen_h - 110, color(230, 230, 250, 255), visual_fonts.vel_font, true, "eb");
		}
	}
}

int tick_prev = 0;

void features::esp::velocity_indicator() {
	static auto& velocity_indicator = options::vars["misc.velocity_indicator"].val.b;

	if (!velocity_indicator)
		return;

	if (!csgo::i::engine->is_connected() || !csgo::i::engine->is_in_game() || !g::local)
		return;

	int screen_w, screen_h;
	csgo::i::engine->get_screen_size(screen_w, screen_h);

	static int last_speed = 0, last_jump_speed = 0;
	static float last_vel_update = 0, jump_speed_duration = 0;
	int current_speed = g::local->vel().length_2d() + 0.5;
	static bool last_onground = false;
	bool current_onground = g::local->flags() & FL_ONGROUND;

	if (last_onground && !current_onground)
		last_jump_speed = current_speed;

	color c = current_speed == last_speed ? color(255, 199, 89, 255) : current_speed < last_speed ? color(255, 119, 119, 255) : color(30, 255, 109, 255);
	std::string str = std::to_string(current_speed);
	if (!current_onground) {
		jump_speed_duration = csgo::i::globals->m_curtime + 1.5f;
	}

	if (jump_speed_duration > csgo::i::globals->m_curtime)
		str += " (" + std::to_string(last_jump_speed) + ")";

	draw::string(screen_w / 2, screen_h - 200, c, visual_fonts.vel_font, true, str.c_str());

	if (tick_prev + 5 < csgo::i::globals->m_tickcount) {
		last_speed - current_speed;
		tick_prev = csgo::i::globals->m_tickcount;
	}
	if (csgo::i::globals->m_tickcount <= 320) {
		tick_prev = 0;
	}

	last_onground = current_onground;
}

void features::esp::arrows(ucmd_t* umcd) {
	static auto& arrows = options::vars["misc.arrows"].val.b;

	if (!arrows)
		return;

	if (!csgo::i::engine->is_connected() || !csgo::i::engine->is_in_game() || !g::local)
		return;

	int screen_w, screen_h;
	int x, y;
	csgo::i::engine->get_screen_size(screen_w, screen_h);

	x = screen_w / 2;
	y = screen_h / 2;

	draw::rectangle(x - 50, y - 10, 1, 21, color(0, 0, 0, 125));
	draw::rectangle(x - 51, y - 10 + 1, 1, 19, color(0, 0, 0, 125));
	draw::rectangle(x - 52, y - 10 + 2, 1, 17, color(0, 0, 0, 125));
	draw::rectangle(x - 53, y - 10 + 3, 1, 15, color(0, 0, 0, 125));
	draw::rectangle(x - 54, y - 10 + 4, 1, 13, color(0, 0, 0, 125));
	draw::rectangle(x - 55, y - 10 + 5, 1, 11, color(0, 0, 0, 125));
	draw::rectangle(x - 56, y - 10 + 6, 1, 9, color(0, 0, 0, 125));
	draw::rectangle(x - 57, y - 10 + 7, 1, 7, color(0, 0, 0, 125));
	draw::rectangle(x - 58, y - 10 + 8, 1, 5, color(0, 0, 0, 125));
	draw::rectangle(x - 59, y - 10 + 9, 1, 3, color(0, 0, 0, 125));
	draw::rectangle(x - 60, y - 10 + 10, 1, 1, color(0, 0, 0, 125));

	draw::rectangle(x + 50, y - 10, 1, 21, color(0, 0, 0, 125));
	draw::rectangle(x + 51, y - 10 + 1, 1, 19, color(0, 0, 0, 125));
	draw::rectangle(x + 52, y - 10 + 2, 1, 17, color(0, 0, 0, 125));
	draw::rectangle(x + 53, y - 10 + 3, 1, 15, color(0, 0, 0, 125));
	draw::rectangle(x + 54, y - 10 + 4, 1, 13, color(0, 0, 0, 125));
	draw::rectangle(x + 55, y - 10 + 5, 1, 11, color(0, 0, 0, 125));
	draw::rectangle(x + 56, y - 10 + 6, 1, 9, color(0, 0, 0, 125));
	draw::rectangle(x + 57, y - 10 + 7, 1, 7, color(0, 0, 0, 125));
	draw::rectangle(x + 58, y - 10 + 8, 1, 5, color(0, 0, 0, 125));
	draw::rectangle(x + 59, y - 10 + 9, 1, 3, color(0, 0, 0, 125));
	draw::rectangle(x + 60, y - 10 + 10, 1, 1, color(0, 0, 0, 125));

	if (g::ucmd->m_buttons & in_moveleft) {
		draw::rectangle(x - 50, y - 10, 1, 21, color(255, 255, 255, 200));
		draw::rectangle(x - 51, y - 10 + 1, 1, 19, color(255, 255, 255, 200));
		draw::rectangle(x - 52, y - 10 + 2, 1, 17, color(255, 255, 255, 200));
		draw::rectangle(x - 53, y - 10 + 3, 1, 15, color(255, 255, 255, 200));
		draw::rectangle(x - 54, y - 10 + 4, 1, 13, color(255, 255, 255, 200));
		draw::rectangle(x - 55, y - 10 + 5, 1, 11, color(255, 255, 255, 200));
		draw::rectangle(x - 56, y - 10 + 6, 1, 9, color(255, 255, 255, 200));
		draw::rectangle(x - 57, y - 10 + 7, 1, 7, color(255, 255, 255, 200));
		draw::rectangle(x - 58, y - 10 + 8, 1, 5, color(255, 255, 255, 200));
		draw::rectangle(x - 59, y - 10 + 9, 1, 3, color(255, 255, 255, 200));
		draw::rectangle(x - 60, y - 10 + 10, 1, 1, color(255, 255, 255, 200));
	}
 
	if (g::ucmd->m_buttons& in_moveright) {
		draw::rectangle(x + 50, y - 10, 1, 21, color(255, 255, 255, 200));
		draw::rectangle(x + 51, y - 10 + 1, 1, 19, color(255, 255, 255, 200));
		draw::rectangle(x + 52, y - 10 + 2, 1, 17, color(255, 255, 255, 200));
		draw::rectangle(x + 53, y - 10 + 3, 1, 15, color(255, 255, 255, 200));
		draw::rectangle(x + 54, y - 10 + 4, 1, 13, color(255, 255, 255, 200));
		draw::rectangle(x + 55, y - 10 + 5, 1, 11, color(255, 255, 255, 200));
		draw::rectangle(x + 56, y - 10 + 6, 1, 9, color(255, 255, 255, 200));
		draw::rectangle(x + 57, y - 10 + 7, 1, 7, color(255, 255, 255, 200));
		draw::rectangle(x + 58, y - 10 + 8, 1, 5, color(255, 255, 255, 200));
		draw::rectangle(x + 59, y - 10 + 9, 1, 3, color(255, 255, 255, 200));
		draw::rectangle(x + 60, y - 10 + 10, 1, 1, color(255, 255, 255, 200));
	}
}