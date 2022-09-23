/*
    File Name: texture.vert
    Project Name: Q
    Author(s):
        Primary: Junwoo Seo
        Secondary:
    All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=2) in vec2 vVertexUV;

uniform mat3 uModel2NDC;
uniform int uFrameOfReference;

layout (location=0) out vec2 vUV;


void main() 
{
	vec2 offset = vec2(0, 0);

	switch (uFrameOfReference)
	{
		// top left
		case 1:
			offset = vec2(0, -1);
			break;
		
		// bottom center
		case 2:
			offset = vec2(-0.5, 0);
			break;
		
		// center
		case 3:
			offset = vec2(-0.5, -0.5);
			break;
	}

	gl_Position = vec4(vec2(uModel2NDC * vec3(vVertexPosition + offset, 1.f)), 0.0, 1.0);
	vUV = vVertexUV;
}
