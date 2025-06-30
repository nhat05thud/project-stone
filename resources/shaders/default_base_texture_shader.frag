#version 460

layout (location = 0) in vec2 in_texcoord;

layout (location = 0) out vec4 out_color;

layout (location = 0) uniform sampler2D albedo_texture;

void main()
{
    out_color = texture(albedo_texture, in_texcoord);
}