#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 coord;

out vec3 OutColor;
out vec3 OutNormal;
out vec3 OutCoord;
out vec3 OutPostion;

uniform mat4 m;

void main(void)
{
    // 颜色、纹理坐标、点的位置等信息
    OutNormal = normal;
    OutColor = color;
    OutCoord = coord;

    vec3 tempPos = vec3(vec4(pos, 1.0));
    tempPos = vec3(tempPos.x, 1.0 - (tempPos.y + 1.0) * 0.5, tempPos.z);
    OutPostion = tempPos;     // 转换为[0, 1]

    gl_Position = vec4(pos, 1.0);
}
