/*
    File Name: Mesh.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../Debugger/Debugger.h"


namespace q_engine
{
	Mesh::Mesh(const std::filesystem::path& meshFilePath)
	{
		std::ifstream meshFile{ meshFilePath };
		if (!meshFile)
		{
			Debugger::GetLogger().AddMessage("Failed to open file " + meshFilePath.string(), SeverityLevel::ERROR);
		#ifdef _DEBUG
            std::exit(EXIT_FAILURE);
		#else
			return;
		#endif
		}

		unsigned int vertexCount, indexCount;
		std::string primitiveType;
		meshFile >> vertexCount >> indexCount >> primitiveType;
		
		
		using VertexPositionType = glm::vec2;
		using VertexUVType = glm::vec2;

		using VertexIndexType = GLushort;

		
		std::vector<VertexPositionType> vertexPositions(vertexCount, { 0, 0 });
		std::vector<VertexUVType> vertexUVs(vertexCount, { 0, 0 });

		std::vector<VertexIndexType> vertexIndices(indexCount, 0);


		if (primitiveType == "points")
		{
			mPrimitiveType = EPrimitiveType::POINTS;
		}
		else if (primitiveType == "lines")
		{
			mPrimitiveType = EPrimitiveType::LINES;
		}
		else if (primitiveType == "line_strip")
		{
			mPrimitiveType = EPrimitiveType::LINE_STRIP;
		}
		else if (primitiveType == "line_loop")
		{
			mPrimitiveType = EPrimitiveType::LINE_LOOP;
		}
		else if (primitiveType == "triangles")
		{
			mPrimitiveType = EPrimitiveType::TRIANGLES;
		}
		else if (primitiveType == "triangle_strip")
		{
			mPrimitiveType = EPrimitiveType::TRIANGLE_STRIP;
		}
		else if (primitiveType == "triangle_fan")
		{
			mPrimitiveType = EPrimitiveType::TRIANGLE_FAN;
		}
		else
		{
			Debugger::GetLogger().AddMessage("Unknown primitive type \"" + primitiveType + "\" in mesh file " + meshFilePath.string(), SeverityLevel::WARNING);
			return;
		}


		char mode;
		std::string line;
		unsigned int posIndex = 0, uvIndex = 0, indexIndex = 0;
		
		while (std::getline(meshFile, line))
		{
			if (line.empty())
			{
				continue;
			}
			std::stringstream sstr{ line };
			sstr >> mode;
			switch (mode)
			{
				case 'v':
					sstr >> vertexPositions[posIndex].x >> vertexPositions[posIndex].y;
					posIndex++;
					break;

				case 'u':
					sstr >> vertexUVs[uvIndex].x >> vertexUVs[uvIndex].y;
					uvIndex++;
					break;

				case 'i':
					sstr >> vertexIndices[indexIndex++];
					break;

				default:
					Debugger::GetLogger().AddMessage("Unknown mode \"" + std::to_string(mode) + "\" in mesh file " + meshFilePath.string(), SeverityLevel::WARNING);
					break;
			}
			mode = ' ';
		}

		
		VBOHandle vboHandle;
		VAOHandle vaoHandle;
		EBOHandle eboHandle;
		
		constexpr size_t vertexPositionSize = sizeof(VertexPositionType);
		constexpr size_t vertexUVSize = sizeof(VertexUVType);
		
		const size_t vertexPositionsMemorySize = vertexPositions.size() * vertexPositionSize;
		const size_t vertexUVsMemorySize = vertexUVs.size() * vertexUVSize;

		const size_t vertexIndicesMemorySize = vertexIndices.size() * sizeof(VertexIndexType);

		constexpr GLuint positionAttributeIndex = 0;
		constexpr GLuint uvAttributeIndex = 2;

		
		glCreateBuffers(1, &vboHandle);
		glNamedBufferStorage(vboHandle, vertexPositionsMemorySize + vertexUVsMemorySize, vertexPositions.data(), GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(vboHandle, vertexPositionsMemorySize, vertexUVsMemorySize, vertexUVs.data());

		
		glCreateVertexArrays(1, &vaoHandle);
		
		glEnableVertexArrayAttrib(vaoHandle, positionAttributeIndex);
		glVertexArrayVertexBuffer(vaoHandle, positionAttributeIndex, vboHandle, 0, vertexPositionSize);
		glVertexArrayAttribFormat(vaoHandle, positionAttributeIndex, VertexPositionType::length(), GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vaoHandle, positionAttributeIndex, positionAttributeIndex);

		glEnableVertexArrayAttrib(vaoHandle, uvAttributeIndex);
		glVertexArrayVertexBuffer(vaoHandle, uvAttributeIndex, vboHandle, vertexPositionsMemorySize, vertexUVSize);
		glVertexArrayAttribFormat(vaoHandle, uvAttributeIndex, VertexUVType::length(), GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vaoHandle, uvAttributeIndex, uvAttributeIndex);


		glCreateBuffers(1, &eboHandle);
		glNamedBufferStorage(eboHandle, vertexIndicesMemorySize, vertexIndices.data(), GL_DYNAMIC_STORAGE_BIT);

		glVertexArrayElementBuffer(vaoHandle, eboHandle);


		glBindVertexArray(0);


		mVAOHandle = vaoHandle;
		mVBOHandle = vboHandle;
		mEBOHandle = eboHandle;

		mDrawCount = unsigned(vertexIndices.size());
	}


	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &mVAOHandle);
		glDeleteBuffers(1, &mVBOHandle);
		glDeleteBuffers(1, &mEBOHandle);
	}
} // namespace q_engine
