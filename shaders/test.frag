#version 460 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture1;
uniform sampler2D screenTexture2;
uniform vec2 resolution; 

const float offset_x = 1.0f / 800.0f;  
const float offset_y = 1.0f / 800.0f;  

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

float kernel[9] = float[]
(
    1,  1, 1,
    1, -10, 1,
    1,  1, 1
);

void main()
{
    vec4 tex = vec4(vec3(texture(screenTexture2, TexCoords)), 1.0f);
    vec3 color = vec3(0.0f);
    color += vec3(texture(screenTexture1, TexCoords)); 
    for(int i = 0; i < 9; i++)
        color += vec3(texture(screenTexture1, TexCoords.st + offsets[i])) * kernel[i];

    vec4 col = vec4(color.r, color.g, color.b, 1.0f);
    //FragColor = vec4(tex.r+col.r, tex.g+col.g, tex.b+col.b, tex.a+col.a);
    FragColor   = (col+tex) / vec4(2.0f);
}
