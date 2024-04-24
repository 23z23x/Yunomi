#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <optional>

//Allows GLFW to include Vulkan for us
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//Local files that shouldn't change (often) and are widely used

#include "Yunomi/Log.h"
#include "Yunomi/Core.h"