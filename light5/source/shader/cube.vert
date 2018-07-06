#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 a_normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_trans;

out vec3 normal;
out vec3 frag_position;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    normal = normal_trans * a_normal;
    frag_position = vec3(model*vec4(pos, 1.0));
}