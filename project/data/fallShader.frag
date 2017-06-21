precision mediump float;
precision mediump int;

//This is the light datatype
struct PointLight {
    vec4 position;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float attenuation;
    float radius;
};

uniform float offset;
uniform sampler2D fall1;
uniform sampler2D fall2;
uniform sampler2D fall3;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

uniform mat4 inverseModel;
uniform mat4 inverseView;
uniform mat4 inverseProjection;

uniform vec3 eyePosition;

uniform int blueShift;
uniform int celShader;

uniform vec3 ambient;

uniform PointLight lights[4];
uniform int numLights;

uniform int numPass;

uniform vec4 lightPos;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;		// specular material exponent (shininess)

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;

varying vec2 fragTexCoord;
varying vec3 fragTangent;
varying vec3 fragBitangent;
varying vec3 fragNormal;
varying vec4 fragPosition;

float lambertWeight(vec3 n, vec3 d) {
    return clamp(dot(n, d), 0.0, 1.0);
}

float phongWeight(
                  vec3 lightDirection,
                  vec3 surfaceNormal,
                  vec3 eyeDirection,
                  float shininess
                  ) {
	//calculate halfway vector
	vec3 h = normalize(lightDirection + eyeDirection);

	//calculate specular light intensity
	float specIntensity = pow(max(dot(h,surfaceNormal), 0.0), shininess);
	return clamp(specIntensity, 0.0, 1.0);
}

mediump mat3 tbn(vec3 tangent, vec3 normal) {
    mediump vec3 t = normalize(tangent - dot(normal, tangent) * normal);
    mediump vec3 b = cross(normal, t);
    
    if (dot(cross(t, normal), b) < 0.0){
        t = t * -1.0;
    }
    
    return mat3(t, b, normal);
}

vec4 reduceColors(vec4 color, float numColors) {
    color.r = floor(color.r * numColors + 0.5) / numColors;
    color.g = floor(color.g * numColors + 0.5) / numColors;
    color.b = floor(color.b * numColors + 0.5) / numColors;
    
    return color;
}

void main() {
    
	vec4 blueShiftWeight = vec4(1.0, 1.0, 6.0, 1.0);
    // Normalize position
    vec3 position = fragPosition.xyz;
    
    // Prepare lighting terms
    vec3 diffuseLight = vec3(0.0);
    vec3 specularLight = vec3(0.0);
    
    vec3 normalForTBN = normalize(fragNormal);
    mat3 tbn = tbn(fragTangent, normalForTBN);
    
    // Get textures
    vec4 c4 = (blueShift == 1 ? blueShiftWeight : vec4(1.0)) * texture2D(DiffuseMap, fragTexCoord);
	float ac4 = c4[3];

	vec2 tx1 = fragTexCoord;
	vec2 tx2 = fragTexCoord;
	vec2 tx3 = fragTexCoord;
	
	tx1.x += 2*sin(tx1.x * 20.0 + offset)/15;
	tx1.y -= 2*(0.03*offset + sin(tx1.y * 50.0 + offset) / 250.0);

	tx2.x += 2*sin(tx2.x * 20.0 + offset)/15;
	tx2.y -= 4*(0.03*offset + sin(tx2.y * 50.0 + offset) / 250.0);

	tx3.x += 2*sin(tx3.x * 20.0 + offset)/15;
	tx3.y -= 3*(0.03*offset + sin(tx3.y * 50.0 + offset) / 250.0);

	vec4 c1 = texture2D(fall1, tx1);
	float ac1 = c1[3];

	vec4 c2 = texture2D(fall2, tx2);
	float ac2 = c2[3];

	vec4 c3 = texture2D(fall3, tx3);
	float ac3 = c3[3];

	//c1 = ac1 * vec3(c1) + (1-ac1) * ac2 * vec3(c2);
	c1 = vec4(vec3(c1) * 6, ac1*1.01);
	c2 = vec4(vec3(c2) * 6, ac2*1.01);
	c3 = vec4(vec3(c3) * 6, ac3*1.01);

	c1 = vec4(ac1 * vec3(c1) + (1-ac1) * ac2 * vec3(c2), ac1+ac2);
	ac1 = c1[3];
	c1 = vec4(ac1 * vec3(c1) + (1-ac1) * ac3 * vec3(c3), ac1+ac3);
	//vec4 color = c1;

	vec4 color = vec4(ac1 * vec3(c1) + (1-ac1) * ac4 * vec3(c4), ac4+ac1);

    
    if(numPass == 1) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, color.w); // Draw first pass as black
        return; // nothing more to do here
    }
    
    vec3 spec = texture2D(SpecularMap, fragTexCoord).xyz;
    vec3 normal = texture2D(NormalMap, fragTexCoord).xyz;
    normal = normal * 2.0 - vec3(1.0);
    normal = normalize(tbn * normal);
    
    
    
    // Calculate diffuse and specular light
    // for each light
    for(int i = 0; i < numLights; ++i) {
        PointLight light = lights[i];
        
		light.radius += blueShift == 1 ? 500.0 : 0.0;

        // Normalize light direction
        vec3 lightVector = light.position.xyz - position;
        
		vec3 direction = normalize(lightVector);
        float distance = length(lightVector);
        
        float intensityBasedOnDistance = 0.0;
        
        if(distance < light.radius)
            intensityBasedOnDistance = 1.0 - distance / light.radius;
        
        // Calculate lambert weight
        float intensity = lambertWeight(normal, direction);
		vec3 diffuse = Kd * intensity * light.diffuse * light.intensity * intensityBasedOnDistance;
		diffuseLight += clamp(diffuse, 0.0, 1.0);
        
        
        if(intensity > 0.0) {
            // Calculate phong weight
            float phong = phongWeight(direction, normal, normalize(eyePosition - position), Ns);
			
			vec3 specularResult = Ks * phong * light.specular * light.intensity * intensityBasedOnDistance;
			specularLight += clamp(specularResult, 0.0, 1.0);
        }
    }
    
    if(celShader == 0) {
        vec4 ambientResult = vec4(Ka * ambient, 1.0) * color;
        vec4 diffuseResult = vec4(diffuseLight, 1.0) * color;
        vec4 specularResult = vec4(specularLight * spec, 0.0);
        
        // Set the final color
        gl_FragColor = ambientResult + diffuseResult + specularResult;
    }
    else {
        float numColors = 6.0;
        
        color = reduceColors(color, numColors);
        
         vec3 viewDirection = normalize(eyePosition - position);
        
        
        vec4 ambientResult = vec4(Ka * ambient, 1.0) * color;
        vec4 diffuseResult = reduceColors(vec4(diffuseLight, 1.0), numColors) * color;
        vec4 specularResult = reduceColors(vec4(specularLight * spec, 0.0), 4.0);
        
        vec4 lightColor = ambientResult + diffuseResult + specularResult;
        
        if (dot(viewDirection, normal) < 0.3)
            lightColor = vec4(vec3(0.0), lightColor.w);
        
        gl_FragColor = lightColor;
    }
}
