/*
    File Name: Window.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Window.h"

#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


#include "../Debugger/Debugger.h"
#include "Engine/Engine.h"


namespace q_engine
{
	void gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, [[maybe_unused]] GLsizei length, const GLchar* message, const void*)
	{
		std::stringstream sstr;
		sstr << "Debug Message in OpenGL: ";
		switch (source)
		{
			case GL_DEBUG_SOURCE_API:
				sstr << "API";
				break;

			case GL_DEBUG_SOURCE_APPLICATION:
				sstr << "APPLICATION";
				break;

			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				sstr << "SHADER";
				break;

			case GL_DEBUG_SOURCE_THIRD_PARTY:
				sstr << "THIRD_PARTY";
				break;

			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				sstr << "WINDOW_SYSTEM";
				break;

			case GL_DEBUG_SOURCE_OTHER:
				sstr << "OTHER";
				break;

			default:
				sstr << "UNKNOWN";
				break;
		}
		sstr << '\t';
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:
				sstr << "ERROR";
				break;

			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				sstr << "DEPRECATED";
				break;

			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				sstr << "UNDEFINED";
				break;

			case GL_DEBUG_TYPE_PORTABILITY:
				sstr << "PORTABILITY";
				break;

			case GL_DEBUG_TYPE_PERFORMANCE:
				sstr << "PERFORMANCE";
				break;

			case GL_DEBUG_TYPE_OTHER:
				sstr << "OTHER";
				break;

			case GL_DEBUG_TYPE_MARKER:
				sstr << "MARKER";
				break;

			default:
				sstr << "UNKNOWN";
				break;
		}
		sstr << '\t' << id << '\t';
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				sstr << "NOTIFICATION";
				break;

			case GL_DEBUG_SEVERITY_HIGH:
				sstr << "HIGH";
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
				sstr << "MEDIUM";
				break;

			case GL_DEBUG_SEVERITY_LOW:
				sstr << "LOW";
				break;

			default:
				sstr << "UNKNOWN";
				break;
		}
		sstr << '\t' << message << '\n';
	}
	

	
	void create_window(int width, int height, const std::string& title, const EWindowMode windowMode)
	{
		using namespace q_impl;

		if (p_window != nullptr)
		{
			Debugger::GetLogger().AddMessage("Call to create_window while there already is. Doing nothing", SeverityLevel::NOTICE);
			return;
		}
		if (!glfwInit())
		{		
			Debugger::GetLogger().AddMessage("Failed to initialize glfw", SeverityLevel::ERROR);
			std::exit(EXIT_FAILURE);
		}
		std::atexit([]() { glfwTerminate(); });

		glfwSetErrorCallback([](int error, const char* desc) 
			{
				Debugger::GetLogger().AddMessage("Error in glfw, " + std::to_string(error) + ": " + std::string(desc), SeverityLevel::ERROR);
			}
		);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif

		GLFWmonitor* const monitor = glfwGetPrimaryMonitor();
		p_default_video_mode = glfwGetVideoMode(monitor);

		if (width <= 0)
		{
			width = p_default_video_mode->width;
		}
		if (height <= 0)
		{
			height = p_default_video_mode->height;
		}

		cache_window_width = width;
		cache_window_height = height;
		
		p_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		set_window_mode(windowMode);
		if (p_window == nullptr)
		{
			Debugger::GetLogger().AddMessage("Failed to create a window", SeverityLevel::ERROR);
			std::exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(p_window);

		glfwSetInputMode(p_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		set_cursor_visible(false);
		glfwSetCursorPosCallback(p_window, [](GLFWwindow*, double x, double y) { if (mouse_moved_callback) mouse_moved_callback(x, y); });
		glfwSetMouseButtonCallback(p_window, [](GLFWwindow*, int button, int action, int mod) { if (mouse_button_callback) mouse_button_callback(button, action, mod); });
		glfwSetScrollCallback(p_window, [](GLFWwindow*, double xAmount, double yAmount) { if (mouse_wheel_callback) mouse_wheel_callback(xAmount, yAmount); });
		glfwSetKeyCallback(p_window, [](GLFWwindow*, int key, int scanCode, int action, int mod) { if (key_button_callback) key_button_callback(key, scanCode, action, mod); });
		glfwSetWindowFocusCallback(p_window, [](GLFWwindow*, int focused)
		{
			bool isFocused = static_cast<bool>(focused);
			Engine::SetWindowFocus(isFocused);

		});

		if (glewInit() != GLEW_OK)
		{
			Debugger::GetLogger().AddMessage("Failed to initialize glew", SeverityLevel::ERROR);
			std::exit(EXIT_FAILURE);
		}

	//#ifdef _DEBUG
	//	if (!GLEW_VERSION_4_5)
	//	{
	//		Debugger::GetLogger().AddMessage("Hardware doesn't support OpenGL version 4.5", SeverityLevel::ERROR);
	//		std::exit(EXIT_FAILURE);
	//	}
	//#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

	#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageCallback(gl_debug_callback, nullptr);


	#endif

		monitor_size_width = p_default_video_mode->width;
		monitor_size_height = p_default_video_mode->height;
	}


	void update_window()
	{
		glfwPollEvents();
	}


	bool is_window_closed()
	{
		return glfwWindowShouldClose(q_impl::p_window);
	}


	void close_window()
	{
		glfwSetWindowShouldClose(q_impl::p_window, true);
	}


	EWindowMode get_window_mode()
	{
		return q_impl::window_mode;
	}


	void set_window_mode(const EWindowMode windowMode)
	{
		using namespace q_impl;

		GLFWmonitor* const monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* const videoMode = window_mode == EWindowMode::FULLSCREEN ? p_default_video_mode : glfwGetVideoMode(monitor);

		const int fullscreenWidth = videoMode->width;
		const int fullscreenHeight = videoMode->height;

		
		switch (windowMode)
		{
			case EWindowMode::WINDOWED:
				glfwSetWindowAttrib(p_window, GLFW_DECORATED, GLFW_TRUE);
				glfwSetWindowMonitor(p_window, nullptr, 0, 0, cache_window_width, cache_window_height, GLFW_DONT_CARE);
				center_window_position();
				break;
			
			case EWindowMode::BORDERLESS_WINDOW:
			{
				auto [width, height] = get_window_size();
				cache_window_width = width;
				cache_window_height = height;
				glfwSetWindowAttrib(p_window, GLFW_DECORATED, GLFW_FALSE);
				glfwSetWindowMonitor(p_window, nullptr, 0, 0, fullscreenWidth, fullscreenHeight, GLFW_DONT_CARE);
				break;
			}
			
			case EWindowMode::FULLSCREEN:
			{
				auto [width, height] = get_window_size();
				cache_window_width = width;
				cache_window_height = height;
				glfwSetWindowMonitor(p_window, monitor, 0, 0, fullscreenWidth, fullscreenHeight, GLFW_DONT_CARE);
				break;
			}
		}
	}


	bool is_cursor_visible()
	{
		return glfwGetInputMode(q_impl::p_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
	}


	void set_cursor_visible(const bool bVisible)
	{
		glfwSetInputMode(q_impl::p_window, GLFW_CURSOR, bVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
	}


	void toggle_cursor_visibility()
	{
		set_cursor_visible(!is_cursor_visible());
	}


	std::pair<double, double> get_mouse_position()
	{
		double x, y;
		glfwGetCursorPos(q_impl::p_window, &x, &y);

		return { x, y };
	}


	void set_mouse_position(const double x, const double y)
	{
		glfwSetCursorPos(q_impl::p_window, x, y);
	}


	bool is_window_resizable()
	{
		return glfwGetWindowAttrib(q_impl::p_window, GLFW_RESIZABLE);
	}


	void set_window_resizable(const bool bResizable)
	{
		glfwSetWindowAttrib(q_impl::p_window, GLFW_RESIZABLE, bResizable);
	}


	void toggle_window_resizability()
	{
		set_window_resizable(!is_window_resizable());
	}


	std::pair<int, int> get_window_size()
	{
		using namespace q_impl;
		return { cache_window_width, cache_window_height };
	}


	void set_window_size(const int width, const int height)
	{
		glfwSetWindowSize(q_impl::p_window, width, height);
	}


	std::pair<int, int> get_window_position()
	{
		int x, y;
		glfwGetWindowSize(q_impl::p_window, &x, &y);

		return { x, y };
	}


	void set_window_position(const int posX, const int posY)
	{
		glfwSetWindowPos(q_impl::p_window, posX, posY);
	}


	void center_window_position()
	{
		using namespace q_impl;
		
		const GLFWvidmode* const videoMode = window_mode == EWindowMode::FULLSCREEN ? p_default_video_mode : glfwGetVideoMode(glfwGetPrimaryMonitor());

		const int fullscreenWidth = videoMode->width;
		const int fullscreenHeight = videoMode->height;

		auto [width, height] = get_window_size();

		constexpr int TITLE_BAR_HEIGHT = 32;
		set_window_position((fullscreenWidth - width) / 2, (fullscreenHeight - height) / 2 + TITLE_BAR_HEIGHT);
	}


	void set_window_title(const std::string& title)
	{
		glfwSetWindowTitle(q_impl::p_window, title.c_str());
	}


	void set_window_icon([[maybe_unused]] const WindowImage& icon)
	{
	
		// https://www.glfw.org/docs/3.3/window_guide.html#window_icon
	}


	void reset_window_icon()
	{
		glfwSetWindowIcon(q_impl::p_window, 0, nullptr);
	}


	void set_window_cursor([[maybe_unused]] const WindowImage& cursor)
	{
		
		// https://www.glfw.org/docs/3.3/input_guide.html#cursor_custom
	}


	void reset_window_cursor()
	{
		glfwSetCursor(q_impl::p_window, nullptr);
	}


	void request_window_attention()
	{
		glfwRequestWindowAttention(q_impl::p_window);
	}


	void set_mouse_moved_callback(std::function<void(double, double)>&& callback)
	{
		q_impl::mouse_moved_callback = std::move(callback);
	}


	void set_mouse_button_callback(std::function<void(int, int, int)>&& callback)
	{
		q_impl::mouse_button_callback = std::move(callback);
	}


	void set_mouse_wheel_callback(std::function<void(double, double)>&& callback)
	{
		q_impl::mouse_wheel_callback = std::move(callback);
	}


	void set_key_button_callback(std::function<void(int, int, int, int)>&& callback)
	{
		q_impl::key_button_callback = std::move(callback);
	}


	void set_window_resized_callback(std::function<void(int newWidth, int newHeight)>&& callback)
	{
		q_impl::window_resized_callback = std::move(callback);
	}
} // namespace q_engine
