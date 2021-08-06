#pragma once
#include "../sdk/sdk.hpp"
#include "../globals.hpp"
#include "../menu/options.hpp"

struct fonts_t {
	unsigned long small_font;
	unsigned long big_font;
	unsigned long speed_font;
	unsigned long vel_font;
	unsigned long watermark_font;
	void init() {
		csgo::i::surface->set_font_glyph(big_font = csgo::i::surface->font_create(), "Segoe UI", 13, FW_MEDIUM, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		csgo::i::surface->set_font_glyph(small_font = csgo::i::surface->font_create(), "Small Fonts", 8, FW_NORMAL, 0, 0, FONTFLAG_OUTLINE);
		csgo::i::surface->set_font_glyph(speed_font = csgo::i::surface->font_create(), "Verdana", 26, FW_BOLD, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		csgo::i::surface->set_font_glyph(vel_font = csgo::i::surface->font_create(), "Verdana", 26, FW_BOLD, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		csgo::i::surface->set_font_glyph(watermark_font = csgo::i::surface->font_create(), "Tahoma", 12, FW_BOLD, 0, 0, FONTFLAG_DROPSHADOW);
	}
}; inline fonts_t visual_fonts;

struct box_t {
	box_t() = default;
	box_t(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	int x, y, w, h;
};

namespace features {
	namespace esp {
		bool create_bbox(player_t* pl, box_t* box);
		void paint();
		void watermark();
		void indicators();
		void velocity_indicator();
		void arrows(ucmd_t* ucmd);
		struct esp_info_t {
			const char* name;
			const char* weapon;
			int health;
			int ammo;
			int armor;
			bool has_heavy_armor;
			bool has_helmet;
			int account;
			bool dormant;
			bool scoped;
			int opacity;
		}; inline std::array< esp_info_t, 65> esp_info;

		struct {
			int flags;
			int ammo;
		} static offset;
	}
}