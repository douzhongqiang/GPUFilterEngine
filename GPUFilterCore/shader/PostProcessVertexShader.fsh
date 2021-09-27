#version 330 core

in vec3 OutColor;
in vec3 OutNormal;
in vec3 OutCoord;
in vec3 OutPostion;

uniform sampler2D fboSample;
uniform int m_PostProcessType;      // 0，正常; 1，转成YUV; 
uniform int width;
uniform int height;

uniform bool m_isBGR0;

vec3 rgbToyuv(vec2 coord)
{
    vec3 yuv;
    vec3 rgb;

    int srcWidth = width;
    int srcHeight = height;
    int sampleInterVal = 0;

    float tempXVal = coord.x * srcWidth * 1.0 / (srcWidth + sampleInterVal);
    vec2 tempCoord = vec2(tempXVal, coord.y);
    rgb = vec3(texture(fboSample, tempCoord));
    if (m_isBGR0)
        rgb = vec3(rgb.b, rgb.g, rgb.r);

//    yuv = mat3(0.299,       -0.169,       0.5,
//               0.587,       -0.331,       -0.419,
//               0.114,       0.5,          -0.081) * rgb;

    yuv = mat3(0.183,       -0.101,       0.439,
               0.614,       -0.339,      -0.399,
               0.062,       0.439,       -0.040) * rgb;

    yuv = yuv + vec3(16.0 / 255, 0.5, 0.5);
    return yuv;
}

float converToRangeNormal(float minValue, float maxValue, float value)
{
    return (value - minValue) / (maxValue - minValue);
}

vec4 converToYUV()
{
    float interval = 1.0 / width;
    vec4 resultVec = vec4(0.0, 0.0, 0.0, 0.0);

    bool isY = OutPostion.y > (1.0 / 3);
    bool isU = !isY && (OutPostion.x < 0);

    if (isY)
    {
        float yCoord = converToRangeNormal(1.0 / 3, 1.0, OutPostion.y);
//        yCoord = 1.0 - yCoord;

        float r = rgbToyuv(vec2(OutCoord.x, yCoord)).x;
        float g = rgbToyuv(vec2(OutCoord.x + interval, yCoord)).x;
        float b = rgbToyuv(vec2(OutCoord.x + interval * 2.0f, yCoord)).x;
        float a = rgbToyuv(vec2(OutCoord.x + interval * 3.0f, yCoord)).x;

        resultVec = vec4(r, g, b, a);
        //resultVec = vec4(0.0, 0.0, 0.0, a);
    }
    else if (isU)
    {
        float yCoord = converToRangeNormal(0.0, 1.0 / 3, OutPostion.y);
//        yCoord = 1.0 - yCoord;
        float intervalY = 1.0 / height;

        float r = rgbToyuv(vec2(OutCoord.x * 2.0, yCoord)).y;
        float g = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f, yCoord)).y;
        float b = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f * 2.0f, yCoord)).y;
        float a = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f * 3.0f, yCoord)).y;

        float r2 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval, yCoord)).y;
        float g2 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f + interval, yCoord)).y;
        float b2 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f * 2.0f + interval, yCoord)).y;
        float a2 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f * 3.0f + interval, yCoord)).y;

        float r3 = rgbToyuv(vec2(OutCoord.x * 2.0, yCoord + intervalY)).y;
        float g3 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f, yCoord + intervalY)).y;
        float b3 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f * 2.0f, yCoord + intervalY)).y;
        float a3 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f * 3.0f, yCoord + intervalY)).y;

        float r4 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval, yCoord)).y;
        float g4 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f + interval, yCoord + intervalY)).y;
        float b4 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f * 2.0f + interval, yCoord + intervalY)).y;
        float a4 = rgbToyuv(vec2(OutCoord.x * 2.0 + interval * 2.0f * 3.0f + interval, yCoord + intervalY)).y;

        r = (r + r2 + r3 + r4) / 4.0;
        g = (g + g2 + g3 + g4) / 4.0;
        b = (b + b2 + b3 + b4) / 4.0;
        a = (a + a2 + a3 + a4) / 4.0;

        resultVec = vec4(r, g, b, a);
//        resultVec = vec4(0.5, 0.5, 0.5, 0.5);
    }
    else
    {
        float yCoord = converToRangeNormal(0.0, 1.0 / 3, OutPostion.y);
        float intervalY = 1.0 / height;
//        yCoord = 1.0 - yCoord;

        float r = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2, yCoord)).z;
        float g = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f, yCoord)).z;
        float b = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f * 2.0f, yCoord)).z;
        float a = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f * 3.0f, yCoord)).z;

        float r2 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval, yCoord)).z;
        float g2 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f + interval, yCoord)).z;
        float b2 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f * 2.0f + interval, yCoord)).z;
        float a2 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f * 3.0f + interval, yCoord)).z;

        float r3 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2, yCoord + intervalY)).z;
        float g3 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f, yCoord + intervalY)).z;
        float b3 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f * 2.0f, yCoord + intervalY)).z;
        float a3 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f * 3.0f, yCoord + intervalY)).z;

        float r4 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval, yCoord + intervalY)).z;
        float g4 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f + interval, yCoord + intervalY)).z;
        float b4 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f * 2.0f + interval, yCoord + intervalY)).z;
        float a4 = rgbToyuv(vec2((OutCoord.x - 0.5 - interval) * 2 + interval * 2.0f * 3.0f + interval, yCoord + intervalY)).z;

        r = (r + r2 + r3 + r4) / 4.0;
        g = (g + g2 + g3 + g4) / 4.0;
        b = (b + b2 + b3 + b4) / 4.0;
        a = (a + a2 + a3 + a4) / 4.0;

        resultVec = vec4(r, g, b, a);
//          resultVec = vec4(0.5, 0.5, 0.5, 0.5);
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
}
