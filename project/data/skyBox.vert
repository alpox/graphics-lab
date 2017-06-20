precision highp float;

attribute vec4 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform samplerCube skyCube;

varying vec3 fragTexCoord;

void main() {
	vec4 fragPosition = model * vec4(Position.xyz, 1.0);
	fragTexCoord = vec3(fragPosition);
    
    gl_Position = projection * view * fragPosition;
}
