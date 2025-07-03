//
// Created by nguye on 6/30/2025.
//

#ifndef MAP_HPP
#define MAP_HPP

#include "map/tile.hpp"

namespace core {
    class Map {
        public:
            Map(uint32_t rows, uint32_t cols);

            auto is_within_bounds(int32_t row, int32_t col) const -> bool;

            auto tile(uint32_t row, uint32_t col) -> map::tile&;

            auto rows() const->uint32_t;
            auto cols() const->uint32_t;

            // TODO move this three functions inside map builder class
            auto collapse()                 -> void;
            auto propagate(int x, int y)    -> void;
            auto generate_corners()         -> void;

        private:
            std::vector<map::tile> _tiles;

			uint32_t _rows { };
            uint32_t _cols { };
    };
}

#endif //MAP_HPP
