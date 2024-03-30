#version 330 core

in vec2 uv;
in vec3 n;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D smiley;

void main()
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    
    vec4 wood = texture(ourTexture, uv);
    vec4 smiley = texture(smiley, uv);

    FragColor = vec4((wood.rgb*wood.a + smiley.rgb*smiley.a)/(wood.a+smiley.a), 1);

    //FragColor = vec4(vec3(1, 1, 1)*(normalize(n).z), 1);
    FragColor = vec4(wood.rgb*(1-smiley.a) + smiley.rgb*smiley.a, 1)*(normalize(n).z);


    //FragColor = mix(texture(ourTexture, uv), texture(smiley, uv), 0.2);
    //FragColor = texture(smiley, uv);
    //FragColor = texture(ourTexture, uv);
};