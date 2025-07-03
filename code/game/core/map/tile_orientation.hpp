//
// Created by nguye on 7/1/2025.
//

#ifndef MAP_TILE_POSITION_HPP
#define MAP_TILE_POSITION_HPP

namespace core::map {
    enum class tile_orientation {

        /* one direction */
        left_top        = 1,
        left_bottom     = 1 << 1,
        right_top       = 1 << 2,
        right_bottom    = 1 << 3,

        /* two directions */

               top = left_top     | right_top,
              left = left_top     | left_bottom,
             right = right_top    | right_bottom,
            bottom = right_bottom | left_bottom,

        left_right = left_bottom  | right_top,
        top_bottom = right_bottom | left_top,

        /* directions with sides */
        
        left_top_with_sides     = right_top | right_bottom | left_bottom,
        left_bottom_with_sides  = left_top  | left_bottom  | right_bottom,
        right_bottom_with_sides = left_top  | left_bottom  | right_top,
        right_top_with_sides    = right_top | right_bottom | left_top
    };
}
#endif //MAP_TILE_POSITION_HPP
