precision mediump float;
precision mediump int;

//This is the light datatype
struct PointLight {
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float shininess;			//change to intensity please :)
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

    //First reflect light by surface normal
    //vec3 rlight = reflect(lightDirection, surfaceNormal);
    
    //Next find the projected length of the reflected
    //light vector in the view direction
    //float eyeLight = max(dot(rlight, eyeDirection), 0.0);
    
    //Finally exponentiate by the shininess factor
    //return clamp(pow(eyeLight, shininess), 0.0, 1.0);
}

mediump mat3 tbn(vec3 tangent, vec3 normal) {
    mediump vec3 t = normalize(tangent - dot(normal, tangent) * normal);
    mediump vec3 b = cross(normal, t);
    
    if (dot(cross(t, normal), b) < 0.0){
        t = t * -1.0;
    }
    
    //vec3 firstRow = vec3(t.x, b.x, normal.x);
    //vec3 secondRow = vec3(t.y, b.y, normal.y);
    //vec3 thirdRow = vec3(t.z, b.z, normal.z);

	vec3 firstRow = vec3(t.x, t.y, t.z);
    vec3 secondRow = vec3(b.x, b.y, b.z);
    vec3 thirdRow = vec3(normal.x, normal.y, normal.z);
    
    return mat3(firstRow, secondRow, thirdRow);
}

mediump mat3 alttbn(vec3 tangent, vec3 normal, vec3 bitangent) {
	mediump vec3 t = normalize(tangent - dot(normal, tangent) * normal);
	mediump vec3 b = normalize(bitangent - dot(normal, bitangent) * normal - dot(t,bitangent) * t);

	return mat3(t,b,normal);
}


void main() {
    // Normalize position
    vec3 position = fragPosition.xyz;
    
    // Prepare lighting terms
    vec3 diffuseLight = vec3(0.0);
    vec3 specularLight = vec3(0.0);
    
    vec3 normalForTBN = normalize(fragNormal);
	vec3 tempNorm = normalize((fragNormal + vec3(1.0))/2);
    mat3 tbn = tbn(fragTangent, normalForTBN);
	//mat3 tbn = alttbn(fragTangent, normalForTBN, fragBitangent);
    
    // Get textures
    vec3 color = texture2D(DiffuseMap, fragTexCoord).xyz;
    vec3 spec = texture2D(SpecularMap, fragTexCoord).xyz;
    vec3 normal = texture2D(NormalMap, fragTexCoord).xyz;
    normal = normal * 2.0 - vec3(1.0);											//???
	normal = normalize(tbn * normal);
    
    // Calculate diffuse and specular light
    // for each light
    for(int i = 0; i < numLights; ++i) {
        PointLight light = lights[i];
        
        // Normalize light direction
        //vec3 direction = tbn * normalize(light.position - position);
		vec3 direction = normalize(light.position.xyz - position);				//without TBN
        
        // Calculate lambert weight
        float intensity = lambertWeight(normal, direction);
		vec3 diffuse = Kd * intensity * light.diffuse * light.shininess;
		diffuseLight = clamp(diffuse, 0.0, 1.0);			//clamp diffuse term again after multiplying Kd
        
        // Sum up diffuse light
        //diffuseLight += clamp(light.diffuse * intensity, 0.0, 1.0);
        
        if(intensity > 0.0) {
            // Calculate phong weight
            float phong = phongWeight(direction, normal, normalize(eyePosition - position), Ns);
			
			vec3 specularResult = Ks * phong * light.specular * light.shininess;
			specularLight = clamp(specularResult, 0.0, 1.0);

            //vec3 specularResult = light.specular * phong * light.shininess;
            //specularLight += clamp(specularResult, 0.0, 1.0);
        }
    }
    
    // Set the final color
    gl_FragColor = vec4(Ka * ambient * color +
                        diffuseLight * color +
                        specularLight * spec, 1.0); //forgot to multiply specular map

	//gl_FragColor = vec4(tempNorm, 1.0);
}
