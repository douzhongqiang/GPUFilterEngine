#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 coord;

// 动画相关
layout (location = 4) in ivec4 boneIds;
layout (location = 5) in vec4 weights;

uniform bool M_isAnimation;
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 OutColor;
out vec3 OutNormal;
out vec3 OutCoord;
out vec3 OutPostion;

void main(void)
{
    vec3 normalVec = vec3(1.0);
    vec4 totalPosition = vec4(0.0f);

    if (M_isAnimation)
    {
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if (boneIds[i] == -1)
            {
                totalPosition = vec4(pos, 1.0);
                continue;
            }

            if(boneIds[i] >= MAX_BONES)
            {
                totalPosition = vec4(pos,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(pos, 1.0f);
            totalPosition += localPosition * weights[i];
            vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
            normalVec += localNormal * weights[i];
       }
        //totalPosition = vec4(pos, 1.0);
        gl_Position = P * V * M * totalPosition;
    }
    else
    {
        gl_Position = P * V * M * vec4(pos, 1.0);
        totalPosition = vec4(pos, 1.0);
    }

    // 法线
    vec3 tempNormal;
    if (M_isAnimation)
        tempNormal = mat3(transpose(inverse(M))) * normalize(normalVec);
    else
        tempNormal = mat3(transpose(inverse(M))) * normalize(normal);
    OutNormal = normalize(tempNormal);

    // 颜色、纹理坐标、点的位置等信息
    OutColor = color;
    OutCoord = coord;
    OutPostion = vec3(M * totalPosition);
}
