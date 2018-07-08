#version 460 core

in vec3 normal;
in vec3 frag_position;

out vec4 out_color;
uniform vec3 light;
uniform vec3 color;
uniform vec3 view_position;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light;

    vec3 n = normalize(normal);
    vec3 l = normalize(light - frag_position);
    float factor = max(dot(n,l), 0.0);
    vec3 diffuse = factor * light;

    vec3 ref = reflect(-l, n);
    vec3 v = normalize(view_position - frag_position);
    float spec = pow(max(dot(ref, v), 0.0), 32);
    vec3 specular = 0.5 * spec * light;

    out_color = vec4((ambient + diffuse + specular) * color, 1.0);
}