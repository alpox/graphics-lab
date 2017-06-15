precision highp float;
precision mediump int;

uniform float exposure;
uniform float decay;
uniform float density;
uniform float weight;
uniform vec2 lightPositionOnScreen;
uniform sampler2D firstPass;
const int NUM_SAMPLES = 100 ;
varying vec2 vUv;

void main()
{
    vec3 fragColor = vec3(0.0,0.0,0.0);
    vec2 deltaTextCoord = vec2( vUv - lightPositionOnScreen.xy );
    vec2 textCoo = vUv;
    deltaTextCoord *= (1.0 / float(NUM_SAMPLES)) * density;
    float illuminationDecay = 1.0;
    
    
    for(int i=0; i < NUM_SAMPLES; ++i)
    {
        textCoo -= deltaTextCoord;
        vec3 sample = texture2D(firstPass, textCoo ).xyz;
        
        sample *= illuminationDecay * weight;
        
        fragColor += sample;
        
        illuminationDecay *= decay;
    }
    fragColor *= exposure;
    gl_FragColor = vec4(fragColor, 1.0);
}
