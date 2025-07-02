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

    auto Map::generate_empty(const float tile_half_width, const float tile_half_height) -> void {
        for (auto row = 0; row < tile_rows; row++) {
            const auto offset_x = row * tile_half_width;
            const auto offset_y = row * tile_half_height;

            for (auto column = 0; column < tile_cols; column++) {
                auto& map_tile = tiles[row][column];

                auto x = offset_x + column * tile_half_width;
                auto y = offset_y - column * tile_half_height;

                map_tile.posistion = {x, y};

                map_tile.variation = glm::linearRand(0, 1);
            }
        }
    }

    auto Map::generate(float tile_half_width, float tile_half_height) -> void {
        srand(time(0)); // EP5 11:00

        const auto noise_seed_factor = glm::linearRand(1.0f, 10.0f); // TODO use the last param as a seed from random float

        for (auto row = 0; row < tile_rows; row++) {
            const auto offset_x = row * tile_half_width;
            const auto offset_y = row * tile_half_height;

            for (auto column = 0; column < tile_cols; column++) {
                auto& map_tile = tiles[row][column];

                constexpr auto noise_x_factor = 0.2f;
                constexpr auto noise_y_factor = 0.2f;

                const auto noise = glm::simplex(glm::vec3(row * noise_x_factor, column * noise_y_factor, noise_seed_factor));

                if (noise <= -0.5f) {
                    map_tile.type = map::tile_type::water;
                    map_tile.variation = glm::linearRand(0, 1);
                }
                else {
                    map_tile.type = map::tile_type::ground;
                    map_tile.variation = glm::linearRand(0, 2);
                }

                auto x = offset_x + column * tile_half_width;
                auto y = offset_y - column * tile_half_height;

                map_tile.posistion = {x, y};
            }
        }
    }

    auto Map::generate_corners() -> void {

        std::cout << "Generating corners - begin" << std::endl;

        for (auto i = 0; i < tile_rows * tile_cols; i++) {
            collapse();
        }

        std::cout << "Generating corners - end" << std::endl;
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
                    tiles[nx][ny].variation = 0;
                    tiles[nx][ny].orientation = position;
                }
                else {
                    // TODO go over the neighbor and see how many of them you have, based on that choose the appropriate orientation

                }
            }
        }
    }
}
