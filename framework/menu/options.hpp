#pragma once
#include "../menu/menu.hpp"
#include <unordered_map>
#include <deque>

namespace options {
	enum class option_type_t : int {
		boolean = 0,
		list,
		integer,
		floating_point,
		string,
		color,
		deque
	};

	class option {
	public:
		option_type_t type;

		union val {
			bool b;
			int i;
			float f;
			char s[128];
			bool l[128];
			std::deque<bool> d;
			color c;
			std::string str;

			val() { }
			~val() { }
		} val;

		int list_size = 0;

		static void add_list(const std::string& id, int count);
		static void add_list(const std::string& id, std::deque<bool> count);
		static void add_bool(const std::string& id, bool val);
		static void add_int(const std::string& id, int val);
		static void add_float(const std::string& id, float val);
		static void add_str(const std::string& id, const char* val);
		static void add_color(const std::string& id, const color& val);

		static void add_script_list(const std::string& id, int count);
		static void add_script_bool(const std::string& id, bool val);
		static void add_script_int(const std::string& id, int val);
		static void add_script_float(const std::string& id, float val);
		static void add_script_str(const std::string& id, const char* val);
		static void add_script_color(const std::string& id, const color& val);
	};

	extern std::unordered_map< std::string, option > vars;
	extern std::unordered_map< std::string, option > script_vars;

	void save(const std::unordered_map< std::string, option >& options, const std::string& path);
	void load(std::unordered_map< std::string, option >& options, const std::string& path);
	void init();
}