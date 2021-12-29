#pragma once

#include "Rengine/Core/core.hpp"

#ifdef RE_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <array>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Rengine/Core/log.hpp"
#include "Rengine/Debug/Instrumentor.hpp"

#ifdef RE_WINDOWS
    #include <Windows.h>
#endif