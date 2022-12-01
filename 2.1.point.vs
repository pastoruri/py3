#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 color;
    vec3 normal;
    vec3 fragPos;
} vs_out;

void main() {
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(vs_out.fragPos, 1.0);
    vs_out.color = vec3(1,1,0);
    vs_out.normal = aNormal;
}