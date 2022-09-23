/*
    File Name: texture.frag
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#version 450 core

layout (location=0) in vec2 vUV;

uniform sampler2D uTexture2D;
uniform float uZDepth;

layout (location=0) out vec4 fFragClr;


void main () 
{
	fFragClr = texture(uTexture2D, vUV);
	if (fFragClr.a < 0.3)
	{
		discard;
	}
	gl_FragDepth = uZDepth;
}
