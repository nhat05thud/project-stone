//
// Created by nguye on 7/3/2025.
//

#ifndef MAP_BUILDER_HPP
#define MAP_BUILDER_HPP

#include "map.hpp"

namespace core
{
    class MapBuilder
    {
    public:
        auto generate       (uint32_t rows, uint32_t columns, float tile_half_width, float tile_half_height) -> MapBuilder&;
        auto generate_empty (uint32_t rows, uint32_t columns, float tile_half_width, float tile_half_height) -> MapBuilder&;

        auto build() -> std::unique_ptr<Map>;

    private:
        std::unique_ptr<Map> _map;
    };
}



#endif //MAP_BUILDER_HPP
