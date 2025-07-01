//
// Created by nguye on 6/29/2025.
//

#ifndef MAP_TILE_HPP
#define MAP_TILE_HPP

#include "map_tile_type.hpp"
#include "map_tile_positions.hpp"

struct map_tile {
    glm::vec2 posistion;
    int32_t variation {};

    bool marked { false };
    map_tile_type type {};
    map_tile_positions position_type;
};

#endif //MAP_TILE_HPP
