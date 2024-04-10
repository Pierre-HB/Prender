#version 330 core

struct lightCaster
{
	vec3 lightColor;
	int falloff;
	vec3 position;
	float cos_angle_min;
	vec3 direction;
	float cos_angle_max;
};

layout (std140) uniform lightCastersBlock
{
    lightCaster lightCasters[32];
}; 

in vec2 uv;
in vec3 n;
in vec3 pos;

out vec4 FragColor;

uniform sampler2D albedo;
uniform sampler2D roughness;
uniform int lightCasterID[5];
uniform vec4 ambiantColor;
uniform ivec2 dimensions;

// to smooth transition
/*const float[16] ditheringMatrix1 = float[16](0.22, 0.88, 0.11, 0.77,
                                            0.55, 0.33, 0.66, 0.44,
                                            0.11, 0.77, 0.22, 0.88,
                                            0.66, 0.44, 0.55, 0.33);*/

const float[16] ditheringMatrix = float[16](0.2, 0.8, 0.2, 0.8,
                                            0.6, 0.4, 0.6, 0.4,
                                            0.2, 0.8, 0.2, 0.8,
                                            0.6, 0.4, 0.6, 0.4);

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 getLightCasterColor(int lightID, vec3 viewDir, vec3 normal, vec3 position, float roughness)
{
    //no light targeted or light does not exist(not supposed to happend)
    if(lightID < 0 || lightCasters[lightID].falloff < 0)
        return vec3(0, 0, 0);

    vec3 lightDir;
    float d;
    if(isinf(lightCasters[lightID].position.x)){
        lightDir = lightCasters[lightID].direction;
        d = 1;
    }else{
        lightDir = position  - lightCasters[lightID].position;
        d = length(lightDir);
        lightDir /= d;
    }

    
    vec3 halfWay = normalize(viewDir + lightDir);

    vec3 diffuseColor = lightCasters[lightID].lightColor*max(0, -dot(normal, lightDir));

    roughness = pow(2, roughness);

    float tmp = (roughness+1)/(2*3.1415); //This one look better
    vec3 specularColor = tmp*lightCasters[lightID].lightColor*pow(max(0, -dot(normal, halfWay)), roughness);

    return (diffuseColor+specularColor)/pow(d, lightCasters[lightID].falloff);
}

void main()
{   
    vec4 obj_material = texture(roughness, uv);
    float rougness = obj_material.x*32; //rougness is encoded in a color (so < 1) but is supposed to be in the interval [0, 32]

    vec3 cameraPos = vec3(0, 0, 0);
    vec3 viewDir = normalize(pos-cameraPos);
    vec3 normal = normalize(n);

    vec4 objectColor = texture(albedo, uv);

    FragColor = objectColor*ambiantColor;
    for(int i = 0; i < 5; i++){
        FragColor += objectColor*vec4(getLightCasterColor(lightCasterID[i], viewDir, normal, pos, rougness), 0);
    }

    ivec2 xy = ivec2(gl_FragCoord.xy);
    xy /= 2;
    int ditheringHash = 4*(xy.x%4) + xy.y%4;

    if(obj_material.y >= ditheringMatrix[ditheringHash]){
        vec3 tmp = rgb2hsv(FragColor.xyz);
        tmp.x = 1-tmp.x;
        FragColor = vec4(hsv2rgb(tmp), 1);
    }

    //FragColor = vec4(gl_FragCoord.xy/vec2(dimensions), 0, 1);
    //FragColor = vec4((normal+vec3(1, 1, 1)/2), 1);
    //FragColor = obj_material;
};