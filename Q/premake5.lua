-- writer : junwoo.seo


workspace "Q"
    architecture "x64"
    startproject "Q"
    
    configurations
    {
        "Debug",
        "Release"
    }

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- include dir is relative with root folder!
IncludeDir={}
IncludeDir["Q"]="Q"
IncludeDir["external"]="external/include"
IncludeDir["game"]="Q/Game"
IncludeDir["ImGui"]="external/include/imgui"
IncludeDir["ImGui_Backends"]="external/include/imgui/backends"
IncludeDir["lua"] = "external/lua/src/"
group "Dependencies"
    include "external/include/imgui"
    include "external/lua"
group ""


ToCopy={}
--ToCopy["python_bin"]="%{wks.location}/external/bin"
--ToCopy["resource"]="%{wks.location}/Q/resource/*.*"

SolutionDir="%{wks.location}"

project "Q"
    location "Q"
    kind "consoleApp"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"

    linkoptions { "-IGNORE:4006" }
    disablewarnings { "4701","4006","4702","4244","4067","4312","4006","4267","4018", "26495",
    "26451","26439", "26815", "4100", "4245", "4100", "4201", "4389", "4189", "4239", "4457", "4456", "4459", "4458"}

    warnings "Extra"

    flags {"MultiProcessorCompile" }

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir("bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.inl"
    }
    removefiles 
    {   
        "Q/Editor/**.**"
    }
    includedirs
    {
        "%{IncludeDir.Q}",
        "%{IncludeDir.external}",
        "%{IncludeDir.game}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGui_Backends}",
        "%{IncludeDir.lua}"

    }

    links
    {
        "ImGui",
        "lua"
    }

    libdirs {"external/lib"}

    postbuildcommands
    {

    }

    buildoptions  
    {
        "/bigobj"
    }

    filter "system:windows"
        
        systemversion "latest"
        debugdir "$(TargetDir)"

        files { 'Q/Q.rc', '**.ico' }
        vpaths { ['Resources/*'] = { '*.rc', '**.ico' } }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        ignoredefaultlibraries { "MSVCRT.lib" }
        links
        {
            "fmod_vc.lib",
            "freetype.lib",
            "opengl32.lib",
            "glew32.lib",
            "glfw3dll.lib"
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        links
        {
            "fmod_vc.lib",
            "freetype.lib",
            "opengl32.lib",
            "glew32.lib",
            "glfw3dll.lib",
        }



project "Q-Editor"
    location "Q"
    kind "consoleApp"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"
    linkoptions { "-IGNORE:4006" }
    disablewarnings { "4701","4702","4244","4067","4312","4006","4267","4018", "26495",
    "26451","26439"}

    warnings "Extra"

    flags {"MultiProcessorCompile" }

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir("bin-int/"..outputdir.."/%{prj.name}")
    defines 
    {
        "QEDITOR"
    }
    files
    {
        "Q/**.h",
        "Q/**.cpp",
        "Q/**.inl"
    }
    removefiles 
    { 
        "Q/main.cpp",
        "Q/Engine/Debugger/EntityPhysicsDebugger.h",
        "Q/Engine/Debugger/EntityPhysicsDebugger.cpp",
        "Q/Engine/Event/**.**",
        "Q/Engine/System/**.**",
        "Q/Engine/GameStateManager.h",
        "Q/Engine/GameStateManager.cpp",
        "Q/Game/**.**",
        "Q/Levels/**.**",
    }
    files
    {
        "Q/Game/Component/EnemyStateComponent.**",
        "Q/Game/Component/HealthComponent.**",
        "Q/Engine/GameStateManager.**",
        "Q/Engine/System/**.**",
        "Q/Engine/Event/**.**",
        "Q/Engine/System/TextDrawSystem.**",
        "Q/Engine/System/TextureBoxUpdateSystem.**",
        "Q/Engine/System/AnimationUpdateDrawSystem.**",
        "Q/Engine/System/TimerManagerSystem.**",
        "Q/Game/JsonParser/**.**",
        "Q/Game/Entity/**.**",
        "Q/Game/Component/**.**",
        "Q/Game/System/**.**",
        "Q/Levels/**.**",
    }
    includedirs
    {
        "%{IncludeDir.Q}",
        "%{IncludeDir.external}",
        "%{IncludeDir.game}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGui_Backends}",
        "%{IncludeDir.lua}"

    }

    links
    {
        "ImGui",
        "lua"
    }

    libdirs {"external/lib"}

    postbuildcommands
    {

    }

    buildoptions  
    {
        "/bigobj"
    }
    
    filter "system:windows"
        
        systemversion "latest"
        debugdir "$(TargetDir)"

        files { 'Q/Q.rc', '**.ico' }
        vpaths { ['Resources/*'] = { '*.rc', '**.ico' } }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        ignoredefaultlibraries { "MSVCRT.lib" }
        links
        {
            "fmod_vc.lib",
            "freetype.lib",
            "opengl32.lib",
            "glew32.lib",
            "glfw3dll.lib"
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        links
        {
            "fmod_vc.lib",
            "freetype.lib",
            "opengl32.lib",
            "glew32.lib",
            "glfw3dll.lib",
        }