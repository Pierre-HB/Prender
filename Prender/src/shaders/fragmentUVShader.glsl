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

uniform sampler2D ourTexture;
uniform sampler2D smiley;
uniform int lightCasterID[5];
uniform vec4 ambiantColor;

const float specularDensity = 64;

vec3 getLightCasterColor(int lightID, vec3 viewDir, vec3 normal, vec3 position)
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
    vec3 specularColor = lightCasters[lightID].lightColor*pow(max(0, -dot(normal, halfWay)), specularDensity);

    return (diffuseColor+specularColor)/pow(d, lightCasters[lightID].falloff);
}

void main()
{   
    
    vec3 cameraPos = vec3(0, 0, 0);
    vec3 viewDir = normalize(pos-cameraPos);
    vec3 normal = normalize(n);

    vec4 wood = texture(ourTexture, uv);
    vec4 smiley = texture(smiley, uv);
    vec4 objectColor = vec4(wood.rgb*(1-smiley.a) + smiley.rgb*smiley.a, 1);


    FragColor = objectColor*ambiantColor;
    for(int i = 0; i < 5; i++){
        FragColor += objectColor*vec4(getLightCasterColor(lightCasterID[i], viewDir, normal, pos), 0);
    }

    //FragColor = vec4((normal+vec3(1, 1, 1)/2), 1);
};