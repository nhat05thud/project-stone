//
// Created by nguye on 6/29/2025.
//

#ifndef MAP_TILE_HPP
#define MAP_TILE_HPP

#include "map/tile_type.hpp"
#include "tile_orientation.hpp"

namespace core::map {
    struct tile {
        glm::vec2 posistion;
        int32_t variation {};

        tile_type type {};
        tile_orientation orientation;

        bool marked { false };
    };
}

#endif //MAP_TILE_HPP
