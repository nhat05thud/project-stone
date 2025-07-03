//
// Created by nguye on 7/1/2025.
//

#ifndef MAP_TILE_POSITION_HPP
#define MAP_TILE_POSITION_HPP

namespace core::map {
    // TODO handle this as flags starting from 1, 2, 4, etc
	// TODO based on the flags we can construct the additional ones ex: if is left_top + right_top = top | think about it
    enum class tile_orientation {
        // top,

        // left,
        left_top        = 1,
        left_bottom     = 2,

        // right,
        right_top       = 4,
        right_bottom    = 8,

        // bottom
    };
}
#endif //MAP_TILE_POSITION_HPP
