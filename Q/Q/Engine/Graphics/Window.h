/*
    File Name: Window.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <functional>
#include <string>

struct GLFWwindow;
struct GLFWvidmode;

namespace q_engine
{
	enum class [[nodiscard]] EWindowMode
	{
		WINDOWED,
		BORDERLESS_WINDOW,
		FULLSCREEN
	};

	class [[nodiscard]] WindowImage
	{
	
	};
	
    void create_window(int width = 0, int height = 0, const std::string& title = "", EWindowMode windowMode = EWindowMode::FULLSCREEN);
	
    void update_window();

    [[nodiscard]] bool is_window_closed();
    void close_window();

    EWindowMode get_window_mode();
    void set_window_mode(EWindowMode windowMode);
	
    [[nodiscard]] bool is_cursor_visible();
    void set_cursor_visible(bool bVisible = true);
    void toggle_cursor_visibility();

    [[nodiscard]] std::pair<double, double> get_mouse_position();
    void set_mouse_position(double x, double y);

    [[nodiscard]] bool is_window_resizable();
    void set_window_resizable(bool bResizable = true);
    void toggle_window_resizability();

    [[nodiscard]] std::pair<int, int> get_window_size();
    void set_window_size(int width, int height);

    [[nodiscard]] std::pair<int, int> get_window_position();
    void set_window_position(int posX, int posY);
    void center_window_position();

    void set_window_title(const std::string& title);

    void set_window_icon(const WindowImage& icon);
    void reset_window_icon();

    void set_window_cursor(const WindowImage& cursor);
    void reset_window_cursor();

    void request_window_attention();

    void set_mouse_moved_callback(std::function<void(double x, double y)>&& callback = nullptr);
    void set_mouse_button_callback(std::function<void(int button, int action, int mod)>&& callback = nullptr);
    void set_mouse_wheel_callback(std::function<void(double xAmount, double yAmount)>&& callback = nullptr);
    void set_key_button_callback(std::function<void(int key, int scanCode, int action, int mode)>&& callback = nullptr);
    void set_window_resized_callback(std::function<void(int newWidth, int newHeight)>&& callback = nullptr);


    namespace q_impl
    {
        inline GLFWwindow* p_window;
        inline const GLFWvidmode* p_default_video_mode;
        inline EWindowMode window_mode;
        inline int cache_window_width;
        inline int cache_window_height;
        inline int monitor_size_width;
        inline int monitor_size_height;

        inline std::function<void(double, double)> mouse_moved_callback = nullptr;
        inline std::function<void(int, int, int)> mouse_button_callback = nullptr;
        inline std::function<void(double, double)> mouse_wheel_callback = nullptr;
        inline std::function<void(int, int, int, int)> key_button_callback = nullptr;
        inline std::function<void(int, int)> window_resized_callback = nullptr;

        inline double FPS = 0.f;
    }
} // namespace q_engine
