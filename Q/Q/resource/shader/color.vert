/*
    File Name: color.vert
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:Junwoo Seo
    All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#version 450 core

layout (location=0) in vec2 vVertexPosition;

uniform vec4 uColor;
uniform mat4 uModel2NDC;

layout (location=0) out vec4 vColor;


void main() 
{
    
	gl_Position = uModel2NDC * vec4(vVertexPosition, 0.f, 1.f);
	vColor = uColor;
}