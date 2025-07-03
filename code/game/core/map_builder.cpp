//
// Created by nguye on 7/3/2025.
//

#include "map_builder.hpp"

namespace core
{
    auto MapBuilder::create(uint32_t rows, uint32_t cols) -> MapBuilder&
    {
        assert(_map == nullptr);
        _map = std::make_unique<Map>(rows, cols); 

		return *this;
    }

    auto MapBuilder::generate(const float tile_half_width, const float tile_half_height) -> MapBuilder&
    {
        assert(_map);

        const auto noise_seed_factor = glm::linearRand(1.0f, 10.0f);

        for (auto row = 0; row < _map->rows(); row++) {
            const auto offset_x = row * tile_half_width;
            const auto offset_y = row * tile_half_height;

            for (auto col = 0; col < _map->cols(); col++) {
                auto& tile = _map->tile(row, col);

                constexpr auto noise_x_factor = 0.2f;
                constexpr auto noise_y_factor = 0.2f;

                const auto noise_value = glm::simplex(glm::vec3(row * noise_x_factor, col * noise_y_factor, noise_seed_factor));

                if (noise_value <= -0.5f) {
                    tile.variation = glm::linearRand(0, 1);
                    tile.type = map::tile_type::water;
                }
                else {
                    tile.variation = glm::linearRand(0, 2);
                    tile.type = map::tile_type::ground;
                }

                auto x = offset_x + col * tile_half_width;
                auto y = offset_y - col * tile_half_height;

                tile.posistion = {x, y};
            }
        }

        return *this;
    }

    auto MapBuilder::generate_empty(const float tile_half_width, const float tile_half_height) -> MapBuilder&
    {
        assert(_map);

        for (auto row = 0; row < _map->rows(); row++) {
            const auto offset_x = row * tile_half_width;
            const auto offset_y = row * tile_half_height;

            for (auto col = 0; col < _map->cols(); col++) {
                auto& tile = _map->tile(row, col);

                auto x = offset_x + col * tile_half_width;
                auto y = offset_y - col * tile_half_height;

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
