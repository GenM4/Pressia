#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <filesystem>
#include <fstream>

#include <glm/glm.hpp>

#include "Pressia/Core/CoreConfig.h"
#include "Pressia/Core/Log.h"
#include "Pressia/Core/KeyCodes.h"
#include "Pressia/Core/MouseButtonCodes.h"
#include "Pressia/Debug/Instrumentor.h"

#ifdef PS_PLATFORM_WINDOWS
#include <Windows.h>
#endif