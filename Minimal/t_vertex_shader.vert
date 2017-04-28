#version 410 core

uniform mat4 projection = mat4(1);
uniform mat4 modelview = mat4(1);

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

out vec2 outTexCoord;
void main(void) {
   gl_Position = projection * modelview * vec4(position, 1.0f);
   outTexCoord = texCoord;
}