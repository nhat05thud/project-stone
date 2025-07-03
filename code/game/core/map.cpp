//
// Created by nguye on 6/30/2025.
//

#include "map.hpp"

namespace core {
    std::vector all_tile_types = { map::tile_type::ground, map::tile_type::water };

    std::map<map::tile_type, std::map<map::tile_orientation, std::set<map::tile_type>>> tile_rules
    {
        {
            map::tile_type::ground, {
                // TODO add weight for each possible ground to retrieve the first one based on the bigger weight
                { map::tile_orientation::left_top,     { map::tile_type::ground, map::tile_type::ground_water } },
                { map::tile_orientation::left_bottom,  { map::tile_type::ground, map::tile_type::ground_water } },
                { map::tile_orientation::right_top,    { map::tile_type::ground, map::tile_type::ground_water } },
                { map::tile_orientation::right_bottom, { map::tile_type::ground, map::tile_type::ground_water } }
            }
        },
        {
            map::tile_type::water, {
                { map::tile_orientation::left_top,     { map::tile_type::water, map::tile_type::ground_water } },
                { map::tile_orientation::left_bottom,  { map::tile_type::water, map::tile_type::ground_water } },
                { map::tile_orientation::right_top,    { map::tile_type::water, map::tile_type::ground_water } },
                { map::tile_orientation::right_bottom, { map::tile_type::water, map::tile_type::ground_water } }
            }
        }
    };

    struct cell {
        std::set<map::tile_type> options;
        bool collapsed {};
    };

    //cell cell_tiles[Map::tile_rows][Map::tile_cols];

    Map::Map(const uint32_t rows, const uint32_t cols) 
		: _rows{ rows }, _cols{ cols }, _tiles{ rows * cols }
    {

    }

    /*auto Map::generate_corners() -> void 
    {
        std::println("Generating corners - begin");
        for (auto i = 0; i < tile_rows * tile_cols; i++) {
            collapse();
        }
        std::println("Generating corners - end");
    }*/

    /*auto Map::collapse() -> void 
    {
        for (auto row = 0; row < tile_rows; row++) {
            for (auto column = 0; column < tile_cols; column++) {
                auto& cell = cell_tiles[row][column];
    
                if (!cell.collapsed) {
                    cell.collapsed = true;
                    propagate(row, column);
                    return;
                }
            }
        }
    }*/

    auto Map::propagate(const int x, const int y) -> void 
    {
        // TODO make directions an positions into a single structure
		// TODO add this dynamically in the map generator

        std::vector<std::pair<int, int>> directions = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
        std::vector<map::tile_orientation> positions = { map::tile_orientation::left_top, map::tile_orientation::right_top, map::tile_orientation::right_bottom, map::tile_orientation::left_bottom};

        auto tile_type = tile(x, y).type;

        for (auto i = 0; i < 4; i++) 
        {
            auto position = positions[i];

            const auto nx = x + directions[i].first;
            const auto ny = y + directions[i].second;

            if (is_within_bounds(nx, ny) /*&& !cell_tiles[nx][ny].collapsed*/) 
            { 
                auto& next_tile = tile(nx, ny); // EP: Generate Neighbors Tiles - First Part 52:00

                next_tile.marked = true;

                auto allowed = tile_rules[tile_type][position];

                if (allowed.contains(next_tile.type) == false) 
                {
                    std::set<map::tile_type> new_allowed;
                    std::ranges::copy_if(allowed, std::inserter(new_allowed, new_allowed.begin()), [&](auto type) {
                       return type != tile_type;
                    });

                    assert(new_allowed.size() == 1); // TODO check this in the feature to fix it if more than one is allowed based on some weight

                    // TODO don't let this with the *iterator
                    next_tile.type          = *new_allowed.begin(); // TODO this should be selected from the allowed set
                    next_tile.orientation   = position;
                    next_tile.variation     = 0;
                }
                else {
                    // TODO go over the neighbor and see how many of them you have, based on that choose the appropriate orientation

                }
            }
        }
    }

    auto Map::tile(const uint32_t row, const uint32_t col) -> map::tile&
    {
		return _tiles[_cols * row + col]; // TODO change/improve this if it is a performance issue
    }

    auto Map::is_within_bounds(const int32_t row, const int32_t col) const -> bool 
    {
        return row >= 0 && row < _rows && col >= 0 && col < _cols;
    }

    auto Map::rows() const -> uint32_t
    {
        return _rows;
    }

    auto Map::cols() const -> uint32_t
    {
        return _cols;
    }
}
