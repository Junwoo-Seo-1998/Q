/*
    File Name: StringHash.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <string>

#include <xxhash_cx/xxhash_cx.h>
#include <Engine/Types.h>

namespace q_engine
{
	constexpr IDType get_hash_id(const std::string& str)
	{
        return xxhash::xxh32(str.c_str(), str.size(), XXHASH_CX_XXH32_SEED);
	}

	constexpr IDType operator""_hashID(const char* str, const size_t size)
	{
        return xxhash::xxh32(str, size, XXHASH_CX_XXH32_SEED);
	}
}
