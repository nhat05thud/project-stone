//
// Created by nguye on 6/30/2025.
//

#ifndef MAP_TILE_TYPE_HPP
#define MAP_TILE_TYPE_HPP

namespace core::map {
    // TODO handle forest different, ex: ground, hill and mountain both can have forest
    enum class tile_type {
        water,

        ground,
        ground_water,

        hill,
        mountain
    };
}

#endif //MAP_TILE_TYPE_HPP
