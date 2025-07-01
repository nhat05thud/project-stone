//
// Created by nguye on 6/30/2025.
//

#include "map.hpp"
#include "map_tile_positions.hpp"

std::set<map_tile_type> all_tile_types = { ground, water };

std::map<map_tile_type, std::map<map_tile_positions, std::set<map_tile_type>>> tile_rules
{
    {
        ground, {
                { left_top,     { ground, ground_water } },
                { left_bottom,  { ground, ground_water } },
                { right_top,    { ground, ground_water } },
                { right_bottom, { ground, ground_water } }
        }
    },
    {
        water, {
                { left_top,     { water, ground_water } },
                { left_bottom,  { water, ground_water } },
                { right_top,    { water, ground_water } },
                { right_bottom, { water, ground_water } }
        }
    }
};

struct cell {
    std::set<map_tile_type> options;
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
                map_tile.type = water;
                map_tile.variation = glm::linearRand(0, 1);
            }
            else {
                map_tile.type = ground;
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

    std::vector<std::pair<int, int>> directions = {{0,-1}, {1,0}, {0,1}, {-1,0}};
    std::vector<map_tile_positions> positions = {left_top, right_top, right_bottom, left_bottom};

    for (auto i = 0; i < 4; i++) {
        const auto nx = x + directions[i].first;
        const auto ny = y + directions[i].second;

        auto position = positions[i];
        if (is_valid(nx, ny) && !cell_tiles[nx][ny].collapsed) {
            tiles[nx][ny].marked = true;

            auto allowed = tile_rules[tile_type][position];

            if (!allowed.contains(tiles[nx][ny].type)) {
                tiles[nx][ny].type = ground_water;
                tiles[nx][ny].variation = 0;
                tiles[nx][ny].position_type = position;
            }
        }
    }
}
