#include "config.hpp"

void config::init() {
	char my_documents[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents))) {
		CreateDirectory((std::string(my_documents).append("\\eclipse")).c_str(), NULL);
		CreateDirectory((std::string(my_documents).append("\\eclipse\\cfg")).c_str(), NULL);
	}

	auto normalize = [&](const std::string& name) -> std::string {
		size_t idx = name.find_last_of(".");
		std::string raw = name.substr(0, idx);
		return raw;
	};

	if (!config_list.empty())
		config_list.clear();

	auto path = (std::string(my_documents).append("\\eclipse\\cfg\\"));
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
		auto file = normalize(entry.path().filename().string());
		config_list.push_back(file);
	}
}