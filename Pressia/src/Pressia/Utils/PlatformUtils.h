#pragma once
#include <string>

namespace Pressia {

	class FileDialogs {
	public:
		static std::string OpenFile(const char* filter);	// Returns empty string if failed
		static std::string SaveFile(const char* filter);	// Returns empty string if failed
	};
}