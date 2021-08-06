#include "sdk.hpp"

class glow_object_definition_t {
public:
	int m_next_free_slot;
	entity_t* m_ent;
	vec3_t m_glow_color;
	float m_glow_alpha;
	PAD(8);
	float m_bloom_amount;
	float m_localplayeriszeropoint3;
	bool m_render_when_occluded;
	bool m_render_when_unoccluded;
	bool m_full_bloom_render;
	PAD(1);
	int m_full_bloom_stencil_test_value;
	int m_style;
	int m_split_screen_slot;

	void set(float r, float g, float b, float a, int style = 0) {
		m_glow_color = vec3_t(r, g, b);
		m_glow_alpha = a;
		m_render_when_occluded = true;
		m_render_when_unoccluded = false;
		m_bloom_amount = 1.0f;
		m_style = style;
	}

	bool should_draw(int slot) const {
		return get_entity() && (m_split_screen_slot == -1 || m_split_screen_slot == slot) && (m_render_when_occluded || m_render_when_unoccluded);
	}

	entity_t* get_entity() const {
		return m_ent;
	}

	bool is_unused() const {
		return m_next_free_slot != -2;
	}
};

class c_glow_obj_mgr {
public:
	glow_object_definition_t* m_glow_object_definitions;
	int m_max_size;
	int m_pad;
	int m_size;
	glow_object_definition_t* m_more_glow_object_definitions;
	int m_current_objects;
};