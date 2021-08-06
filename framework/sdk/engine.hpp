#pragma once
#include "../utils/vfunc.hpp"
#include "../utils/padding.hpp"
#include "matrix3x4.hpp"

class vec3_t;

struct player_info_t {
	std::uint64_t m_unknown;

	union {
		__int64 m_steam_id_64;

		struct {
			int m_xuid_low;
			int m_xuid_high;
		};
	};

	char m_name[128];
	int m_user_id;
	char m_str_steam_id[20];
	PAD(16);
	std::uintptr_t m_steam_id;
	char m_friends_name[128];
	bool m_fake_player;
	bool m_is_hltv;
	std::uintptr_t m_custom_files[4];
	std::uint8_t m_files_downloaded;
};

class nci_t {
public:
	virtual const char* get_name() const = 0;
	virtual const char* get_address() const = 0;
	virtual float get_time() const = 0;
	virtual float get_time_connected() const = 0;
	virtual int get_buffer_size() const = 0;
	virtual int get_data_rate() const = 0;
	virtual bool is_loopback() const = 0;
	virtual bool is_timing_out() const = 0;
	virtual bool is_playback() const = 0;
	virtual float get_latency(int flow) const = 0;
	virtual float get_avg_latency(int flow) const = 0;
};

class c_clientstate {
public:
	std::uint32_t& choked() {
		return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x4D30);
	}

	std::uint32_t& last_command_ack() {
		return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(&choked()) + 0x4);
	}

	std::uint32_t& last_outgoing_cmd() {
		return *reinterpret_cast<std::uint32_t*>(&choked() + sizeof(std::uint32_t));
	}

	std::uint32_t& server_tickcount() {
		return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x164);
	}

	std::uint32_t& delta_tick() {
		return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x174);
	}

	std::uint32_t& out_seq_num() {
		return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x4D24);
	}
};

class c_engine {
public:
	virtual int get_intersecting_surfaces(const void* model, const vec3_t& center, const float radius, const bool only_visible_surfaces, void* p_infos, const int max_infos) = 0;
	virtual vec3_t get_light_for_point(const vec3_t& pos, bool clamp) = 0;
	virtual void* trace_line_material_and_lighting(const vec3_t& start, const vec3_t& end, vec3_t& diffuse_light_color, vec3_t& base_color) = 0;
	virtual const char* parse_file(const char* data, char* token, int maxlen) = 0;
	virtual bool copy_file(const char* source, const char* destination) = 0;
	virtual void get_screen_size(int& width, int& height) = 0;
	virtual void server_cmd(const char* cmd, bool reliable = true) = 0;
	virtual void client_cmd(const char* cmd) = 0;
	virtual bool get_player_info(int ent_num, player_info_t* p_info) = 0;
	virtual int get_player_for_userid(int user_id) = 0;
	virtual void* text_message_get(const char* name) = 0;
	virtual bool con_is_visible() = 0;
	virtual int get_local_player() = 0;
	virtual const void* load_model(const char* name, bool prop = false) = 0;
	virtual float get_last_time_stamp() = 0;
	virtual void* get_sentence(void* audio_source) = 0;
	virtual float get_sentence_length(void* audio_source) = 0;
	virtual bool is_streaming(void* audio_source) const = 0;
	virtual void get_viewangles(vec3_t& angs) = 0;
	virtual void set_viewangles(vec3_t& angs) = 0;
	virtual int get_max_clients() = 0;
	virtual const char* key_lookup_binding(const char* binding) = 0;
	virtual const char* key_binding_for_key(int& code) = 0;
	virtual void key_set_binding(int, char const*) = 0;
	virtual void start_key_trap_mode() = 0;
	virtual bool check_done_key_trapping(int& code) = 0;
	virtual bool is_in_game() = 0;
	virtual bool is_connected() = 0;
	virtual bool is_drawing_loading_image() = 0;
	virtual void hide_loading_plaque() = 0;
	virtual void con_nprintf(int pos, const char* fmt, ...) = 0;
	virtual void con_nxprintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;
	virtual int is_box_visible(const vec3_t& mins, const vec3_t& maxs) = 0;
	virtual int is_box_in_view_cluster(const vec3_t& mins, const vec3_t& maxs) = 0;
	virtual bool cull_box(const vec3_t& mins, const vec3_t& maxs) = 0;
	virtual void sound_extra_update() = 0;
	virtual const char* get_game_directory() = 0;
	virtual const matrix3x4_t& world_to_screen_matrix() = 0;
	virtual const matrix3x4_t& world_to_view_matrix() = 0;
	virtual int game_lump_version(int lumpid) const = 0;
	virtual int game_lump_size(int lumpid) const = 0;
	virtual bool load_game_lump(int lumpid, void* buf, int size) = 0;
	virtual int level_leaf_count() const = 0;
	virtual void* get_bsp_tree_query() = 0;
	virtual void linear_to_gamma(float* linear, float* gamma) = 0;
	virtual float light_style_value(int style) = 0;
	virtual void compute_dynamic_lighting(const vec3_t& pt, const vec3_t* normal, vec3_t& color) = 0;
	virtual void get_ambient_light_color(vec3_t& color) = 0;
	virtual int get_dx_support_level() = 0;
	virtual bool supports_hdr() = 0;
	virtual void mat_stub(void* mat_sys) = 0;
	virtual void get_chapter_name(char* buf, int max_len) = 0;
	virtual char const* get_level_name() = 0;
	virtual char const* get_level_name_short() = 0;
	virtual char const* get_map_group_name() = 0;
	virtual void* get_voice_tweak_api() = 0;
	virtual void set_voice_caster_id(unsigned int id) = 0;
	virtual void enginestats_begin_frame() = 0;
	virtual void enginestats_end_frame() = 0;
	virtual void fire_events() = 0;
	virtual int get_leaves_area(unsigned short* p_leaves, int leaves) = 0;
	virtual bool does_box_touch_area_frustum(const vec3_t& mins, const vec3_t& maxs, int area) = 0;
	virtual int get_frustum_list(void** p_list, int list_max) = 0;
	virtual bool should_use_area_frustrum(int i) = 0;
	virtual void set_audio_state(const void* state) = 0;
	virtual int sentence_group_pick(int group_index, char* name, int name_buf_len) = 0;
	virtual int sentence_group_pick_sequential(int group_index, char* name, int name_buf_len, int sentence_index, int reset) = 0;
	virtual int sentence_index_from_name(const char* sentence_name) = 0;
	virtual const char* sentence_name_from_index(int sentence_index) = 0;
	virtual int sentence_group_index_from_name(const char* group_name) = 0;
	virtual const char* sentence_group_name_from_index(int group_index) = 0;
	virtual float sentence_length(int sentence_index) = 0;
	virtual void compute_lighting(const vec3_t& point, const vec3_t* normal, bool clamp, vec3_t& color, vec3_t* box_colors = nullptr) = 0;
	virtual void activate_occluder(int occluder_index, bool active) = 0;
	virtual bool is_occluded(const vec3_t& mins, const vec3_t& maxs) = 0;
	virtual int get_occlusion_view_id() = 0;
	virtual void* save_alloc_memory(size_t num, size_t size) = 0;
	virtual void save_free_memory(void* save_mem) = 0;
	virtual nci_t* get_net_channel_info() = 0;
	virtual void pad_func() = 0;
	virtual void pad_func1() = 0;
	virtual void pad_func2() = 0;
	virtual bool is_playing_demo() = 0;
	virtual void pad_func3() = 0;
	virtual void pad_func4() = 0;
	virtual void pad_func5() = 0;
	virtual void pad_func6() = 0;
	virtual void pad_func7() = 0;
	virtual void pad_func8() = 0;
	virtual void pad_func9() = 0;
	virtual void pad_func10() = 0;
	virtual void pad_func11() = 0;
	virtual void pad_func12() = 0;
	virtual bool is_hltv() = 0;

	void client_cmd_unrestricted(const char* cmd) {
		using clientcmd_unrestricted_fn = void(__thiscall*)(void*, const char*);
		vfunc< clientcmd_unrestricted_fn >(this, 108)(this, cmd);
	}
};