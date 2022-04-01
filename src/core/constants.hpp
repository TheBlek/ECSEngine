#pragma once
#include <cstdint>

using Component = std::uint16_t;
constexpr Component MAX_COMPONENTS = 30;
constexpr Component MAX_SYSTEMS = 30;

using Entity = std::uint16_t;
constexpr Entity MAX_ENTITIES = 5000;
