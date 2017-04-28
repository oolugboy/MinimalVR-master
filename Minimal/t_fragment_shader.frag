#version 410 core

out vec4 fragColor;
in vec2 outTexCoord;

uniform sampler2D ourTexture;

void main(void) {   
    fragColor = texture(ourTexture, outTexCoord);
}