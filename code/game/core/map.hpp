//
// Created by nguye on 6/30/2025.
//

#ifndef MAP_HPP
#define MAP_HPP

#include "map_tile.hpp"

class Map {
    public:
        auto generate(float tile_half_width, float tile_half_height) -> void;
        auto generate_corners() -> void;

        map_tile tiles[8][8];

    private:
};



#endif //MAP_HPP
