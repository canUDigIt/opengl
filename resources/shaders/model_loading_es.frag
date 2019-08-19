#version 300 es

in lowp vec2 TexCoords;

out lowp vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{    
    color = vec4(texture(texture_diffuse1, TexCoords));
}
