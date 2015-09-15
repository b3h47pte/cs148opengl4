#version 330 core

layout(location = 0) const in vec4 vertexPosition;
layout(location = 1) const in vec3 vertexNormals;
layout(location = 2) const in vec2 vertexUV;
layout(location = 3) const in vec4 vertexColor;

out vec4 fragmentColor;

void main() {
    fragmentColor= vertexColor;
    gl_Position = vertexPosition;
}
