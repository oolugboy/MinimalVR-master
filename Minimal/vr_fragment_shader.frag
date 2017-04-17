#version 130

out vec4 fragColor;
in vec3 outColor;

void main(void) {   
    fragColor = vec4(outColor, 1.0);
}