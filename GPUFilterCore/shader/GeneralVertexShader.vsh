#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 coord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 OutColor;
out vec3 OutNormal;
out vec3 OutCoord;
out vec3 OutPostion;

void main(void)
{
    gl_Position = P * V * M * vec4(pos, 1.0);

    // 法线
    vec3 tempNormal = mat3(transpose(inverse(M))) * normalize(normal);
    OutNormal = normalize(tempNormal);

    // 颜色、纹理坐标、点的位置等信息
    OutColor = color;
    OutCoord = coord;
    OutPostion = vec3(M * vec4(pos, 1.0));
}
