/*
    File Name: Mesh.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <filesystem>

#include "Engine/Types.h"


namespace q_engine
{
    enum class [[nodiscard]] EPrimitiveType : unsigned int
    {
        POINTS = 0,  //GL_POINTS
        LINES = 1,  //GL_LINES
        LINE_STRIP = 3, //GL_LINE_STRIP
        LINE_LOOP = 2,  //GL_LINE_LOOP
        TRIANGLES = 4,  //GL_TRIANGLES
        TRIANGLE_STRIP = 5,  //GL_TRIANGLE_STRIP
        TRIANGLE_FAN = 6,  //GL_TRIANGLE_FAN
    };
	
    class [[nodiscard]] Mesh
    {
        using VAOHandle = unsigned;
        using VBOHandle = unsigned;
        using EBOHandle = unsigned;

        friend class AssetManager;
    public:
        Mesh(const std::filesystem::path& meshFilePath);
        ~Mesh();

        Mesh(const Mesh& other) = delete;
        Mesh(Mesh&& other) noexcept = default;
        Mesh& operator=(const Mesh& other) = delete;
        Mesh& operator=(Mesh && other) noexcept = default;
    	
        [[nodiscard]] constexpr VAOHandle GetVAOHandle() const noexcept { return mVAOHandle; }
        [[nodiscard]] constexpr unsigned int GetDrawCount() const noexcept { return mDrawCount; }
        constexpr EPrimitiveType GetPrimitiveType() const noexcept { return mPrimitiveType; }

        [[nodiscard]] constexpr MeshID GetMeshID() const noexcept { return mMeshID; }


    	
    private:
        VAOHandle mVAOHandle;
        VBOHandle mVBOHandle;
        EBOHandle mEBOHandle;

        unsigned int mDrawCount;
        EPrimitiveType mPrimitiveType;

        MeshID mMeshID = INVALID_ID;
    };
} // namespace q_engine
