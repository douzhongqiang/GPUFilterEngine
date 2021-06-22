#version 330 core

in vec3 OutColor;
in vec3 OutNormal;
in vec3 OutCoord;
in vec3 OutPostion;

uniform sampler2D fboSample;
uniform int width;
uniform int height;

uniform int m_PostProcessType;      // 0，正常; 1，转成YUV; 

vec3 rgbToyuv(vec2 coord)
{
    vec3 yuv;
    vec3 rgb;

    rgb = vec3(texture2D(fboSample, coord));

    yuv = mat3(0.299,       -0.169,       0.5,
               0.587,       -0.331,       -0.419,
               0.114,       0.5,          -0.081) * rgb;

    yuv = yuv + vec3(0.0, 0.5, 0.5);
    return yuv;
}

float converToRangeNormal(float minValue, float maxValue, float value)
{
    return (value - minValue) / (maxValue - minValue);
}

vec4 converToYUV()
{
    float interval = 1.0 / width;
    vec4 resultVec;

    bool isY = OutPostion.y > (1.0 / 3 * 2);
    bool isU = !isY && (OutPostion.x < 0.5);

    if (isY)
    {
        float yCoord = converToRangeNormal(0.0, (1.0 / 3 * 2), OutPostion.y);
//        yCoord = 1.0 - yCoord;

        float r = rgbToyuv(vec2(OutCoord.x, yCoord)).x;
        float g = rgbToyuv(vec2(OutCoord.x + interval, yCoord)).x;
        float b = rgbToyuv(vec2(OutCoord.x + interval * 2.0f, yCoord)).x;
        float a = rgbToyuv(vec2(OutCoord.x + interval * 3.0f, yCoord)).x;

        resultVec = vec4(r, g, b, a);
    }
    else if (isU)
    {
        float yCoord = converToRangeNormal((1.0 / 3 * 2), 1.0, OutPostion.y);
//        yCoord = 1.0 - yCoord;

        float r = rgbToyuv(vec2(OutCoord.x * 2, yCoord)).y;
        float g = rgbToyuv(vec2(OutCoord.x * 2 + interval, yCoord)).y;
        float b = rgbToyuv(vec2(OutCoord.x * 2 + interval * 2.0f, yCoord)).y;
        float a = rgbToyuv(vec2(OutCoord.x * 2 + interval * 3.0f, yCoord)).y;

        resultVec = vec4(r, g, b, a);
    }
    else 
    {
        float yCoord = converToRangeNormal(0.0, (1.0 / 3 * 2), OutPostion.y);
//        yCoord = 1.0 - yCoord;

        float r = rgbToyuv(vec2((OutCoord.x - 0.5) * 2, yCoord)).y;
        float g = rgbToyuv(vec2((OutCoord.x - 0.5) * 2 + interval, yCoord)).y;
        float b = rgbToyuv(vec2((OutCoord.x - 0.5) * 2 + interval * 2.0f, yCoord)).y;
        float a = rgbToyuv(vec2((OutCoord.x - 0.5) * 2 + interval * 3.0f, yCoord)).y;

        resultVec = vec4(r, g, b, a);
    }

    return resultVec;
}

void main(void)
{
    if (m_PostProcessType == 1)
    {
        gl_FragColor = converToYUV();
        return;
    }

    gl_FragColor = texture(fboSample, OutCoord.xy);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
