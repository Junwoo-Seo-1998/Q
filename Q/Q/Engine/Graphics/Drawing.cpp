/*
    File Name: Drawing.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:Junwoo Seo
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Drawing.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"


namespace q_engine
{
	void clear_background()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void clear_background(const Color color)
	{
        glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void swap_buffers()
	{
		glfwSwapBuffers(q_impl::p_window);
	}
} // namespace q_engine
