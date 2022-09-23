/*
	File Name: ContiguousHashMap.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <type_traits>
#include <iterator>
#include <unordered_map>
#include <thread>

#include "../ECS/Component.h"
#include "ContiguousHashMapIterator.h"
#include "Engine/Types.h"

namespace q_engine
{
	extern const unsigned MAX_THREAD;
	
	template <class T>
	class ContiguousPartContainer
	{
	public:
		using iterator = T*;
		
	private:
		iterator mBegin;
		iterator mEnd;
	public:
		ContiguousPartContainer() = delete;
		ContiguousPartContainer(T* begin, T* end) :mBegin(begin), mEnd(end) {}
		
		iterator begin() { return mBegin; }
		iterator end() { return mEnd; }
	};
	
	class IContiguousHashMap
	{
	public:
		using size_type = int;
		virtual void Delete(IDType entityID) = 0;

		virtual void* GetValuePtr(IDType valueID) = 0;

	    [[nodiscard]] virtual bool isExist(IDType key) = 0;
	};
	
	template <class T>
	class ContiguousHashMap : public IContiguousHashMap
	{
	public:
		using element_type = T;
		using value_type = std::remove_cv_t<T>;
		using size_type = int;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using iterator = ContiguousHashMapIterator<T>;
		using reverse_iterator = std::reverse_iterator<iterator>;

	protected:
		value_type* mElements{ nullptr };

		size_type mSizeNum{ 0 };
		size_type mCapacityNum{ 0 };

		std::unordered_map<IDType, value_type*> mElementIDs;
	public:
		template<typename Q = value_type, std::enable_if_t<std::is_default_constructible_v<Q>>* = nullptr>
		ContiguousHashMap();
		template<typename Q = value_type, std::enable_if_t<!std::is_default_constructible_v<Q>>* = nullptr>
		ContiguousHashMap();
		template<typename Q = value_type, std::enable_if_t<std::is_default_constructible_v<Q>>* = nullptr>
		ContiguousHashMap(const ContiguousHashMap& other);
		template<typename Q = value_type, std::enable_if_t<!std::is_default_constructible_v<Q>>* = nullptr>
		ContiguousHashMap(const ContiguousHashMap& other);
		ContiguousHashMap(ContiguousHashMap&& other) noexcept;

		~ContiguousHashMap();

		template<typename Q = value_type, std::enable_if_t<std::is_default_constructible_v<Q>>* = nullptr>
		ContiguousHashMap& operator=(const ContiguousHashMap& other);
		template<typename Q = value_type, std::enable_if_t<!std::is_default_constructible_v<Q>>* = nullptr>
		ContiguousHashMap& operator=(const ContiguousHashMap& other);
		
		ContiguousHashMap& operator=(ContiguousHashMap&& other) noexcept;
		
		[[nodiscard]] reference Front() const;
		[[nodiscard]] reference Back() const;
		[[nodiscard]] reference operator[](size_type index);
		[[nodiscard]] reference operator[] (size_type index) const;
		[[nodiscard]] reference at(size_type index);
		[[nodiscard]] reference at(size_type index) const;
		[[nodiscard]] pointer Data() const noexcept;

		[[nodiscard]] size_type Size() const noexcept;
		[[nodiscard]] size_type SizeBytes() const noexcept;
		[[nodiscard]] bool isEmpty() const noexcept;

		[[nodiscard]] iterator begin() const;
		[[nodiscard]] iterator end() const;
		[[nodiscard]] reverse_iterator rbegin() const;
		[[nodiscard]] reverse_iterator rend() const;

		[[nodiscard]] iterator find(IDType key);
		[[nodiscard]] bool isExist(IDType key) override;
		
		IDType GetKeyByValueAddress(pointer address);
		template <typename Q = value_type, typename... Args>
		std::enable_if_t<std::is_default_constructible_v<Q>, iterator> Add(IDType elementID, Args... args);
		template <typename Q = value_type, typename... Args>
		std::enable_if_t<!std::is_default_constructible_v<Q>, iterator> Add(IDType elementID, Args... args);

		template<typename Q = value_type, std::enable_if_t<std::is_destructible_v<Q>>* = nullptr>
		void Remove(IDType elementID);
		template<typename Q = value_type, std::enable_if_t<!std::is_destructible_v<Q>>* = nullptr>
		void Remove(IDType elementID);

		void Delete(IDType elementID);
		
		void Clear();

		ContiguousPartContainer<T> GetPartContainer(size_type maxPart, size_type partNum);
		void* GetValuePtr(IDType valueID) override;
	};
}

#include "ContiguousHashMap.inl"
