#version 130

uniform mat4 projection = mat4(1);
uniform mat4 modelview = mat4(1);
uniform vec3 color;

//layout(location = 0)
in vec3 position;
//layout(location = 1) 
in vec3 normal;

out vec3 outColor;
void main(void) {
   gl_Position = projection * modelview * vec4(position, 1.0f);
   outColor = color;
}