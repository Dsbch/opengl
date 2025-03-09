#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coords;
layout(location = 2) in int tex_index;

out vec2 v_tex_coords;
out float v_tex_index;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    gl_Position = u_projection * u_view * vec4(position.xyz, 1.0f);
    v_tex_coords = tex_coords;
    v_tex_index = tex_index;
}