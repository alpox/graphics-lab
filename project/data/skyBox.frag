precision highp float;

uniform samplerCube skyCube;

varying vec3 fragTexCoord;

void main() {

	gl_FragColor = textureCube(skyCube, fragTexCoord);
    //gl_FragColor = vec4(1.0);
}
