precision mediump float;
precision mediump int;

//This is the light datatype
struct PointLight {
    vec4 position;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float attenuation;
    float radius;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

uniform mat4 inverseModel;
uniform mat4 inverseView;
uniform mat4 inverseProjection;

uniform vec3 eyePosition;

uniform vec3 ambient;

uniform PointLight lights[4];
uniform int numLights;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;

varying vec2 fragTexCoord;
varying vec3 fragTangent;
varying vec3 fragBitangent;
varying vec3 fragNormal;
varying vec4 fragPosition;

float phongWeight(
                  vec3 lightDirection,
                  vec3 surfaceNormal,
                  vec3 eyeDirection,
                  float shininess
                  ) {
    //First reflect light by surface normal
    vec3 rlight = reflect(lightDirection, surfaceNormal);
    
    //Next find the projected length of the reflected
    //light vector in the view direction
    float eyeLight = max(dot(rlight, eyeDirection), 0.0);
    
    //Finally exponentiate by the shininess factor
    return clamp(pow(eyeLight, shininess), 0.0, 1.0);
}

mediump mat3 tbn(vec3 tangent, vec3 bitangent, vec3 normal) {
    mediump vec3 t = normalize(tangent - dot(normal, tangent) * normal);
    mediump vec3 b = cross(normal, t);
    
    if (dot(cross(normal, t), b) < 0.0)
        t = t * -1.0;
    
    return mat3(t, b, normal);
}

void main() {
    // Normalize position
    vec3 position = fragPosition.xyz;
    
    // Prepare lighting terms
    vec3 diffuseLight = vec3(0.0);
    vec3 specularLight = vec3(0.0);
    
    mat3 tbn = tbn(normalize(fragTangent), normalize(fragBitangent),  normalize(fragNormal));
    
    // Get textures
    vec3 color = texture2D(DiffuseMap, fragTexCoord).xyz;
    vec3 spec = texture2D(SpecularMap, fragTexCoord).xyz;
    vec3 normal = texture2D(NormalMap, fragTexCoord).xyz;
    normal = normal * 2.0 - vec3(1.0);
    normal = normalize(tbn * normal);
    
    float intensity;
    
    // Calculate diffuse and specular light
    // for each light
    for(int i = 0; i < numLights; ++i) {
        PointLight light = lights[i];
        
        // Normalize light direction
        vec3 direction = normalize(light.position.xyz - position);
        
        // Calculate lambert weight
        intensity = clamp(dot(normal, direction), 0.0, 1.0);
        
        // Sum up diffuse light
        diffuseLight += clamp(light.diffuse * intensity * light.shininess, 0.0, 1.0);
        
        if(intensity > 0.0) {
            // Calculate phong weight
            float phong = phongWeight(direction, normal, normalize(eyePosition - position), Ns);
        
            vec3 specularResult = light.specular * phong * light.shininess;
            specularLight += clamp(specularResult, 0.0, 1.0);
        }
    }
    
    // Set the final color
    //gl_FragColor = vec4(Ka * ambient + clamp(vec3(normalize(lights[0].position)), 0.0, 1.0), 1.0);
    gl_FragColor = clamp(vec4(vec3(intensity), 1.0), 0.0, 1.0);
    //gl_FragColor = vec4(normal, 1.0);
}
