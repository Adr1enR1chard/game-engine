#pragma once

#include <engine/utils/types.hpp>

namespace engine
{

    Key GLFWKeyToEngineKey(int glfwKey);

    KeyAction GLFWActionToEngineAction(int glfwAction);

} // namespace engine