/*
	File Name: CppPythonEmbedder.hpp
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary:
	All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
// This should be included before any std header
#define PY_SSIZE_T_CLEAN
#include <python/Python.h>
#include <python/structmember.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/seq/reverse.hpp>

#include <xxhash_cx/xxhash_cx.h>

#include "FunctionTypes.hpp"


using xxhash::literals::operator ""_xxh64;


#define _PY_EXPORTER_FIELD(T, fieldName) { #fieldName, cpp_python_embedder::get_member_type_number<decltype(T::##fieldName)>(), offsetof(T, fieldName), 0, nullptr },
#define _PY_EXPORTER_FIELD_EXPANDER(_, T, fieldName) _PY_EXPORTER_FIELD(T, fieldName)
#define _PY_EXPORTER_FIELDS(T, seq) BOOST_PP_SEQ_FOR_EACH(_PY_EXPORTER_FIELD_EXPANDER, T, seq) { nullptr, 0, 0, 0, nullptr }

#define _PY_EXPORTER_ENUMERATOR(E, enumerator) { #enumerator, E##::##enumerator },
#define _PY_EXPORTER_ENUMERATOR_EXPANDER(_, E, enumerator) _PY_EXPORTER_ENUMERATOR(E, enumerator)
#define _PY_EXPORTER_ENUMERATORS(E, seq) BOOST_PP_SEQ_FOR_EACH(_PY_EXPORTER_ENUMERATOR_EXPANDER, E, seq) { nullptr, static_cast<E>(0) }

#define _PY_EXPORTER_FIELD_TYPE(T, fieldName) decltype(T::fieldName),
#define _PY_EXPORTER_FIELD_TYPE_EXPANDER(_, T, fieldName) _PY_EXPORTER_FIELD_TYPE(T, fieldName)
#define _PY_EXPORTER_HEADS_FIELD_TYPES(T, seq) BOOST_PP_SEQ_FOR_EACH(_PY_EXPORTER_FIELD_TYPE_EXPANDER, T, seq)
#define _PY_EXPORTER_FIELD_TYPES(T, seq) _PY_EXPORTER_HEADS_FIELD_TYPES(T, BOOST_PP_SEQ_POP_BACK(seq)) decltype(T::BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(seq)))

#define _PY_EXPORTER_FIELD_OFFSET(T, fieldName) offsetof(T, fieldName),
#define _PY_EXPORTER_FIELD_OFFSET_EXPANDER(_, T, fieldName) _PY_EXPORTER_FIELD_OFFSET(T, fieldName)
#define _PY_EXPORTER_HEADS_FIELD_OFFSETS(T, seq) BOOST_PP_SEQ_FOR_EACH(_PY_EXPORTER_FIELD_OFFSET_EXPANDER, T, seq)
#define _PY_EXPORTER_FIELD_OFFSETS(T, seq) _PY_EXPORTER_HEADS_FIELD_OFFSETS(T, BOOST_PP_SEQ_POP_BACK(seq)) offsetof(T, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(seq)))

#define _PY_EXPORTER_REMOVE_PARENTHESIS_EXPAND_HELPER(...) __VA_ARGS__
#define _PY_EXPORTER_REMOVE_PARENTHESIS_HELPER(x) x
#define _PY_EXPORTER_REMOVE_PARENTHESIS(x) _PY_EXPORTER_REMOVE_PARENTHESIS_HELPER(_PY_EXPORTER_REMOVE_PARENTHESIS_EXPAND_HELPER x)

#define _PY_EXPORTER_STRINGIFY_HELPER(...) #__VA_ARGS__
#define _PY_EXPORTER_STRINGIFY(x) _PY_EXPORTER_REMOVE_PARENTHESIS_HELPER(_PY_EXPORTER_STRINGIFY_HELPER x)

#define _PY_EXPORTER_TEMPLATE_INSTANCE(func, templateParam) cpp_python_embedder::TemplateInstanceInfo<decltype(&(##func<_PY_EXPORTER_REMOVE_PARENTHESIS(templateParam)>)), &(##func<_PY_EXPORTER_REMOVE_PARENTHESIS(templateParam)>), _PY_EXPORTER_STRINGIFY(templateParam)##_xxh64>,
#define _PY_EXPORTER_TEMPLATE_INSTANCE_EXPANDER(_, func, templateParam) _PY_EXPORTER_TEMPLATE_INSTANCE(func, templateParam)
#define _PY_EXPORTER_HEADS_TEMPLATE_SPECIALIZES(func, seq) BOOST_PP_SEQ_FOR_EACH(_PY_EXPORTER_TEMPLATE_INSTANCE_EXPANDER, func, seq)
#define _PY_EXPORTER_TEMPLATE_INSTANCES(func, seq) _PY_EXPORTER_HEADS_TEMPLATE_SPECIALIZES(func, BOOST_PP_SEQ_POP_BACK(seq)) cpp_python_embedder::TemplateInstanceInfo<decltype(&(##func<_PY_EXPORTER_REMOVE_PARENTHESIS(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(seq)))>)), &(##func<_PY_EXPORTER_REMOVE_PARENTHESIS(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(seq)))>), _PY_EXPORTER_STRINGIFY(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(seq)))##_xxh64>





#define PY_EXPORT_GLOBAL_FUNCTION_PTR(funcPtr, funcName, moduleName) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterFunction<decltype(funcPtr), funcPtr>(#funcName)
#define PY_EXPORT_STATIC_FUNCTION_PTR(funcPtr, funcName, moduleName) PY_EXPORT_GLOBAL_FUNCTION_PTR(funcPtr, funcName, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION_PTR_AS_STATIC_FUNCTION_PTR(funcPtr, funcName, instanceReturnerPtr, moduleName) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterMemberFunctionAsStaticFunction<decltype(funcPtr), funcPtr, decltype(instanceReturnerPtr), instanceReturnerPtr>(#funcName)
#define PY_EXPORT_MEMBER_FUNCTION_PTR_AS_STATIC_FUNCTION(funcPtr, funcName, instanceReturner, moduleName) PY_EXPORT_MEMBER_FUNCTION_PTR_AS_STATIC_FUNCTION_PTR(funcPtr, funcName, &##instanceReturner, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION_PTR(func, funcName, instanceReturnerPtr, moduleName) PY_EXPORT_MEMBER_FUNCTION_PTR_AS_STATIC_FUNCTION_PTR(&##func, funcName, instanceReturnerPtr, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION_PTR_AS_STATIC_FUNCTION_LAMBDA(funcPtr, funcName, instanceReturner, moduleName) static auto T##funcName##moduleName##lambda = instanceReturner; \
	cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterMemberFunctionAsStaticFunctionLambda<decltype(funcPtr), funcPtr, decltype(&decltype(T##funcName##moduleName##lambda##)::operator()), &decltype(T##funcName##moduleName##lambda##)::operator(), decltype(&##T##funcName##moduleName##lambda), &##T##funcName##moduleName##lambda>(#funcName)
#define PY_EXPORT_MEMBER_FUNCTION_PTR(funcPtr, funcName, moduleName) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterMemberFunction<decltype(funcPtr), funcPtr>(#funcName)


#define PY_EXPORT_GLOBAL_FUNCTION_NAME(func, funcName, moduleName) PY_EXPORT_GLOBAL_FUNCTION_PTR(&##func, funcName, moduleName)
#define PY_EXPORT_STATIC_FUNCTION_NAME(T, func, funcName, moduleName) PY_EXPORT_GLOBAL_FUNCTION_NAME(T##::##func, funcName, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION_NAME(T, func, funcName, instanceReturner, moduleName) PY_EXPORT_MEMBER_FUNCTION_PTR_AS_STATIC_FUNCTION(&##T##::##func, funcName, instanceReturner, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION_LAMBDA_NAME(T, func, funcName, instanceReturner, moduleName) PY_EXPORT_MEMBER_FUNCTION_PTR_AS_STATIC_FUNCTION_LAMBDA(&##T##::##func, funcName, instanceReturner, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION_NAME(T, func, funcName, moduleName) PY_EXPORT_MEMBER_FUNCTION_PTR(&##T##::##func, funcName, moduleName)

#define PY_EXPORT_TEMPLATE_GLOBAL_FUNCTION_NAME(func, funcName, moduleName, templateParamSeq) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterTemplateFunction<#funcName##_xxh64, _PY_EXPORTER_TEMPLATE_INSTANCES(func, templateParamSeq)>(#funcName)
#define PY_EXPORT_TEMPLATE_STATIC_FUNCTION_NAME(T, func, funcName, moduleName, templateParamSeq) PY_EXPORT_TEMPLATE_GLOBAL_FUNCTION_NAME(T##::##func, funcName, moduleName, templateParamSeq)
#define PY_EXPORT_TEMPLATE_MEMBER_FUNCTION_AS_STATIC_FUNCTION_NAME(T, func, funcName, instanceReturner, moduleName, templateParamSeq) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterTemplateMemberFunctionAsStaticFunction<#T###funcName##_xxh64, decltype(&##instanceReturner), &##instanceReturner, _PY_EXPORTER_TEMPLATE_INSTANCES(T##::##func, templateParamSeq)>(#funcName)
#define PY_EXPORT_TEMPLATE_MEMBER_FUNCTION_AS_STATIC_FUNCTION_LAMBDA_NAME(T, func, funcName, instanceReturner, moduleName, templateParamSeq) static auto T##funcName##moduleName##lambda = instanceReturner; \
	cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterTemplateMemberFunctionAsStaticFunctionLambda<#T###funcName##_xxh64, decltype(&decltype(T##funcName##moduleName##lambda##)::operator()), &decltype(T##funcName##moduleName##lambda##)::operator(), decltype(&##T##funcName##moduleName##lambda), &##T##funcName##moduleName##lambda, _PY_EXPORTER_TEMPLATE_INSTANCES(T##::##func, templateParamSeq)>(#funcName)
#define PY_EXPORT_TEMPLATE_MEMBER_FUNCTION_NAME(T, func, funcName, moduleName, templateParamSeq) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterTemplateMemberFunction<#T###funcName##_xxh64, _PY_EXPORTER_TEMPLATE_INSTANCES(T##::##func, templateParamSeq)>(#funcName)

#define PY_EXPORT_TYPE_NAME(T, typeName, moduleName, fieldSeq) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterType<T, std::integer_sequence<size_t, _PY_EXPORTER_FIELD_OFFSETS(T, fieldSeq)>, _PY_EXPORTER_FIELD_TYPES(T, fieldSeq)>(#typeName, { _PY_EXPORTER_FIELDS(T, fieldSeq) })
#define PY_EXPORT_TYPE_1FIELD_NAME(T, typeName, moduleName, field) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterType<T, std::integer_sequence<size_t, offsetof(T, field)>, decltype(T##::##field)>(#typeName, {{ #field, cpp_python_embedder::get_member_type_number<decltype(T::##field)>(), offsetof(T, field), 0, nullptr }, { nullptr, 0, 0, 0, nullptr }})
#define PY_EXPORT_TYPE_0FIELD_NAME(T, typeName, moduleName) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterType<T, std::integer_sequence<size_t>>(#typeName, {{ nullptr, 0, 0, 0, nullptr }})

#define PY_EXPORT_ENUM_NAME(E, enumName, moduleName, enumeratorSeq) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterEnum<E>(#enumName, { _PY_EXPORTER_ENUMERATORS(E, enumeratorSeq) })

#define PY_EXPORT_MODULE_NAME(moduleName, newName) cpp_python_embedder::Exporter<#moduleName##_xxh64>::Export(#newName)


#define PY_EXPORT_GLOBAL_FUNCTION(func, moduleName) PY_EXPORT_GLOBAL_FUNCTION_NAME(func, func, moduleName)
#define PY_EXPORT_STATIC_FUNCTION(T, func, moduleName) PY_EXPORT_STATIC_FUNCTION_NAME(T, func, func, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION(T, func, instanceReturner, moduleName) PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION_NAME(T, func, func, instanceReturner, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION_LAMBDA(T, func, instanceReturner, moduleName) PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION_LAMBDA_NAME(T, func, func, instanceReturner, moduleName)
#define PY_EXPORT_MEMBER_FUNCTION(T, func, moduleName) PY_EXPORT_MEMBER_FUNCTION_NAME(T, func, func, moduleName)

#define PY_EXPORT_GLOBAL_OPERATOR(func, operatorType, moduleName) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterGlobalOperator<decltype(&##func), &##func, operatorType>()
#define PY_EXPORT_MEMBER_OPERATOR(T, func, operatorType, moduleName) cpp_python_embedder::Exporter<#moduleName##_xxh64>::RegisterMemberOperator<decltype(&##T##::##func), &##T##::##func, T, operatorType>()

#define PY_EXPORT_TEMPLATE_GLOBAL_FUNCTION(func, moduleName, templateParamSeq) PY_EXPORT_TEMPLATE_GLOBAL_FUNCTION_NAME(func, func, moduleName, templateParamSeq)
#define PY_EXPORT_TEMPLATE_STATIC_FUNCTION(T, func, moduleName, templateParamSeq) PY_EXPORT_TEMPLATE_STATIC_FUNCTION_NAME(T, func, func, moduleName, templateParamSeq)
#define PY_EXPORT_TEMPLATE_MEMBER_FUNCTION_AS_STATIC_FUNCTION(T, func, instanceReturner, moduleName, templateParamSeq) PY_EXPORT_TEMPLATE_MEMBER_FUNCTION_AS_STATIC_FUNCTION_NAME(T, func, func, instanceReturner, moduleName, templateParamSeq)
#define PY_EXPORT_TEMPLATE_MEMBER_FUNCTION_AS_STATIC_FUNCTION_LAMBDA(T, func, instanceReturner, moduleName, templateParamSeq) PY_EXPORT_TEMPLATE_MEMBER_FUNCTION_AS_STATIC_FUNCTION_LAMBDA_NAME(T, func, func, instanceReturner, moduleName, templateParamSeq)
#define PY_EXPORT_TEMPLATE_MEMBER_FUNCTION(T, func, moduleName, templateParamSeq) PY_EXPORT_TEMPLATE_MEMBER_FUNCTION_NAME(T, func, func, moduleName, templateParamSeq)

#define PY_EXPORT_TYPE(T, moduleName, fieldSeq) PY_EXPORT_TYPE_NAME(T, T, moduleName, fieldSeq)
#define PY_EXPORT_TYPE_1FIELD(T, moduleName, field) PY_EXPORT_TYPE_1FIELD_NAME(T, T, moduleName, field)
#define PY_EXPORT_TYPE_0FIELD(T, moduleName) PY_EXPORT_TYPE_0FIELD_NAME(T, T, moduleName)

#define PY_EXPORT_ENUM(E, moduleName, enumeratorSeq) PY_EXPORT_ENUM_NAME(E, E, moduleName, enumeratorSeq)

#define PY_EXPORT_MODULE(moduleName) PY_EXPORT_MODULE_NAME(moduleName, moduleName)



namespace cpp_python_embedder
{
// TODO: Reduce macros as much as possible
// TODO: Support user-defined data types as field
// TODO: Support array as field (python list)
// TODO: Support more operator overloading https://docs.python.org/3/c-api/typeobj.html#number-object-structures
// TODO: Support template operator overloading (maybe?)

// TODO: Separate files and #include
// (INCREF, DECREF) https://docs.python.org/3/c-api/intro.html#objects-types-and-reference-counts
// http://edcjones.tripod.com/refcount.html


	
enum class [[nodiscard]] EOperatorType
{
	// operator +
	ADD = offsetof(PyNumberMethods, nb_add),
	// operator -
	SUBTRACT = offsetof(PyNumberMethods, nb_subtract),
	// operator *
	MULTIPLY = offsetof(PyNumberMethods, nb_multiply),
	// operator /
	DIVIDE = offsetof(PyNumberMethods, nb_true_divide),
	// operator %
	REMAINDER = offsetof(PyNumberMethods, nb_remainder),
	// operator +
	POSITIVE = offsetof(PyNumberMethods, nb_positive),
	// operator -
	NEGATIVE = offsetof(PyNumberMethods, nb_negative),
	// operator ^
	XOR = offsetof(PyNumberMethods, nb_xor),
	// operator &
	AND = offsetof(PyNumberMethods, nb_and),
	// operator |
	OR = offsetof(PyNumberMethods, nb_or),
	// operator ~
	INVERT = offsetof(PyNumberMethods, nb_invert),
	// operator <<
	LEFT_SHIFT = offsetof(PyNumberMethods, nb_lshift),
	// operator >>
	RIGHT_SHIFT = offsetof(PyNumberMethods, nb_rshift),

	// operator +=
	INPLACE_ADD = offsetof(PyNumberMethods, nb_inplace_add),
	// operator -=
	INPLACE_SUBTRACT = offsetof(PyNumberMethods, nb_inplace_subtract),
	// operator *=
	INPLACE_MULTIPLY = offsetof(PyNumberMethods, nb_inplace_multiply),
	// operator /=
	INPLACE_DIVIDE = offsetof(PyNumberMethods, nb_inplace_true_divide),
	// operator %=
	INPLACE_REMAINDER = offsetof(PyNumberMethods, nb_inplace_remainder),
	// operator ^=
	INPLACE_XOR = offsetof(PyNumberMethods, nb_inplace_xor),
	// operator &=
	INPLACE_AND = offsetof(PyNumberMethods, nb_inplace_and),
	// operator |=
	INPLACE_OR = offsetof(PyNumberMethods, nb_inplace_or),
	// operator <<=
	INPLACE_LEFT_SHIFT = offsetof(PyNumberMethods, nb_inplace_lshift),
	// operator >>=
	INPLACE_RIGHT_SHIFT = offsetof(PyNumberMethods, nb_inplace_rshift),

	// operator [integral]
	INT = offsetof(PyNumberMethods, nb_int),
	// operator [floating_point]
	FLOAT = offsetof(PyNumberMethods, nb_float),

	//// operator [], mp_subscript & mp_ass_subscript
	//SUBSCRIPT,

	//// operator (), tp_call
	//CALL,
	//
	//// operator ==, py_EQ
	//EQUAL,
	//// operator !=, py_NE
	//NOT_EQUAL,
	//// operator <, py_LT
	//LESS,
	//// operator <=, py_LE
	//LESS_OR_EQUAL,
	//// operator >, py_GT
	//GREATER,
	//// operator >=, py_GE
	//GREATER_OR_EQUAL
};


	
inline namespace python_embedder_detail
{
	using HashValueType = unsigned long long;


	template<typename T, typename = void>
	struct remove_const_ref
	{
		using type = std::remove_const_t<std::remove_reference_t<T>>;
	};

	template<typename T>
	struct remove_const_ref<T, std::enable_if_t<std::is_lvalue_reference_v<T> && !std::is_const_v<std::remove_reference_t<T>>>>
	{
		using type = T;
	};

	template<typename T>
	using remove_const_ref_t = typename remove_const_ref<T>::type;


	template<typename T, typename = void>
	struct ref_to_ptr
	{
		using type = T;
	};

	template<typename T>
	struct ref_to_ptr<T, std::enable_if_t<std::is_lvalue_reference_v<T> && !std::is_const_v<std::remove_reference_t<T>>>>
	{
		using type = std::remove_reference_t<T>*;
	};

	template<typename T>
	using ref_to_ptr_t = typename ref_to_ptr<T>::type;


	
	template<typename T, typename = void>
	struct is_supported_custom_type : std::false_type
	{};

	template<typename T>
	struct is_supported_custom_type<T, std::enable_if_t<std::is_class_v<T> && std::is_default_constructible_v<T> && std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>>
	> : std::true_type
	{};

	template<typename T>
	constexpr bool is_supported_custom_type_v = is_supported_custom_type<T>::value;


	template<typename T, typename = void>
	struct is_supported_field_type : std::false_type
	{};

	template<typename T>
	struct is_supported_field_type<T, std::enable_if_t<
		std::is_fundamental_v<T> || std::is_enum_v<T> || std::is_same_v<T, const char*> || std::is_same_v<T, std::string>
	>> : std::true_type
	{};

	template<typename T>
	constexpr bool is_supported_field_type_v = is_supported_field_type<T>::value;
	

	template<typename T, typename = void>
	struct is_supported_parameter_type : std::false_type
	{};

	template<typename T>
	struct is_supported_parameter_type<T, std::enable_if_t<
		is_supported_field_type_v<remove_const_ref_t<T>> || 
		is_supported_custom_type_v<remove_const_ref_t<T>> || is_supported_custom_type_v<std::remove_reference_t<T>>
	>> : std::true_type
	{};

	template<typename T>
	constexpr bool is_supported_parameter_type_v = is_supported_parameter_type<T>::value;


	template<typename T, typename = void>
	struct is_supported_return_type : std::false_type
	{};

	template<typename T>
	struct is_supported_return_type<T, std::enable_if_t<
		std::is_void_v<T> || is_supported_field_type_v<T> || 
		is_supported_custom_type_v<remove_const_ref_t<T>> || is_supported_custom_type_v<std::remove_reference_t<T>>
	>> : std::true_type
	{};

	template<typename T>
	constexpr bool is_supported_return_type_v = is_supported_return_type<T>::value;



	template<typename FunctionPtrType_, FunctionPtrType_ FunctionPtr_, HashValueType TemplateParametersHashValue_>
	struct TemplateInstanceInfo
	{
		using FunctionPtrType = FunctionPtrType_;
		static constexpr FunctionPtrType functionPtr = FunctionPtr_;
		static constexpr HashValueType templateParametersHashValue = TemplateParametersHashValue_;
	};


	template<typename E>
	struct EnumeratorInfo
	{
		const char* name;
		E value;
	};

	
	
	template<template<typename, typename = void> typename Checker, typename ParameterTypes, size_t... Indices>
	auto validity_checker_helper(std::index_sequence<Indices...>)
		-> std::conjunction<Checker<std::tuple_element_t<Indices, ParameterTypes>>...>;

	template<template<typename, typename> typename Checker, typename ParameterTypes, size_t ParameterCount>
	using ValidityChecker = decltype(validity_checker_helper<Checker, ParameterTypes>(std::make_index_sequence<ParameterCount>()));


	template<typename ParameterTypes, size_t... Indices>
	auto parameter_type_remove_const_ref_ref_to_ptr_helper(std::index_sequence<Indices...>)
		-> std::tuple<remove_const_ref_t<ref_to_ptr_t<std::tuple_element_t<Indices, ParameterTypes>>>...>;

	template<typename ParameterTypes>
	using RemoveConstRefRefToPtrParameterTuple = decltype(parameter_type_remove_const_ref_ref_to_ptr_helper<ParameterTypes>(std::make_index_sequence<std::tuple_size_v<ParameterTypes>>()));

	
	template<typename ParameterTypes, size_t... Indices>
	auto parameter_type_remove_const_ref_helper(std::index_sequence<Indices...>)
		->std::tuple<remove_const_ref_t<std::tuple_element_t<Indices, ParameterTypes>>...>;

	template<typename ParameterTypes>
	using RemoveConstRefParameterTuple = decltype(parameter_type_remove_const_ref_helper<ParameterTypes>(std::make_index_sequence<std::tuple_size_v<ParameterTypes>>()));


	template<typename T, typename = void>
	struct OriginalValueGetter
	{
		static T GetOriginalValue(T t) { return t; }
	};

	template<typename T>
	struct OriginalValueGetter<T, std::enable_if_t<std::is_pointer_v<T>>>
	{
		static std::remove_pointer_t<T>& GetOriginalValue(T t) { return *t; }
	};

	
	
	template<typename RefToPtrParameterTuple, typename ParameterTuple, size_t... Indices>
	[[nodiscard]] ParameterTuple restore_ref_helper(const RefToPtrParameterTuple& params, std::index_sequence<Indices...>);
	
	template<typename RefToPtrParameterTuple, typename ParameterTuple>
	[[nodiscard]] ParameterTuple restore_ref(const RefToPtrParameterTuple& params) { return restore_ref_helper<RefToPtrParameterTuple, ParameterTuple>(params, std::make_index_sequence<std::tuple_size_v<ParameterTuple>>()); }
	

	template<typename... Args>
	using TypeList = std::tuple<Args...>;

	

	template<typename T, T... Numbers>
	[[nodiscard]] constexpr T get_nth_element(std::integer_sequence<T, Numbers...>, T i)
	{
		constexpr T arr[] = { Numbers... };
		return arr[i];
	}


	
	template<typename ParameterType, typename = void>
	struct TempVar
	{
		using type = ParameterType;
	};

	template<typename ParameterType>
	struct TempVar<ParameterType, std::enable_if_t<sizeof(ParameterType) < sizeof(int)>>
	{
		using type = int;
	};

	template<typename ParameterType>
	using TempVarType = typename TempVar<ParameterType>::type;



	template<typename T>
	struct PyExportedClass
	{
		PyObject_HEAD
		T* pT;
		bool isOwner = false;


		inline static PyTypeObject typeInfo;
		inline static std::vector<PyMemberDef> fields;
		inline static std::vector<PyMethodDef> methods;
		inline static PyNumberMethods numberMethods;

		
		static PyObject* CustomNew(PyTypeObject* type, PyObject* args, PyObject* keywords);
		
		template<typename Offsets, typename... FieldTypes>
		static int CustomInit(PyObject* self_, PyObject* args, PyObject* keywords);
		
		static void CustomDealloc(PyObject* self);
	};


	
	template<typename T>
	struct Converter
	{
		static int ParseValue(PyObject* pyObject, T* pT);
		static int ParseValuePtr(PyObject* pyObject, T** pT);

		static PyObject* BuildValue(T* pT);
		static PyObject* BuildValuePtr(T* pT);
	};
	


	template<typename T>
	[[nodiscard]] static constexpr const char* get_type_format_string(bool treatCharAsNumber = false, bool isReturnType = false);

	template<typename T>
	[[nodiscard]] static constexpr int get_member_type_number();

	

	template<typename ParameterTypeTuple, size_t i>
	[[nodiscard]] bool parse_argument_helper(PyObject* args, ParameterTypeTuple& parsedArguments);
	
	template<typename ParameterTypeTuple, size_t... ArgumentIndices>
	[[nodiscard]] constexpr bool parse_arguments(PyObject* args, ParameterTypeTuple& parsedArguments, std::index_sequence<ArgumentIndices...>);


	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename ReturnType, typename ParameterTypeTuple>
	[[nodiscard]] PyObject* execute_get_return_value(const ParameterTypeTuple& arguments);

	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename ReturnType, typename ClassReferenceType, typename ParameterTypeTuple, typename InstanceType>
	[[nodiscard]] PyObject* execute_and_get_return_value_ref_possible(const ParameterTypeTuple& arguments, InstanceType* pInstance);


	template<typename FieldTypeTuple, typename ExportedClass, typename Offsets, size_t i>
	[[nodiscard]] bool parse_field_helper(ExportedClass* self, PyObject* args);

	template<typename FieldTypeTuple, typename ExportedClass, typename Offsets, size_t... FieldIndices>
	[[nodiscard]] bool parse_fields(ExportedClass* self, PyObject* args, std::index_sequence<FieldIndices...>);
	
	

	template<typename FunctionPtrType, FunctionPtrType FunctionPtr>
	[[nodiscard]] auto get_function_replicated_function();

	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction>
	[[nodiscard]] auto get_member_function_as_static_function_replicated_function();
	
	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, typename LambdaPtrType, LambdaPtrType LambdaPtr>
	[[nodiscard]] auto get_member_function_as_static_function_lambda_replicated_function();

	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename Class>
	[[nodiscard]] auto get_member_function_replicated_function();


	
	template<typename Pairs, HashValueType FunctionNameHashValue, size_t i>
	void template_function_map_register_helper(std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)>& functionMap);

	template<typename Pairs, HashValueType FunctionNameHashValue, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, size_t i>
	void template_member_function_as_static_function_map_register_helper(std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)>& functionMap);

	template<typename Pairs, HashValueType FunctionNameHashValue, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, typename LambdaPtrType, LambdaPtrType LambdaPtr, size_t i>
	void template_member_function_as_static_function_lambda_map_register_helper(std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)>& functionMap);

	template<typename Pairs, HashValueType FunctionNameHashValue, typename Class, size_t i>
	void template_member_function_map_register_helper(std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)>& functionMap);
	
	
	template<typename Pairs, HashValueType FunctionNameHashValue, size_t... Indices>
	void template_function_register_to_map(std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)>& functionMap, std::index_sequence<Indices...>);

	template<typename Pairs, HashValueType FunctionNameHashValue, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, size_t... Indices>
	void template_member_function_as_static_function_register_to_map(std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)>& functionMap, std::index_sequence<Indices...>);

	template<typename Pairs, HashValueType FunctionNameHashValue, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, typename LambdaPtrType, LambdaPtrType LambdaPtr, size_t... Indices>
	void template_member_function_as_static_function_lambda_register_to_map(std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)>& functionMap, std::index_sequence<Indices...>);

	template<typename Pairs, HashValueType FunctionNameHashValue, typename Class, size_t... Indices>
	void template_member_function_register_to_map(std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)>& functionMap, std::index_sequence<Indices...>);

	

	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename ReturnType, size_t ParameterCount, typename RefToPtrParameterTypeTuple, typename ParameterTypeTuple>
	struct FunctionDispatcher
	{
		static PyObject* ReplicatedFunction(PyObject* self, PyObject* args);
	};

	
	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename ReturnType, size_t ParameterCount, typename RefToPtrParameterTypeTuple, typename ParameterTypeTuple,
		typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, typename InstanceReturnFunctionReturnType, size_t InstanceReturnFunctionParameterCount, typename InstanceReturnFunctionParameterTuple>
	struct MemberFunctionAsStaticFunctionDispatcher
	{
		static PyObject* ReplicatedFunction(PyObject* self, PyObject* args);
	};

	
	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename ReturnType, size_t ParameterCount, typename RefToPtrParameterTypeTuple, typename ParameterTypeTuple,
		typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, typename InstanceReturnFunctionReturnType, size_t InstanceReturnFunctionParameterCount, typename InstanceReturnFunctionParameterTypeTuple,
		typename LambdaPtrType, LambdaPtrType LambdaPtr
	>
	struct MemberFunctionAsStaticFunctionLambdaDispatcher
	{
		static PyObject* ReplicatedFunction(PyObject* self, PyObject* args);
	};


	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename ReturnType, size_t ParameterCount, typename RefToPtrParameterTypeTuple, typename ParameterTypeTuple, typename Class>
	struct MemberFunctionDispatcher
	{
		static PyObject* ReplicatedFunction(PyObject* self, PyObject* args);
	};


	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename ReturnType, size_t ParameterCount, typename RefToPtrParameterTypeTuple, typename ParameterTypeTuple, typename Class, EOperatorType Operator>
	struct MemberOperatorDispatcher
	{
		static PyObject* BinaryReplicatedFunction(PyObject* self, PyObject* arg);
		static PyObject* UnaryReplicatedFunction(PyObject* self);
	};


	template<HashValueType FunctionNameHashValue>
	struct TemplateFunctionDispatcher
	{
		static PyObject* ReplicatedFunction(PyObject* self, PyObject* args);

		inline static std::unordered_map<HashValueType, PyObject* (*)(PyObject*, PyObject*)> instantiatedFunctions;
	};
}




	
template<HashValueType ModuleNameHashValue>
class Exporter
{
public:
	template<typename FunctionPtrType, FunctionPtrType FunctionPtr>
	static void RegisterFunction(const char* functionName);


	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction>
	static void RegisterMemberFunctionAsStaticFunction(const char* functionName);

	
	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, typename LambdaPtrType, LambdaPtrType LambdaPtr>
	static void RegisterMemberFunctionAsStaticFunctionLambda(const char* functionName);


	template<typename FunctionPtrType, FunctionPtrType FunctionPtr>
	static void RegisterMemberFunction(const char* functionName);


	template<typename FunctionPtrType, FunctionPtrType FunctionPtr>
	static void RegisterGlobalOperator(EOperatorType operatorType);

	
	template<typename FunctionPtrType, FunctionPtrType FunctionPtr, typename Class, EOperatorType OperatorType>
	static void RegisterMemberOperator();


	template<HashValueType FunctionNameHashValue, typename... TemplateInstanceInfos>
	static void RegisterTemplateFunction(const char* functionName);


	template<HashValueType FunctionNameHashValue, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, typename... TemplateInstanceInfos>
	static void RegisterTemplateMemberFunctionAsStaticFunction(const char* functionName);


	template<HashValueType FunctionNameHashValue, typename InstanceReturnFunctionType, InstanceReturnFunctionType InstanceReturnFunction, typename LambdaPtrType, LambdaPtrType LambdaPtr, typename... TemplateInstanceInfos>
	static void RegisterTemplateMemberFunctionAsStaticFunctionLambda(const char* functionName);


	template<HashValueType FunctionNameHashValue, typename... TemplateInstanceInfos>
	static void RegisterTemplateMemberFunction(const char* functionName);


	template<typename T, typename Offsets, typename... FieldTypes>
	static void RegisterType(const char* typeName, std::initializer_list<PyMemberDef> members);


	template<typename E>
	static void RegisterEnum(const char* enumName, std::initializer_list<EnumeratorInfo<E>> enumerators);


	static void Export(const std::string& moduleName_);


	static PyObject* Init();


private:
	inline static std::string moduleName;
	
	inline static std::vector<PyMethodDef> methods;
	inline static std::vector<PyTypeObject*> types;
	inline static std::vector<std::string> enums;
	
	inline static PyModuleDef moduleDef;
};
}


#include "CppPythonEmbedder.inl"
