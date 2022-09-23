/*
    File Name: ImGuiHelper.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <gsl\pointers>         //gsl::not_null<T>

class ImGuiHelper
{
public:
    ImGuiHelper() = default;
    ~ImGuiHelper() = default;

    void Setup(gsl::not_null<GLFWwindow*> p_window);

    void StartRender();
    void EndRender();

    void CleanUp();
private:
    ImGuiIO io;
};

class ImGuiRenderable
{
public:
    virtual void ImGuiRender(bool& is_open) = 0;
};