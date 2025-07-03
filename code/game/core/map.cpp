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

    cell cell_tiles[Map::tile_rows][Map::tile_cols] = {};

    auto Map::generate_corners() -> void {

        std::println("Generating corners - begin");

        for (auto i = 0; i < tile_rows * tile_cols; i++) {
            collapse();
        }

        std::println("Generating corners - end");
    }

    auto Map::collapse() -> void {
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
    }

    auto Map::is_valid(const int nx, const int ny) -> bool {
        return nx >= 0 && nx < tile_rows && ny >= 0 && ny < tile_cols;
    }

    auto Map::propagate(const int x, const int y) -> void {
        auto tile_type = tiles[x][y].type;

        std::vector<std::pair<int, int>> directions = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
        std::vector<map::tile_orientation> positions = { map::tile_orientation::left_top, map::tile_orientation::right_top, map::tile_orientation::right_bottom, map::tile_orientation::left_bottom};

        for (auto i = 0; i < 4; i++) {
            const auto nx = x + directions[i].first;
            const auto ny = y + directions[i].second;

            auto position = positions[i];
            if (is_valid(nx, ny) && !cell_tiles[nx][ny].collapsed) {
                tiles[nx][ny].marked = true;

                auto allowed = tile_rules[tile_type][position];
                // std::set<tile_type> allowed;
                // std::ranges::copy_if(tile_rules[tile_type][position], std::inserter(allowed, allowed.begin()), [&](auto type) {
                //     return type != tile_type;
                // });

                if (!allowed.contains(tiles[nx][ny].type)) {

                    std::set<map::tile_type> new_allowed;
                    std::ranges::copy_if(allowed, std::inserter(new_allowed, new_allowed.begin()), [&](auto type) {
                       return type != tile_type;
                    });

                    assert(new_allowed.size() == 1); // TODO check this in the feature to fix it if more than one is allowed based on some weight

                    // TODO don't let this with the *iterator
                    tiles[nx][ny].type = *new_allowed.begin(); // TODO this should be selected from the allowed set
                    tiles[nx][ny].orientation = position;
                    tiles[nx][ny].variation = 0;
                }
                else {
                    // TODO go over the neighbor and see how many of them you have, based on that choose the appropriate orientation

                }
            }
        }
    }
}
