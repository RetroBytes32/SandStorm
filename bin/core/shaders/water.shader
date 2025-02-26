[begin] vertex

#version 330 core

layout(location = 0) in vec3 l_position;
layout(location = 1) in vec3 l_color;
layout(location = 2) in vec3 l_normal;
layout(location = 3) in vec2 l_uv;

uniform mat4 u_proj;
uniform mat4 u_model;
uniform mat4 u_shadow;
uniform mat3 u_inv_model;

uniform vec3 u_eye;
uniform vec3 u_angle;

varying vec2 v_coord;
varying vec3 v_color;

uniform vec3 m_ambient;
uniform vec3 m_diffuse;
uniform vec3 m_specular;

uniform int   u_light_count;
uniform vec3  u_light_position[100];
uniform vec3  u_light_direction[100];
uniform vec4  u_light_attenuation[100];
uniform vec3  u_light_color[100];

void main() {
    
    vec4 vertPos = u_model * vec4(l_position, 1);
    
    vec3 norm = u_inv_model * normalize(l_normal);
    
    vec3 lightColor = m_ambient;
    
    for (int i=0; i < u_light_count; i++) {
        
        float intensity    = u_light_attenuation[i].r;
        float range        = u_light_attenuation[i].g;
        float attenuation  = u_light_attenuation[i].b;
        float type         = u_light_attenuation[i].a;
        
        // 0 - Point light
        if (type < 1) {
            
            // Light MAX distance
            float dist = length( u_light_position[i] - vec3(vertPos));
            
            if (dist > range) 
                continue;
            
            vec3 lightDir = normalize(u_light_position[i] - vec3(vertPos));
            
            float diff = max(dot(norm, lightDir), 0.0);
            
            // Specular
            vec3 viewDir = normalize(u_eye - vec3(vertPos));
            vec3 reflectDir = reflect(-lightDir, norm);  
            float shininess = 1;
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = u_light_color[i] * (spec * m_specular);
            
            lightColor += ((diff * u_light_color[i]) * intensity) / (1.0 + (dist * attenuation)) + specular;
            
            continue;
        }
        
        // 1 - Directional light
        if (type < 2) {
            
            vec3 lightDir = normalize(-u_light_direction[i]);
            
            float diff = max(dot(norm, lightDir), 0.0);
            
            lightColor += (diff * u_light_color[i]) * intensity;
            
            continue;
        }
        
        continue;
    }
    
    v_color = m_diffuse * l_color;
    v_coord = l_uv;
    
    gl_Position = u_proj * vertPos;
    
    return;
};

[end]



[begin] fragment

#version 330 core

varying vec3 v_color;
varying vec2 v_coord;

uniform sampler2D u_sampler;

out vec4 color;

void main() {
    
    float Gamma = 2.2;
    
    color = vec4( pow(v_color.rgb, vec3(1.0/Gamma)), 1);
    
    return;
}

[end]
