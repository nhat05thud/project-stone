//
// Created by nguye on 6/29/2025.
//

#ifndef MAP_TILE_HPP
#define MAP_TILE_HPP

#include "map_tile_type.hpp"

struct map_tile {
    glm::vec2 posistion;
    map_tile_type type {};
};

#endif //MAP_TILE_HPP
