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

#include "map_tile.hpp"

auto main() -> int {

    if (glfwInit() != GLFW_TRUE) {
        return -1;
    }

    static auto window_width { 1366 };
    static auto window_height { 768 };
    static bool window_closed { };

    const auto window = glfwCreateWindow(window_width, window_height, "Project Stone", nullptr);

    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, [](int width, int height) {
        window_width = width;
        window_height = height;
    });

    glfwSetKeyCallback(window, [](int key, int scancode, int action, int mods) {
       if (key == GLFW_KEY_ESCAPE) {
           window_closed = true;
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
    core::texture::data water_1_texture_data;

    ground_1_texture_data.ptr = stbi_load("ground_1.png", &ground_1_texture_data.width, &ground_1_texture_data.height, &nr_channels, 0);
    water_1_texture_data.ptr = stbi_load("snow.png", &water_1_texture_data.width, &water_1_texture_data.height, &nr_channels, 0);

    if (ground_1_texture_data.ptr == nullptr) {
        return -2;
    }
    if (water_1_texture_data.ptr == nullptr) {
        return -2;
    }

    opengl::Texture ground_1_texture;
    ground_1_texture.type(opengl::constants::texture_2d);
    ground_1_texture.create();
    ground_1_texture.storage(ground_1_texture_data, GL_RGBA8);
    ground_1_texture.update(ground_1_texture_data, GL_RGBA);

    opengl::Texture water_1_texture;
    water_1_texture.type(opengl::constants::texture_2d);
    water_1_texture.create();
    water_1_texture.storage(water_1_texture_data, GL_RGBA8);
    water_1_texture.update(water_1_texture_data, GL_RGBA);

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

    opengl::Commands::clear(0.5f, 0.5f, 0.5f, 1.0f);

    opengl::Pipeline::enable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    constexpr auto tile_rows = 8;
    constexpr auto tile_cols = 8;

    map_tile tiles[tile_rows][tile_cols] {};

    // auto increment = 0;

    for (auto row = 0; row < tile_rows; row++) {
        const auto offset_x = row * tile_half_width;
        const auto offset_y = row * tile_half_height;

        for (auto column = 0; column < tile_cols; column++) {
            auto& map_tile = tiles[row][column];

            auto x = offset_x + column * tile_half_width;
            auto y = offset_y - column * tile_half_height;

            constexpr auto noise_x_factor = 0.1f;
            constexpr auto noise_y_factor = 0.1f;

            const auto noise = glm::simplex(glm::vec2(x * noise_x_factor, y * noise_y_factor)); // EP4: 25:00
            //const auto noise = glm::simplex(glm::vec3(x * noise_x_factor, 1.0f, y * noise_y_factor)); // EP4: 35:00

            map_tile.posistion = {x, y};
            if (noise <= -0.5f) {
                map_tile.type = water;
                // increment++;
            }
            else {
                map_tile.type = ground;
            }
        }
    }

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

        tile_vao.bind();

        for (auto row = 0; row < tile_rows; row++) {
            for (auto column = 0; column < tile_cols; column++) {
                auto& map_tile = tiles[row][column];

                if (map_tile.type == 0) {
                    ground_1_texture.bind_unit(core::texture::albedo);
                }
                else if (map_tile.type == 1) {
                    water_1_texture.bind_unit(core::texture::albedo);
                }

                model = glm::translate(glm::mat4(1.0f), glm::vec3(map_tile.posistion, 0.0f));
                transformed_ubo.update(core::buffer::make_data(&model));

                opengl::Commands::draw_elements(opengl::constants::triangles, tile_elements.size());
            }
        }

        glfwSwapBuffers(window);
    }

    return 0;
}
