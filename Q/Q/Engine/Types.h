/*
	File Name: Types.h
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <cstdint>
#include <limits>

namespace q_engine
{
	using IDType = unsigned int;
	using AnimationID = IDType;
	using MeshID = IDType;
	using FontID = IDType;
	using TextureID = IDType;
	using TimerID = IDType;
	using ShaderProgramID = IDType;
	using ShaderID = IDType;
	using ScriptID = IDType;
	constexpr IDType INVALID_ID = std::numeric_limits<IDType>::max();
}

