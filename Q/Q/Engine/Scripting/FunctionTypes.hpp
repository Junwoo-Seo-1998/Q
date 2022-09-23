/*
	File Name: FunctionTypes.hpp
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary:
	All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <tuple>



namespace function_types
{
template<typename>
struct FunctionTypes;

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_ (Class_::*) (Parameters_...)>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) volatile>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const volatile>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) &>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = true;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) &&>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = true;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const &>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = true;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const &&>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = true;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) volatile &>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = true;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) volatile &&>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = true;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const volatile &>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = true;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const volatile &&>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = true;
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) volatile noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const volatile noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) & noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = true;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) && noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = true;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const & noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = true;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const && noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = false;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = true;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) volatile & noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = true;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) volatile && noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = false;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = true;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const volatile & noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = true;
	static constexpr bool isRRef = false;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};

template<typename ReturnType_, typename Class_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(Class_::*) (Parameters_...) const volatile && noexcept>
{
	using ReturnType = ReturnType_;
	using Class = Class_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isConst = true;
	static constexpr bool isVolatile = true;
	static constexpr bool isLRef = false;
	static constexpr bool isRRef = true;
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = true;
};


template<typename ReturnType_, typename ...Parameters_>
struct FunctionTypes<ReturnType_(*) (Parameters_...)>
{
	using ReturnType = ReturnType_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isNoExcept = false;
	static constexpr bool isMemberFunction = false;
};

template<typename ReturnType_, typename ...Parameters_>
struct FunctionTypes<ReturnType_ (*) (Parameters_...) noexcept>
{
	using ReturnType = ReturnType_;
	using Parameters = std::tuple<Parameters_...>;
	static constexpr size_t parameterCount = sizeof...(Parameters_);
	static constexpr bool isNoExcept = true;
	static constexpr bool isMemberFunction = false;
};
}
