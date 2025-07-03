//
// Created by nguye on 6/30/2025.
//

#ifndef MAP_HPP
#define MAP_HPP

#include "map/tile.hpp"

namespace core {
    class Map {
        public:
            auto generate_corners() -> void;

            auto collapse() -> void;

            auto is_valid(const int nx, const int ny) -> bool;

            auto propagate(const int x, const int y) -> void;

            static constexpr auto tile_rows = 8;
            static constexpr auto tile_cols = 8;

            map::tile tiles[tile_rows][tile_cols]; // TODO use just an std::vector here?

        private:
            std::vector<map::tile> _tiles;
    };
}

#endif //MAP_HPP
