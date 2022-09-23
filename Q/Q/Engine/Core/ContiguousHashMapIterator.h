/*
	File Name: ContiguousHashMapIterator.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

namespace q_engine
{
	template <typename T>
	class ContiguousHashMapIterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;
	private:
		pointer current_iter{ nullptr };

	public:
		
		ContiguousHashMapIterator() = default;
		ContiguousHashMapIterator(const ContiguousHashMapIterator& other);
		ContiguousHashMapIterator(ContiguousHashMapIterator&& other);
		ContiguousHashMapIterator(pointer pointer);

		~ContiguousHashMapIterator() = default;

		ContiguousHashMapIterator& operator=(const ContiguousHashMapIterator& other);
		ContiguousHashMapIterator& operator=(ContiguousHashMapIterator&& other);

		ContiguousHashMapIterator& operator++();
		ContiguousHashMapIterator operator++(int);
		ContiguousHashMapIterator& operator--();
		ContiguousHashMapIterator operator--(int);

		reference operator[](const int index) const;
		ContiguousHashMapIterator& operator+=(difference_type num);
		ContiguousHashMapIterator operator+(difference_type num) const;
		friend ContiguousHashMapIterator operator+(difference_type num, const ContiguousHashMapIterator& it);

		ContiguousHashMapIterator& operator-=(difference_type num);
		ContiguousHashMapIterator operator-(difference_type num) const;
		difference_type operator-(const ContiguousHashMapIterator& other) const;
		friend ContiguousHashMapIterator operator-(difference_type num, const ContiguousHashMapIterator& it);

		bool operator!=(const ContiguousHashMapIterator& other) const;
		bool operator==(const ContiguousHashMapIterator& other) const;
		bool operator<(const ContiguousHashMapIterator& other) const;
		bool operator<=(const ContiguousHashMapIterator& other) const;
		bool operator>(const ContiguousHashMapIterator& other) const;
		bool operator>=(const ContiguousHashMapIterator& other) const;

		pointer operator->() const;
		reference operator*() const;
	};

}

#include "ContiguousHashMapIterator.inl"