/*
	File Name: IDGenerator.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung, Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/Types.h"

namespace q_engine
{
	namespace internal
	{
		struct type_id final
		{
			[[nodiscard]] static IDType next()
			{
				static int value{};
				return value++;
			}
		};

		struct entt_type_id final
		{
			[[nodiscard]] static IDType next()
			{
				static int value{};
				return value++;
			}
		};
	}

	template<typename Type>
	struct TypeIDGenerator final
	{
		[[nodiscard]] static IDType GetValue()
		{
			static const IDType value = internal::type_id::next();
			return value;
		}
	};


	class IDGenerator
	{
		inline static IDType lastTypeID;
		inline static IDType lastEntityID;
	public:
		inline static IDType GenerateTypeID();
		inline static IDType GenerateEntityID();
	};
	
	inline IDType IDGenerator::GenerateTypeID()
	{
		++lastTypeID;
		return lastTypeID;
	}
	

	inline IDType IDGenerator::GenerateEntityID()
	{
		++lastEntityID;
		return lastEntityID;
	}

}
