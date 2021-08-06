#pragma once
#include "options.hpp"
#include <shlobj.h>
#include <filesystem>
#include <mutex>

namespace config {
	inline std::vector<std::string> config_list = { };
	inline std::mutex mutex;
	void init();
}