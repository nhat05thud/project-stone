//
// Created by nguye on 6/29/2025.
//
#include <shaders/converter.hpp>

#include <opengl/commands.hpp>
#include <opengl/functions.hpp>
#include <opengl/pipeline.hpp>
#include <opengl/shader.hpp>
#include <opengl/texture.hpp>
#include <opengl/texture_sampler.hpp>
#include <opengl/vertex_array.hpp>

#include <opengl/constants/buffer.hpp>
#include <opengl/constants/commands.hpp>
#include <opengl/constants/sampler.hpp>
#include <opengl/constants/shader.hpp>
#include <opengl/constants/texture.hpp>

#include <core/file.hpp>

#include "map_builder.hpp"

auto main() -> int32_t
{
    srand(time(0)); // EP5 11:00 - change: EP10 1:11:30

    if (glfwInit() != GLFW_TRUE) {
        return -1;
    }

    static auto window_width { 1366 };
    static auto window_height { 768 };
    static bool window_closed { };

    static auto selected_tile_row { 0 };
    static auto selected_tile_col { 0 };

    static bool editor_is_active { false };

    static std::unique_ptr<core::Map> map;

    const auto window = glfwCreateWindow(window_width, window_height, "Project Stone", nullptr);

    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, [](const int width, const int height) {
        window_width = width;
        window_height = height;
    });

    glfwSetKeyCallback(window, [](int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            window_closed = true;
        }
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
            editor_is_active = !editor_is_active;
        }
        if (editor_is_active) {
            if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
                selected_tile_col++;
                if (selected_tile_col > map->cols() - 1) {
                    selected_tile_col = map->cols() - 1;
                }
            }
            else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
                selected_tile_col--;
                if (selected_tile_col < 0) {
                    selected_tile_col = 0;
                }
            }
            else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
                selected_tile_row++;
                if (selected_tile_row > map->rows() - 1) {
                    selected_tile_row = map->rows() - 1;
                }
            }
            else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
                selected_tile_row--;
                if (selected_tile_row < 0) {
                    selected_tile_row = 0;
                }
            }
            if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
                auto& tile = map->tile(selected_tile_row, selected_tile_col);

                tile.variation  = glm::linearRand(0, 1);
                tile.type       = core::map::tile_type::water;

                map->propagate(selected_tile_row, selected_tile_col);
            }
            else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
                auto& tile = map->tile(selected_tile_row, selected_tile_col);

                tile.variation  = glm::linearRand(0, 2);
                tile.type       = core::map::tile_type::ground;

                map->propagate(selected_tile_row, selected_tile_col);
            }
        }
    });

    glfwSetWindowCloseCallback(window, [] {
        window_closed = true;
    });

    glfwMakeContextCurrent(window);

    gladLoadGL();

    opengl::Functions::init();

    shaders::Converter::convert("../../resources/shaders", ".");

    opengl::ShaderStage base_texture_shader_vert;
    base_texture_shader_vert.type(opengl::constants::vertex_shader);
    base_texture_shader_vert.create();
    base_texture_shader_vert.source(core::File::read("default_base_texture_shader.vert", std::ios::binary));

    opengl::ShaderStage base_texture_shader_frag;
    base_texture_shader_frag.type(opengl::constants::fragment_shader);
    base_texture_shader_frag.create();
    base_texture_shader_frag.source(core::File::read("default_base_texture_shader.frag", std::ios::binary));

    opengl::Shader base_texture_shader;
    base_texture_shader.create();
    base_texture_shader.attach(base_texture_shader_vert);
    base_texture_shader.attach(base_texture_shader_frag);
    base_texture_shader.link();

    auto nr_channels = 0;

    core::texture::data ground_1_texture_data;
    core::texture::data ground_2_texture_data;
    core::texture::data ground_3_texture_data;

    core::texture::data water_1_texture_data;
    core::texture::data water_2_texture_data;

    core::texture::data ground_water_bottom_left_data;
    core::texture::data ground_water_bottom_right_data;

    core::texture::data ground_water_top_left_data;
    core::texture::data ground_water_top_right_data;

    ground_1_texture_data.ptr = stbi_load("ground_1.png", &ground_1_texture_data.width, &ground_1_texture_data.height, &nr_channels, 0);
    ground_2_texture_data.ptr = stbi_load("ground_2.png", &ground_2_texture_data.width, &ground_2_texture_data.height, &nr_channels, 0);
    ground_3_texture_data.ptr = stbi_load("ground_3.png", &ground_3_texture_data.width, &ground_3_texture_data.height, &nr_channels, 0);

    water_1_texture_data.ptr = stbi_load("snow.png", &water_1_texture_data.width, &water_1_texture_data.height, &nr_channels, 0);
    water_2_texture_data.ptr = stbi_load("snow-1.png", &water_2_texture_data.width, &water_2_texture_data.height, &nr_channels, 0);

    ground_water_bottom_left_data.ptr = stbi_load("left_bottom.png", &ground_water_bottom_left_data.width, &ground_water_bottom_left_data.height, &nr_channels, 0);
    ground_water_bottom_right_data.ptr = stbi_load("right_bottom.png", &ground_water_bottom_right_data.width, &ground_water_bottom_right_data.height, &nr_channels, 0);

    ground_water_top_left_data.ptr = stbi_load("left_top.png", &ground_water_top_left_data.width, &ground_water_top_left_data.height, &nr_channels, 0);
    ground_water_top_right_data.ptr = stbi_load("right_top.png", &ground_water_top_right_data.width, &ground_water_top_right_data.height, &nr_channels, 0);

    if (ground_1_texture_data.ptr == nullptr || ground_2_texture_data.ptr == nullptr || ground_3_texture_data.ptr == nullptr) {
        return -2;
    }
    if (water_1_texture_data.ptr == nullptr || water_2_texture_data.ptr == nullptr) {
        return -2;
    }

    if (ground_water_bottom_left_data.ptr == nullptr || ground_water_bottom_right_data.ptr == nullptr || ground_water_top_left_data.ptr == nullptr ||ground_water_top_right_data.ptr == nullptr) {
        return -2;
    }

    opengl::Texture ground_1_texture;
    ground_1_texture.type(opengl::constants::texture_2d);
    ground_1_texture.create();
    ground_1_texture.storage(ground_1_texture_data, GL_RGBA8);
    ground_1_texture.update(ground_1_texture_data, GL_RGBA);

    opengl::Texture ground_2_texture;
    ground_2_texture.type(opengl::constants::texture_2d);
    ground_2_texture.create();
    ground_2_texture.storage(ground_2_texture_data, GL_RGBA8);
    ground_2_texture.update(ground_2_texture_data, GL_RGBA);

    opengl::Texture ground_3_texture;
    ground_3_texture.type(opengl::constants::texture_2d);
    ground_3_texture.create();
    ground_3_texture.storage(ground_3_texture_data, GL_RGBA8);
    ground_3_texture.update(ground_3_texture_data, GL_RGBA);

    opengl::Texture water_1_texture;
    water_1_texture.type(opengl::constants::texture_2d);
    water_1_texture.create();
    water_1_texture.storage(water_1_texture_data, GL_RGBA8);
    water_1_texture.update(water_1_texture_data, GL_RGBA);

    opengl::Texture water_2_texture;
    water_2_texture.type(opengl::constants::texture_2d);
    water_2_texture.create();
    water_2_texture.storage(water_2_texture_data, GL_RGBA8);
    water_2_texture.update(water_2_texture_data, GL_RGBA);

    opengl::Texture ground_water_bottom_left_texture;
    ground_water_bottom_left_texture.type(opengl::constants::texture_2d);
    ground_water_bottom_left_texture.create();
    ground_water_bottom_left_texture.storage(ground_water_bottom_left_data, GL_RGBA8);
    ground_water_bottom_left_texture.update(ground_water_bottom_left_data, GL_RGBA);

    opengl::Texture ground_water_bottom_right_texture;
    ground_water_bottom_right_texture.type(opengl::constants::texture_2d);
    ground_water_bottom_right_texture.create();
    ground_water_bottom_right_texture.storage(ground_water_bottom_right_data, GL_RGBA8);
    ground_water_bottom_right_texture.update(ground_water_bottom_right_data, GL_RGBA);

    opengl::Texture ground_water_top_left_texture;
    ground_water_top_left_texture.type(opengl::constants::texture_2d);
    ground_water_top_left_texture.create();
    ground_water_top_left_texture.storage(ground_water_top_left_data, GL_RGBA8);
    ground_water_top_left_texture.update(ground_water_top_left_data, GL_RGBA);

    opengl::Texture ground_water_top_right_texture;
    ground_water_top_right_texture.type(opengl::constants::texture_2d);
    ground_water_top_right_texture.create();
    ground_water_top_right_texture.storage(ground_water_top_right_data, GL_RGBA8);
    ground_water_top_right_texture.update(ground_water_top_right_data, GL_RGBA);

    const std::array ground_textures { &ground_1_texture, &ground_2_texture, &ground_3_texture };
    const std::array ground_water_textures { &ground_water_top_left_texture, &ground_water_bottom_left_texture,  &ground_water_top_right_texture, &ground_water_bottom_right_texture }; // The position follows the enum map_tile_positions
    const std::array water_textures { &water_1_texture, &water_2_texture };

    // ========================================================================================
    opengl::TextureSampler map_tile_texture_sampler;
    map_tile_texture_sampler.create();
    map_tile_texture_sampler.parameter(opengl::constants::texture_min_filter, GL_NEAREST);
    map_tile_texture_sampler.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    const auto tile_width =static_cast<float>(ground_1_texture_data.width);
    const auto tile_height =static_cast<float>(ground_1_texture_data.height);

    const auto tile_half_width = tile_width / 2.0f;
    const auto tile_half_height = tile_height / 2.0f;

    const std::vector tile_vertices {
      -tile_half_width, -tile_half_height, 0.0f, 0.0f,
       tile_half_width, -tile_half_height, 1.0f, 0.0f,
      -tile_half_width,  tile_half_height, 0.0f, 1.0f,
       tile_half_width,  tile_half_height, 1.0f, 1.0f
    };

    const std::vector tile_elements {
        0, 1, 2,
        3, 2, 1
    };

    constexpr core::vertex_array::attribute position_attribute { 0, 2, opengl::constants::float_type, 0 };
    constexpr core::vertex_array::attribute texcoord_attribute { 1, 2, opengl::constants::float_type, sizeof(glm::vec2) };

    opengl::Buffer tile_vbo;
    tile_vbo.create();
    tile_vbo.storage(core::buffer::make_data(tile_vertices));

    opengl::Buffer tile_ebo;
    tile_ebo.create();
    tile_ebo.storage(core::buffer::make_data(tile_elements));

    opengl::VertexArray tile_vao;
    tile_vao.create();
    tile_vao.attach_vertices(tile_vbo, sizeof(glm::vec2) + sizeof(glm::vec2));
    tile_vao.attach_elements(tile_ebo);

    tile_vao.attribute(position_attribute);
    tile_vao.attribute(texcoord_attribute);

    glm::vec3 camera_position { static_cast<float>(window_width) / 2.0f, static_cast<float>(window_height) / 2.0f, 0.0f };

    auto albedo_color = glm::vec4(1.0f);

    auto model = glm::mat4(1.0f);
    auto view = glm::translate(glm::mat4(1.0f), camera_position);;
    auto proj = glm::ortho(
        0.0f,
        static_cast<float>(window_width),
        static_cast<float>(window_height),
        0.0f);

    std::vector camera_uniforms{
        view, proj
    };

    opengl::Buffer transformed_ubo;
    transformed_ubo.create();
    transformed_ubo.storage(core::buffer::make_data(&model), opengl::constants::dynamic_draw);
    transformed_ubo.bind_base(opengl::constants::uniform_buffer, core::buffer::transform);

    opengl::Buffer camera_ubo;
    camera_ubo.create();
    camera_ubo.storage(core::buffer::make_data(camera_uniforms), opengl::constants::dynamic_draw);
    camera_ubo.bind_base(opengl::constants::uniform_buffer, core::buffer::camera);

    opengl::Buffer material_ubo;
    material_ubo.create();
    material_ubo.storage(core::buffer::make_data(&albedo_color), opengl::constants::dynamic_draw);
    material_ubo.bind_base(opengl::constants::uniform_buffer, core::buffer::material);

    opengl::Commands::clear(0.5f, 0.5f, 0.5f, 1.0f);

    opengl::Pipeline::enable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    core::MapBuilder map_builder;

    map = map_builder.create(8, 8)
                     .generate_empty(tile_half_width, tile_half_height)
                     .build();

    constexpr auto camera_speed = 250.0f; // TODO this is not right - because of small delta time fix it

    auto current_time = glfwGetTime();

    while (!window_closed) {
        glfwPollEvents();

        const auto new_time = glfwGetTime();
        const auto delta_time = new_time - current_time;
        current_time = new_time;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_position.x += camera_speed * delta_time;
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_position.x -= camera_speed * delta_time;
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_position.y += camera_speed * delta_time;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_position.y -= camera_speed * delta_time;
        }

        camera_uniforms[0] = glm::translate(glm::mat4(1.0f), camera_position);
        camera_uniforms[1] = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f);

        camera_ubo.update(core::buffer::make_data(camera_uniforms));

        opengl::Commands::viewport(0, 0, window_width, window_height);
        opengl::Commands::clear(opengl::constants::color_buffer);

        base_texture_shader.bind();

        map_tile_texture_sampler.bind(core::texture::albedo);

        // ground_water_textures[left_top].bind_unit(core::texture::albedo); // test

        tile_vao.bind();

        // opengl::Commands::draw_elements(opengl::constants::triangles, tile_elements.size()); // test

        for (auto row = 0; row < map->rows(); row++) {
            for (auto column = 0; column < map->cols(); column++) {
                auto& map_tile = map->tile(row, column);

                if (map_tile.type == core::map::tile_type::ground) 
                {
                    ground_textures[map_tile.variation]->bind_unit(core::texture::albedo);
                }
                else if (map_tile.type == core::map::tile_type::ground_water) 
                {
                    ground_water_textures[static_cast<int>(map_tile.orientation)]->bind_unit(core::texture::albedo);
                }
                else if (map_tile.type == core::map::tile_type::water) 
                {
                    water_textures[map_tile.variation]->bind_unit(core::texture::albedo);
                }

                model = glm::translate(glm::mat4(1.0f), glm::vec3(map_tile.posistion, 0.0f));
                transformed_ubo.update(core::buffer::make_data(&model));

                if (/*tile.marked || */(editor_is_active && row == selected_tile_row && column == selected_tile_col)) 
                {
                    albedo_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);
                }
                else {
                    albedo_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                }
                material_ubo.update(core::buffer::make_data(&albedo_color));

                opengl::Commands::draw_elements(opengl::constants::triangles, tile_elements.size());
            }
        }

        glfwSwapBuffers(window);
    }

    return 0;
}
