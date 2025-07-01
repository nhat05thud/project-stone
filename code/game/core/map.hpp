//
// Created by nguye on 6/30/2025.
//

#ifndef MAP_HPP
#define MAP_HPP

#include "map_tile.hpp"

class Map {
    public:
        auto generate_empty(float tile_half_width, float tile_half_height) -> void;
        auto generate(float tile_half_width, float tile_half_height) -> void;
        auto generate_corners() -> void;

        auto collapse() -> void;

        auto is_valid(const int nx, const int ny) -> bool;

        auto propagate(const int x, const int y) -> void;

        static constexpr auto tile_rows = 8;
        static constexpr auto tile_cols = 8;

        map_tile tiles[tile_rows][tile_cols];

    private:
};



#endif //MAP_HPP
