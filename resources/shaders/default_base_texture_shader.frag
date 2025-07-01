#version 460

layout (location = 0) in vec2 in_texcoord;

layout (location = 0) out vec4 out_color;

layout (location = 0) uniform sampler2D albedo_texture;

layout (binding = 2, std140) uniform v_material
{
    vec4 albedo;
};

void main()
{
    out_color = texture(albedo_texture, in_texcoord) * albedo;
}