#version 330 core

in vec3 OutColor;
in vec3 OutNormal;
in vec3 OutCoord;
in vec3 OutPostion;

// 物体的材质结构
struct Material
{
    bool enabled;
    bool enabledColor;       // True使用颜色， false使用贴图
    bool lightEffect;        // 是否受灯光的影响

    sampler2D ambient;
    sampler2D diffuse;      // 漫反射贴图
    sampler2D specular;     // 镜面反射

    vec3 ambientColor;      // 环境光颜色
    vec3 diffuseColor;      // 漫反射颜色
    vec3 specularColor;     // 镜面反射颜色

    float shininess;        // 镜面发射系数

    // 针对YUV和RGB转换，扩展材质
    bool isUsedExtraSample;
    sampler2D extraSample1;
    sampler2D extraSample2;
    sampler2D extraSample3;
};

// 光的材质结构
struct LightMaterial
{
    int type;           // 0: 平行光；1：点光源；2：聚光灯；3：手电筒

    vec3 ambientColor;      // 环境光颜色
    vec3 diffuseColor;      // 漫反射颜色
    vec3 specularColor;     // 镜面反射颜色

    vec3 lightDirection;    // 光的方向
    vec3 lightPoint;        // 光的位置

    // 衰减相关
    float constant;         // 衰减常数
    float linear;           // 一次项系数
    float quadratic;        // 二次系数

    // 聚光灯相关
    float cutoff;       // 聚光灯的切角
    float outerCutoff;    // 外圆锥的切角
};

uniform Material objectMaterial;            // 物体的材质
uniform LightMaterial lightMaterial[10];    // 光的材质
uniform vec3 ViewPostion;                   // 相机的位置
uniform int M_nLightCount = 0;              // 光的个数

vec3 yuvTorgb()
{
    vec3 yuv;
    vec3 rgb;

    yuv.x = texture2D(objectMaterial.extraSample1, OutCoord.xy).r;
    yuv.y = texture2D(objectMaterial.extraSample2, OutCoord.xy).r - 0.5;
    yuv.z = texture2D(objectMaterial.extraSample3, OutCoord.xy).r - 0.5;

    rgb = mat3(1,       1,         1,
               0,       -0.39465,  2.03211,
               1.13983, -0.58060,  0) * yuv;

    return rgb;
}

vec3 rgbToyuv()
{
    vec3 yuv;
    vec3 rgb;

    rgb = vec3(texture2D(objectMaterial.extraSample1, OutCoord.xy));


    yuv = mat3(0.299,       -0.169,       0.5,
               0.587,       -0.331,       -0.419,
               0.114,       0.5,          -0.081) * rgb;

    yuv = yuv + vec3(0.0, 0.5, 0.5);
    return yuv;
}

// =========================================================================================
// Get Object Material Color
// input 0: ambient; 1:diffuse; 2:specular
vec4 getObjectMaterialColor(int type)
{
    // 不使用物体的材质
    if (!objectMaterial.enabled)
        return vec4(OutColor, 1.0);

    // 使用材质
    if (objectMaterial.enabledColor)
    {
        if (type == 0)
            return vec4(objectMaterial.ambientColor, 1.0);
        else if (type == 1)
            return vec4(objectMaterial.diffuseColor, 1.0);
        else
            return vec4(objectMaterial.specularColor, 1.0);
    }

    // 使用扩展
    if (objectMaterial.isUsedExtraSample)
    {
        vec3 result = yuvTorgb();
        return vec4(result, 1.0);
    }

    // 使用纹理贴图
    vec4 resultColor;
    if (type == 0)
        resultColor = texture(objectMaterial.ambient, OutCoord.xy);
    else if (type == 1)
        resultColor = texture(objectMaterial.diffuse, OutCoord.xy);
    else
        resultColor = texture(objectMaterial.specular, OutCoord.xy);

    return resultColor;
}
// =========================================================================================

// =====================================灯光处理====================================================
// 处理方向光
vec3 processDirectionLight(int index)
{
    // 环境光
    vec3 ambient = lightMaterial[index].ambientColor * vec3(getObjectMaterialColor(0));

    // 漫反射
    vec3 lightDir = normalize(-lightMaterial[index].lightDirection);
    float diff = max(dot(OutNormal, lightDir), 0);
    vec3 diffuse = diff * lightMaterial[index].diffuseColor * vec3(getObjectMaterialColor(1));

    // 鏡面反射
    vec3 viewDir = normalize(ViewPostion - OutPostion);
    vec3 reflectDir = reflect(-lightDir, OutNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
    vec3 specular = spec * lightMaterial[index].specularColor * vec3(getObjectMaterialColor(2));

    return ambient + diffuse + specular;
}

// 处理点光源
vec3 processPointLight(int index)
{
    // 计算点到光源的距离
    float distance = length(lightMaterial[index].lightPoint - OutPostion);
    // 计算光的衰减
    float attenuation = 1.0 / (lightMaterial[index].constant + lightMaterial[index].linear * distance + lightMaterial[index].quadratic * (distance * distance));

    // 环境光
    vec3 ambient = lightMaterial[index].ambientColor * vec3(getObjectMaterialColor(0));
    ambient *= attenuation;

    // 漫反射
    vec3 lightDir = normalize(lightMaterial[index].lightPoint - OutPostion);
    float diff = max(dot(OutNormal, lightDir), 0);
    vec3 diffuse = diff * lightMaterial[index].diffuseColor * vec3(getObjectMaterialColor(1));
    diffuse *= attenuation;

    // 鏡面反射
    vec3 viewDir = normalize(ViewPostion - OutPostion);
    vec3 reflectDir = reflect(-lightDir, OutNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
    vec3 specular = spec * lightMaterial[index].specularColor * vec3(getObjectMaterialColor(2));
    specular *= attenuation;

    return ambient + diffuse + specular;
}

// 处理聚光灯
vec3 processSpotLight(int index)
{
    // 环境光
    vec3 ambient = lightMaterial[index].ambientColor * vec3(getObjectMaterialColor(0));

    vec3 lightDir = normalize(lightMaterial[index].lightPoint - OutPostion);
    float theta = dot(lightDir, normalize(-lightMaterial[index].lightDirection));
    float epsilon = lightMaterial[index].cutoff - lightMaterial[index].outerCutoff;
    float intensity = clamp((theta - lightMaterial[index].outerCutoff) / epsilon, 0.0, 1.0);

    // 漫反射
    float diff = max(dot(OutNormal, lightDir), 0);
    vec3 diffuse = diff * lightMaterial[index].diffuseColor * vec3(getObjectMaterialColor(1));
    diffuse *= intensity;

    // 鏡面反射
    vec3 viewDir = normalize(ViewPostion - OutPostion);
    vec3 reflectDir = reflect(-lightDir, OutNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
    vec3 specular = spec * lightMaterial[index].specularColor * vec3(getObjectMaterialColor(2));
    specular *= intensity;

    return ambient + diffuse + specular;
}
// =========================================================================================

uniform vec3 M_factor;

void main(void)
{
    if (!objectMaterial.lightEffect)
    {
        // 使用漫反射的图片或者颜色
        vec3 tempVecColor = vec3(getObjectMaterialColor(1));
        gl_FragColor = vec4(M_factor * tempVecColor, 1.0);
        return;
    }

    // 添加光照叠加效果
    vec3 resultColor = vec3(0.0, 0.0, 0.0);
    for (int i=0; i<M_nLightCount; ++i)
    {
        if (lightMaterial[i].type == 0)
        {
            resultColor += processDirectionLight(i);
        }
        else if (lightMaterial[i].type == 1)
        {
            resultColor += processPointLight(i);
        }
        else if (lightMaterial[i].type == 2)
        {
            resultColor += processSpotLight(i);
        }
    }

    vec4 destColor = vec4(M_factor * resultColor, 1.0);
    gl_FragColor = destColor;
}
