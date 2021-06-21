#version 330 core

in vec3 OutColor;
in vec3 OutNormal;
in vec3 OutCoord;
in vec3 OutPostion;

uniform sampler2D fboSample;

void main(void)
{
    gl_FragColor = texture(fboSample, OutCoord.xy);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
