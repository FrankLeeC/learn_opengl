#version 460 core

in vec2 TexCoords;
in vec3 normal;
in vec3 frag_position;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 out_color;
uniform vec3 view_position;
uniform Material material;
uniform Light light;

void main() {
    vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;

    vec3 n = normalize(normal);
    vec3 l = normalize(light.position - frag_position);
    float factor = max(dot(n,l), 0.0);
    vec3 diffuse = factor * texture(material.diffuse, TexCoords).rgb * light.diffuse;

    vec3 ref = reflect(-l, n);
    vec3 v = normalize(view_position - frag_position);
    float spec = pow(max(dot(ref, v), 0.0), material.shininess);
    vec3 specular = texture(material.specular, TexCoords).rgb * spec * light.specular;

    out_color = vec4(ambient + diffuse + specular, 1.0);
}