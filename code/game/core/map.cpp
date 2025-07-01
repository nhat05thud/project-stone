//
// Created by nguye on 6/30/2025.
//

#include "map.hpp"

auto Map::generate(float tile_half_width, float tile_half_height) -> void {
    srand(time(0)); // EP5 11:00

    const auto noise_seed_factor = glm::linearRand(1.0f, 10.0f); // TODO use the last param as a seed from random float

    constexpr auto tile_rows = 8;
    constexpr auto tile_cols = 8;

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
                // increment++;
            }
            else {
                map_tile.type = ground;
            }

            // std::cout << std::format("{}[{}]", noise, static_cast<int>(map_tile.type));

            auto x = offset_x + column * tile_half_width;
            auto y = offset_y - column * tile_half_height;

            map_tile.posistion = {x, y};
        }
    }
}

auto Map::generate_corners() -> void {

}