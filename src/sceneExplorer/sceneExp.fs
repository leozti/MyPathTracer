#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec4 color;
uniform bool hasValidTexture;
uniform sampler2D texture;

void main() {    
    if (hasValidTexture) {
        FragColor = texture2D(texture, TexCoords);
    } else {
        FragColor = color;
    }
}