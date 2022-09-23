/*
    File Name: text.frag
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#version 450 core

layout (location=0) in vec2 vUV;

uniform sampler2D uTexture2D;
uniform vec3 uColor;
uniform float uZDepth;

layout (location=0) out vec4 fFragClr;


void main()
{
	fFragClr = vec4(uColor, texture(uTexture2D, vec2(vUV.x, 1.0 - vUV.y)).r);
	if (fFragClr.a < 0.5)
	{
		discard;
	}
	gl_FragDepth = uZDepth;
}