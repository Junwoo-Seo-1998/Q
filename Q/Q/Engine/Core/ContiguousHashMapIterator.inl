/*
	File Name: ContiguousHashMapIterator.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "ContiguousHashMapIterator.h"

namespace q_engine
{
	template <typename T>
	ContiguousHashMapIterator<T>::ContiguousHashMapIterator(const ContiguousHashMapIterator& other) : current_iter(other.current_iter) {}

	template <typename T>
	ContiguousHashMapIterator<T>::ContiguousHashMapIterator(ContiguousHashMapIterator&& other) : current_iter(other.current_iter) { other.current_iter = nullptr; }

	template <typename T>
	ContiguousHashMapIterator<T>::ContiguousHashMapIterator(pointer pointer) : current_iter(pointer) {}


	template <typename T>
	ContiguousHashMapIterator<T>& ContiguousHashMapIterator<T>::operator=(const ContiguousHashMapIterator<T>& other)
	{
		current_iter = other.current_iter;

		return *this;
	}
	template <typename T>
	ContiguousHashMapIterator<T>& ContiguousHashMapIterator<T>::operator=(ContiguousHashMapIterator<T>&& other)
	{
		current_iter = other.current_iter;
		other.current_iter = nullptr;

		return *this;
	}

	template <typename T>
	ContiguousHashMapIterator<T>& ContiguousHashMapIterator<T>::operator++()
	{
		++current_iter;
		return *this;
	}
	template <typename T>
	ContiguousHashMapIterator<T> ContiguousHashMapIterator<T>::operator++(int)
	{
		pointer legacy = current_iter;
		++current_iter;
		return legacy;
	}
	template <typename T>
	ContiguousHashMapIterator<T>& ContiguousHashMapIterator<T>::operator--()
	{
		--current_iter;
		return *this;
	}
	template <typename T>
	ContiguousHashMapIterator<T> ContiguousHashMapIterator<T>::operator--(int)
	{
		pointer legacy = current_iter;
		--current_iter;
		return legacy;
	}

	template <typename T>
	typename ContiguousHashMapIterator<T>::reference ContiguousHashMapIterator<T>::operator[](const int index) const
	{
		return current_iter[index];
	}
	template <typename T>
	ContiguousHashMapIterator<T>& ContiguousHashMapIterator<T>::operator+=(difference_type num)
	{
		current_iter += num;
		return *this;
	}
	template <typename T>
	ContiguousHashMapIterator<T> ContiguousHashMapIterator<T>::operator+(difference_type num) const
	{
		return ContiguousHashMapIterator(current_iter + num);
	}
	template <typename T>
	ContiguousHashMapIterator<T>& ContiguousHashMapIterator<T>::operator-=(difference_type num)
	{
		current_iter -= num;
		return *this;
	}
	template <typename T>
	ContiguousHashMapIterator<T> ContiguousHashMapIterator<T>::operator-(difference_type num) const
	{
		return ContiguousHashMapIterator(current_iter - num);
	}

	template <typename T>
	typename ContiguousHashMapIterator<T>::difference_type ContiguousHashMapIterator<T>::operator-(const ContiguousHashMapIterator<T>& other) const
	{
		return current_iter - other.current_iter;
	}

	template <typename T>
	ContiguousHashMapIterator<T> operator+(typename ContiguousHashMapIterator<T>::difference_type num, const ContiguousHashMapIterator<T>& it)
	{
		return ContiguousHashMapIterator<T>(it.current_iter + num);
	}
	template <typename T>
	ContiguousHashMapIterator<T> operator-(typename ContiguousHashMapIterator<T>::difference_type num, const ContiguousHashMapIterator<T>& it)
	{
		return ContiguousHashMapIterator<T>(it.current_iter - num);
	}


	template <typename T>
	bool ContiguousHashMapIterator<T>::operator==(const ContiguousHashMapIterator<T>& other) const
	{
		return current_iter == other.current_iter;
	}
	template <typename T>
	bool ContiguousHashMapIterator<T>::operator!=(const ContiguousHashMapIterator<T>& other) const
	{
		return current_iter != other.current_iter;
	}
	template <typename T>
	bool ContiguousHashMapIterator<T>::operator<(const ContiguousHashMapIterator<T>& other) const
	{
		return current_iter < other.current_iter;
	}
	template <typename T>
	bool ContiguousHashMapIterator<T>::operator<=(const ContiguousHashMapIterator<T>& other) const
	{
		return current_iter <= other.current_iter;
	}
	template <typename T>
	bool ContiguousHashMapIterator<T>::operator>(const ContiguousHashMapIterator<T>& other) const
	{
		return current_iter > other.current_iter;
	}
	template <typename T>
	bool ContiguousHashMapIterator<T>::operator>=(const ContiguousHashMapIterator<T>& other) const
	{
		return current_iter >= other.current_iter;
	}

	template <typename T>
	typename ContiguousHashMapIterator<T>::pointer ContiguousHashMapIterator<T>::operator->() const
	{
		return current_iter;
	}
	template <typename T>
	typename ContiguousHashMapIterator<T>::reference ContiguousHashMapIterator<T>::operator*() const
	{
		return *current_iter;
	}
}
