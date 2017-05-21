precision mediump float;
precision mediump int;

//This is the light datatype
struct PointLight {
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float shininess;
    float attenuation;
    float radius;
};

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

attribute vec3 eyePosition;

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

varying vec4 fragTexCoord;
varying vec3 fragTangent;
varying vec4 fragNormal;
varying vec4 fragPosition;

void main() {
    // Set texture coords
    fragTexCoord = TexCoord;
    
    // Set tangent
    fragTangent = normalMatrix * Tangent;
    
    // Set the normal and the position for usage in the
    // fragment shader
    fragNormal = vec4(normalMatrix * Normal, 0.0);
    fragPosition = model * vec4(Position.xyz, 1.0);
    
    gl_Position = projection * view * fragPosition;
}
