#version 460 core

out vec4 out_color;
uniform vec3 light;
uniform vec3 color;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light;
    out_color = vec4(ambient * color, 1.0);
}