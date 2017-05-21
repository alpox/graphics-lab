precision mediump float;
precision mediump int;

//This is the light datatype
struct PointLight {
    vec3 diffuse;
    vec3 specular;
    vec4 position;
    float shininess;
    float attenuation;
    float radius;
};

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

uniform mat4 inverseModel;
uniform mat4 inverseView;
uniform mat4 inverseProjection;

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

void main() {
    // Set texture coords
    fragTexCoord = TexCoord.st;
    
    // Set the normal and the position for usage in the
    // fragment shader
    fragBitangent = normalize(normalMatrix * Bitangent);
    fragTangent = normalize(normalMatrix * Tangent);
    fragNormal = normalize(normalMatrix * Normal);
    fragPosition = model * vec4(Position.xyz, 1.0);
    
    gl_Position = projection * view * fragPosition;
}
