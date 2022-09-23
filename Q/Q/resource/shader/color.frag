/*
    File Name: color.frag
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#version 450 core

layout (location=0) in vec4 vColor;

uniform float uZDepth;

layout (location=0) out vec4 fFragClr;


void main () 
{
	fFragClr = vColor;
	gl_FragDepth = uZDepth;
}
