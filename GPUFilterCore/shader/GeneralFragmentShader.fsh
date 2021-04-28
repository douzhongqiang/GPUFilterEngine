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
};

// 光的材质结构
struct LightMaterial
{
    int type;           // 0: 平行光；1：点光源；2：聚光灯；3：手电筒

    vec3 ambientColor;      // 环境光颜色
    vec3 diffuseColor;      // 漫反射颜色
    vec3 specularColor;     // 镜面反射颜色

    // 针对方向光
    vec3 lightDirection;    // 光的方向

    // 针对点光源
    vec3 lightPoint;        // 点光源
    // 衰减相关
    float constant;         // 衰减常数
    float linear;           // 一次项系数
    float quadratic;        // 二次系数
};

uniform Material objectMaterial;            // 物体的材质
uniform LightMaterial lightMaterial[10];    // 光的材质
uniform vec3 ViewPostion;                   // 相机的位置
uniform int M_nLightCount = 0;              // 光的个数

// =========================================================================================
// Get Object Material Color
// input 0: ambient; 1:diffuse; 2:specular
vec4 getObjectMaterialColor(int type)
{
    if (!objectMaterial.enabled)
        return vec4(OutColor, 1.0);

    if (objectMaterial.enabledColor)
    {
        if (type == 0)
            return vec4(objectMaterial.ambientColor, 1.0);
        else if (type == 1)
            return vec4(objectMaterial.diffuseColor, 1.0);
        else
            return vec4(objectMaterial.specularColor, 1.0);
    }

    if (type == 0)
        return texture(objectMaterial.ambient, OutCoord.xy);
    else if (type == 1)
        return texture(objectMaterial.diffuse, OutCoord.xy);
    else
        return texture(objectMaterial.specular, OutCoord.xy);
}
// =========================================================================================

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

void main(void)
{
    if (!objectMaterial.lightEffect)
    {
        // 使用漫反射的图片或者颜色
        gl_FragColor = getObjectMaterialColor(1);
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
    }

    gl_FragColor = vec4(resultColor, 1.0);
}
