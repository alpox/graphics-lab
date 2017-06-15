precision highp float;
precision highp int;

attribute vec4 Position;
varying vec2 vUv;

void main() {
    vUv = 0.5 * (Position.xy+1.0);
    
    gl_Position = vec4(Position.xy, 0.0, 1.0);
}
