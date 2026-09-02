#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_diffuse4;
    sampler2D texture_diffuse5;
    sampler2D texture_diffuse6;
    sampler2D texture_diffuse7;
    sampler2D texture_diffuse8;
    sampler2D texture_diffuse9;
    sampler2D texture_diffuse10;
    sampler2D texture_diffuse11;
    sampler2D texture_diffuse12;
    sampler2D texture_diffuse13;
    sampler2D texture_diffuse14;
    sampler2D texture_diffuse15;
    sampler2D texture_diffuse16;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    sampler2D texture_specular4;
    sampler2D texture_specular5;
    sampler2D texture_specular6;
    sampler2D texture_specular7;
    sampler2D texture_specular8;
    sampler2D texture_specular9;
    sampler2D texture_specular10;
    sampler2D texture_specular11;
    sampler2D texture_specular12;
    sampler2D texture_specular13;
    sampler2D texture_specular14;
    sampler2D texture_specular15;
    sampler2D texture_specular16;
    float shininess;
};

out vec4 FragColor;

in vec2 TexCoords;

uniform Material material;

void main() {
        vec3 resColor = vec3(0.0);
        resColor += vec3(texture(material.texture_diffuse1, TexCoords));
        FragColor = vec4(resColor, 1.0);
}
