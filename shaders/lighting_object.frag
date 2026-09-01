#version 330 core

struct Material {
    // Assuming 32 maximum textures on the machine's gpu
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

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};

out vec4 FragColor;

#define NR_POINT_LIGHTS 4
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos; // camera position
uniform Material material;
uniform SpotLight spotLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords)) * diff;
    vec3 specular =  light.specular * vec3(texture(material.specular, TexCoords)) * spec;

    return (ambient + diffuse + specular);
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float atten_divisor = (light.constant) + (distance * light.linear)
        + ((distance * distance) * light.quadratic);

    float attenuation = 1.0 / atten_divisor; 

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // ambient *= 0.3;
    // diffuse *= 0.3;
    // specular *= 0.3;
    return (ambient + diffuse + specular);
};

vec3 CalcSpotlight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float atten_divisor = (light.constant) + (distance * light.linear)
        + ((distance * distance) * light.quadratic);
    float attenuation = 1.0 / atten_divisor; 

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
};

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 resColor = vec3(0.0);
    resColor += CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        resColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    resColor += CalcSpotlight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(resColor, 1.0);
}

// Write some light calculation functions here 
