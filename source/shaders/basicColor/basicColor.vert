#version 330 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormals;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec4 vertexColor;

out vec4 fragmentColor;

void main() {
    fragmentColor = vertexColor;
    gl_Position = vertexPosition;
}
