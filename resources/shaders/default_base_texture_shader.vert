#version 460

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_texcoord;

layout (location = 0) out vec2 out_texcoord;

layout (binding = 0, std140) uniform u_transform
{
    mat4 model;
};

layout (binding = 1, std140) uniform u_camera
{
    mat4 view;
    mat4 proj;
};

void main()
{
    out_texcoord = in_texcoord;
    gl_Position = proj * view * model * vec4(in_position, 0.0f, 1.0f);
}