//
// Created by nguye on 7/1/2025.
//

#ifndef MAP_TILE_POSITION_HPP
#define MAP_TILE_POSITION_HPP

namespace core::map {
    enum class tile_orientation {
        left_top        = 1,
        left_bottom     = 1 << 1,
        right_top       = 1 << 2,
        right_bottom    = 1 << 3,

        /* double combinations */

        left_right = left_bottom | right_top,
        top_bottom = left_top    | right_bottom,

        bottom = left_bottom | right_bottom,
         right = right_top   | right_bottom,
          left = left_top    | left_bottom,
           top = left_top    | right_top,
    };
}
#endif //MAP_TILE_POSITION_HPP
