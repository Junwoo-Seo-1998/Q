project "lua"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    disablewarnings { "26451","6385","6386", "4006" }

    flags {"MultiProcessorCompile" }

    targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
    objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    local lua_src  = 'src/'

    files
    {
		lua_src .. '*.h',
		lua_src .. '*.c'
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"