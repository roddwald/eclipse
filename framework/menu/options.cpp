#include "options.hpp"
#include "../utils/utils.hpp"
#include "../utils/xor.hpp"
#include "../tinyxml2/tinyxml2.h"

std::unordered_map< std::string, options::option > options::vars{ };
std::unordered_map< std::string, options::option > options::script_vars{ };

void options::option::add_bool(const std::string& id, bool val) {
	vars[id].type = option_type_t::boolean;
	vars[id].val.b = val;
}

void options::option::add_list(const std::string& id, int count) {
	vars[id].type = option_type_t::list;
	vars[id].list_size = count;
	memset(vars[id].val.l, 0, sizeof(vars[id].val.l));
}

void options::option::add_int(const std::string& id, int val) {
	vars[id].type = option_type_t::integer;
	vars[id].val.i = val;
}

void options::option::add_list(const std::string& id, std::deque<bool> val) {
	vars[id].type = option_type_t::deque;
	vars[id].list_size = val.size();
	vars[id].val.d = val;
}

void options::option::add_float(const std::string& id, float val) {
	vars[id].type = option_type_t::floating_point;
	vars[id].val.f = val;
}

void options::option::add_str(const std::string& id, const char* val) {
	vars[id].type = option_type_t::string;
	strcpy_s(vars[id].val.s, val);
}

void options::option::add_color(const std::string& id, const color& val) {
	vars[id].type = option_type_t::color;
	vars[id].val.c = val;
}

void options::option::add_script_bool(const std::string& id, bool val) {
	script_vars[id].type = option_type_t::boolean;
	script_vars[id].val.b = val;
}

void options::option::add_script_list(const std::string& id, int count) {
	script_vars[id].type = option_type_t::list;
	script_vars[id].list_size = count;
	memset(script_vars[id].val.l, 0, sizeof(script_vars[id].val.l));
}

void options::option::add_script_int(const std::string& id, int val) {
	script_vars[id].type = option_type_t::integer;
	script_vars[id].val.i = val;
}

void options::option::add_script_float(const std::string& id, float val) {
	script_vars[id].type = option_type_t::floating_point;
	script_vars[id].val.f = val;
}

void options::option::add_script_str(const std::string& id, const char* val) {
	script_vars[id].type = option_type_t::string;
	strcpy_s(script_vars[id].val.s, val);
}

void options::option::add_script_color(const std::string& id, const color& val) {
	script_vars[id].type = option_type_t::color;
	vars[id].val.c = val;
}

std::vector< std::string > split(const std::string& str, const std::string& delim) {
	std::vector< std::string > tokens;
	size_t prev = 0, pos = 0;

	do {
		pos = str.find(delim, prev);

		if (pos == std::string::npos)
			pos = str.length();

		std::string token = str.substr(prev, pos - prev);

		if (!token.empty())
			tokens.push_back(token);

		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());

	return tokens;
}

void options::save(const std::unordered_map< std::string, option >& options, const std::string& path) {
	tinyxml2::XMLDocument doc;

	const auto root = doc.NewElement(_("drown"));

	doc.InsertFirstChild(root);

	for (auto& option : options) {
		const auto element = doc.NewElement(option.first.data());

		switch (option.second.type) {
		case option_type_t::boolean: {
			element->SetText(option.second.val.b);
		} break;
		case option_type_t::deque: {
			for (auto i = 0; i < option.second.val.d.size(); i++) {
				const auto list_element = doc.NewElement(_("deque"));
				list_element->SetText(option.second.val.d[i]);
				element->InsertEndChild(list_element);
			}
		} break;
		case option_type_t::list: {
			for (auto i = 0; i < option.second.list_size; i++) {
				const auto list_element = doc.NewElement(_("item"));
				list_element->SetText(option.second.val.l[i]);
				element->InsertEndChild(list_element);
			}
		} break;
		case option_type_t::integer: {
			element->SetText(option.second.val.i);
		} break;
		case option_type_t::floating_point: {
			element->SetText(option.second.val.f);
		} break;
		case option_type_t::string: {
			element->SetText((char*)option.second.val.s);
		} break;
		case option_type_t::color: {
			const auto r_element = doc.NewElement(_("r"));
			const auto g_element = doc.NewElement(_("g"));
			const auto b_element = doc.NewElement(_("b"));
			const auto a_element = doc.NewElement(_("a"));

			r_element->SetText(option.second.val.c.r);
			g_element->SetText(option.second.val.c.g);
			b_element->SetText(option.second.val.c.b);
			a_element->SetText(option.second.val.c.a);

			element->InsertEndChild(r_element);
			element->InsertEndChild(g_element);
			element->InsertEndChild(b_element);
			element->InsertEndChild(a_element);
		} break;
		}

		root->InsertEndChild(element);
	}

	doc.SaveFile(path.data());
}

void options::load(std::unordered_map< std::string, option >& options, const std::string& path) {
	tinyxml2::XMLDocument doc;

	const auto err = doc.LoadFile(path.data());

	if (err != tinyxml2::XML_SUCCESS) {
		//dbg_print( _( "Failed to open configuration file.\n" ) );
		return;
	}

	const auto root = doc.FirstChild();

	if (!root) {
		//dbg_print( _( "Failed to open configuration file.\n" ) );
		return;
	}

	for (auto& option : options) {
		const auto element = root->FirstChildElement(option.first.data());

		if (!element) {
			//dbg_print( _( "Failed to find element.\n" ) );
			continue;
		}

		auto err = tinyxml2::XML_SUCCESS;

		switch (option.second.type) {
		case option_type_t::boolean: {
			err = element->QueryBoolText(&option.second.val.b);
		} break;
		case option_type_t::list: {
			auto list_element = element->FirstChildElement(_("item"));
			auto i = 0;

			if (!list_element) {
				//dbg_print( _( "Element found had invalid value.\n" ) );
				continue;
			}

			while (list_element && i < option.second.list_size) {
				err = list_element->QueryBoolText(&option.second.val.l[i]);

				if (err != tinyxml2::XML_SUCCESS) {
					//dbg_print( _( "Element found had invalid value.\n" ) );
					break;
				}

				list_element = list_element->NextSiblingElement(_("item"));
				i++;
			}
		} break;
		case option_type_t::deque: {
			auto list_element = element->FirstChildElement(_("deque"));
			auto i = 0;

			if (!list_element) {
				//dbg_print( _( "Element found had invalid value.\n" ) );
				continue;
			}

			while (list_element && i < option.second.list_size) {
				err = list_element->QueryBoolText(&option.second.val.d[i]);

				if (err != tinyxml2::XML_SUCCESS) {
					//dbg_print( _( "Element found had invalid value.\n" ) );
					break;
				}

				list_element = list_element->NextSiblingElement(_("deque"));
				i++;
			}
		} break;
		case option_type_t::integer: {
			err = element->QueryIntText(&option.second.val.i);
		} break;
		case option_type_t::floating_point: {
			err = element->QueryFloatText(&option.second.val.f);
		} break;
		case option_type_t::string: {
			const auto str = element->GetText();

			if (!str) {
				//dbg_print( _( "Element found had invalid value.\n" ) );
				continue;
			}

			strcpy_s(option.second.val.s, str);
		} break;
		case option_type_t::color: {
			const auto r_element = element->FirstChildElement(_("r"));
			const auto g_element = element->FirstChildElement(_("g"));
			const auto b_element = element->FirstChildElement(_("b"));
			const auto a_element = element->FirstChildElement(_("a"));

			if (!r_element || !g_element || !b_element || !a_element) {
				//dbg_print( _( "Element found had invalid value.\n" ) );
				continue;
			}

			err = r_element->QueryIntText(&option.second.val.c.r);

			if (err != tinyxml2::XML_SUCCESS) {
				//dbg_print( _( "Element found had invalid value.\n" ) );
				continue;
			}

			err = g_element->QueryIntText(&option.second.val.c.g);

			if (err != tinyxml2::XML_SUCCESS) {
				//dbg_print( _( "Element found had invalid value.\n" ) );
				continue;
			}

			err = b_element->QueryIntText(&option.second.val.c.b);

			if (err != tinyxml2::XML_SUCCESS) {
				//dbg_print( _( "Element found had invalid value.\n" ) );
				continue;
			}

			err = a_element->QueryIntText(&option.second.val.c.a);
		} break;
		}

		if (err != tinyxml2::XML_SUCCESS) {
			//dbg_print( _( "Element found had invalid value.\n" ) );
			continue;
		}
	}
}

void options::init() {
	// legitbot
}