#version 460 core

out vec4 out_color;
uniform vec3 light;
uniform vec3 color;

void main() {
    out_color = vec4(light * color, 1.0);
}