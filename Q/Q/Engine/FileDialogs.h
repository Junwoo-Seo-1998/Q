/*
	File Name: FileDialogs.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <string>

namespace q_engine
{
	class FileDialogs
	{
	public:
		//these will return empty strings when they fail.
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}
