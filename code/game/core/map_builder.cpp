//
// Created by nguye on 7/3/2025.
//

#include "map_builder.hpp"

namespace core
{
    auto MapBuilder::generate(const uint32_t rows, const uint32_t columns, const float tile_half_width, const float tile_half_height) -> MapBuilder&
    {
        assert(_map == nullptr);

		_map = std::make_unique<Map>(); // TODO move this into a create function?

        const auto noise_seed_factor = glm::linearRand(1.0f, 10.0f);

        for (auto row = 0; row < rows; row++) {
            const auto offset_x = row * tile_half_width;
            const auto offset_y = row * tile_half_height;

            for (auto column = 0; column < columns; column++) {
                auto& tile = _map->tiles[row][column];

                constexpr auto noise_x_factor = 0.2f;
                constexpr auto noise_y_factor = 0.2f;

                const auto noise_value = glm::simplex(glm::vec3(row * noise_x_factor, column * noise_y_factor, noise_seed_factor));

                if (noise_value <= -0.5f) {
                    tile.variation = glm::linearRand(0, 1);
                    tile.type = map::tile_type::water;
                }
                else {
                    tile.variation = glm::linearRand(0, 2);
                    tile.type = map::tile_type::ground;
                }

                auto x = offset_x + column * tile_half_width;
                auto y = offset_y - column * tile_half_height;

                tile.posistion = {x, y};
            }
        }

        return *this;
    }

    auto MapBuilder::generate_empty(const uint32_t rows, const uint32_t columns, const float tile_half_width, const float tile_half_height) -> MapBuilder&
    {
        assert(_map == nullptr);

        _map = std::make_unique<Map>(); // TODO move this into a create function?

        for (auto row = 0; row < rows; row++) {
            const auto offset_x = row * tile_half_width;
            const auto offset_y = row * tile_half_height;

            for (auto column = 0; column < columns; column++) {
                auto& tile = _map->tiles[row][column];

                auto x = offset_x + column * tile_half_width;
                auto y = offset_y - column * tile_half_height;

                tile.variation = glm::linearRand(0, 1);
                tile.posistion = {x, y};
            }
        }

        return *this;
    }

    auto MapBuilder::build() -> std::unique_ptr<Map>
    {
        return std::move(_map);
    }
}
