/*
	File Name: ContiguousHashMap.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "ContiguousHashMap.h"
#include <stdexcept>
constexpr unsigned int DEFAULT_CAPACITY = 512;

namespace q_engine
{
	template <typename T>
	template<typename Q, std::enable_if_t<std::is_default_constructible_v<Q>>*>
	ContiguousHashMap<T>::ContiguousHashMap()
	{
		mSizeNum = 0;
		mCapacityNum = DEFAULT_CAPACITY;
		mElements = new value_type[mCapacityNum];
	}
	template <typename T>
	template<typename Q, std::enable_if_t<!std::is_default_constructible_v<Q>>*>
	ContiguousHashMap<T>::ContiguousHashMap()
	{
		mSizeNum = 0;
		mCapacityNum = DEFAULT_CAPACITY;
		mElements = reinterpret_cast<value_type*>(new char[mCapacityNum * sizeof(value_type)]);
	}

	template <class T>
	template<typename Q, std::enable_if_t<std::is_default_constructible_v<Q>>*>
	ContiguousHashMap<T>::ContiguousHashMap(const ContiguousHashMap& other)
	{
		mSizeNum = other.mSizeNum;
		mCapacityNum = other.mCapacityNum;
		mElements = reinterpret_cast<value_type*>(new char[mCapacityNum * sizeof(value_type)]);
		mElementIDs = other.mElementIDs;

		int i = 0;
		for (auto idx : other)
			mElements[i++] = idx;
	}
	template <class T>
	template<typename Q, std::enable_if_t<!std::is_default_constructible_v<Q>>*>
	ContiguousHashMap<T>::ContiguousHashMap(const ContiguousHashMap& other)
	{
		mSizeNum = other.mSizeNum;
		mCapacityNum = other.mCapacityNum;
		mElements = reinterpret_cast<value_type*>(new char[mCapacityNum * sizeof(value_type)]);
		mElementIDs = other.mElementIDs;

		int i = 0;
		for (auto idx : other)
			mElements[i++] = idx;
	}
	
	template <typename T>
	ContiguousHashMap<T>::ContiguousHashMap(ContiguousHashMap&& other) noexcept
	{
		mElements = other.mElements;
		mSizeNum = other.mSizeNum;
		mCapacityNum = other.mCapacityNum;
		mElementIDs = std::move(other.mElementIDs);
		
		other.mElements = nullptr;
		other.mSizeNum = 0;
	}

	template <typename T>
	ContiguousHashMap<T>::~ContiguousHashMap()
	{
		if(std::is_default_constructible_v<value_type>)
 			delete[] mElements;
		else
			delete[] reinterpret_cast<char*>(mElements);
		mElementIDs.clear();
		mElements = nullptr;
		mSizeNum = 0;
	}

	template <typename T>
	template<typename Q, std::enable_if_t<std::is_default_constructible_v<Q>>*>
	ContiguousHashMap<T>& ContiguousHashMap<T>::operator=(const ContiguousHashMap<T>& other)
	{
		if (&other == this)
			return *this;

		if (mElements != nullptr)
			delete[] mElements;

		mSizeNum = other.mSizeNum;
		mCapacityNum = other.mCapacityNum;
		mElements = new value_type[mCapacityNum];
		mElementIDs = other.mElementIDs;

		int i = 0;
		for (auto idx : other)
			mElements[i++] = idx;

		return *this;
	}

	template <typename T>
	template<typename Q, std::enable_if_t<!std::is_default_constructible_v<Q>>*>
	ContiguousHashMap<T>& ContiguousHashMap<T>::operator=(const ContiguousHashMap<T>& other)
	{
		if (&other == this)
			return *this;

		if (mElements != nullptr)
			delete[] mElements;

		mSizeNum = other.mSizeNum;
		mCapacityNum = other.mCapacityNum;
		mElements = reinterpret_cast<value_type*>(new char[mCapacityNum * sizeof(value_type)]);
		mElementIDs = other.mElementIDs;

		int i = 0;
		for (auto idx : other)
			mElements[i++] = idx;

		return *this;
	}

	template <class T>
	ContiguousHashMap<T>& ContiguousHashMap<T>::operator=(ContiguousHashMap<T>&& other) noexcept
	{
		if (&other == this)
			return *this;
		
		if (mElements != nullptr)
			delete[] mElements;

		mElements = other.mElements;
		mSizeNum = other.mSizeNum;
		mCapacityNum = other.mCapacityNum;
		mElementIDs = std::move(other.mElementIDs);

		other.mElements = nullptr;
		other.mSizeNum = 0;
		
		return *this;
	}

	template <typename T>
	typename ContiguousHashMap<T>::reference ContiguousHashMap<T>::Front() const
	{
		return mElements[0];
	}

	template <typename T>
	typename ContiguousHashMap<T>::reference ContiguousHashMap<T>::Back() const
	{
		return mElements[mSizeNum - 1];
	}

	template <typename T>
	typename ContiguousHashMap<T>::reference ContiguousHashMap<T>::operator[](size_type index)
	{
#ifdef _DEBUG
		if (mElementIDs.find(index) == mElementIDs.end())
		{
			throw std::logic_error("can not find element on ContiguousHashMap on index " + std::to_string(index));
		}
#endif
		
		return *mElementIDs[index];
	}

	template <class T>
	typename ContiguousHashMap<T>::reference ContiguousHashMap<T>::operator[](size_type index) const
	{
#ifdef _DEBUG
		if (mElementIDs.find(index) == mElementIDs.end())
		{
			throw std::logic_error("can not find element on ContiguousHashMap on index " + std::to_string(index));
		}
#endif

		return *mElementIDs[index];
	}

	template <class T>
	typename ContiguousHashMap<T>::reference ContiguousHashMap<T>::at(size_type index)
	{
		return mElements[index];
	}

	template <class T>
	typename ContiguousHashMap<T>::reference ContiguousHashMap<T>::at(size_type index) const
	{
		return mElements[index];
	}

	template <typename T>
	typename ContiguousHashMap<T>::pointer ContiguousHashMap<T>::Data() const noexcept
	{
		return mElements;
	}

	template <typename T>
	typename ContiguousHashMap<T>::size_type ContiguousHashMap<T>::Size() const noexcept
	{
		return mSizeNum;
	}

	template <typename T>
	typename ContiguousHashMap<T>::size_type ContiguousHashMap<T>::SizeBytes() const noexcept
	{
		return mSizeNum * sizeof(T);
	}

	template <typename T>
	[[nodiscard]] bool ContiguousHashMap<T>::isEmpty() const noexcept
	{
		return mElements == nullptr || mSizeNum == 0;
	}

	template <typename T>
	typename ContiguousHashMap<T>::iterator ContiguousHashMap<T>::begin() const
	{
		return iterator(mElements);
	}
	template <typename T>
	typename ContiguousHashMap<T>::iterator ContiguousHashMap<T>::end() const
	{
		return iterator(mElements + mSizeNum);
	}
	template <typename T>
	typename ContiguousHashMap<T>::reverse_iterator ContiguousHashMap<T>::rbegin() const
	{
		return std::make_reverse_iterator<iterator>(end());
	}
	template <typename T>
	typename ContiguousHashMap<T>::reverse_iterator ContiguousHashMap<T>::rend() const
	{
		return std::make_reverse_iterator<iterator>(begin());
	}

	template <typename T>
	typename ContiguousHashMap<T>::iterator ContiguousHashMap<T>::find(IDType key)
	{
		if (mElementIDs.find(key) != mElementIDs.end())
			return ContiguousHashMap<T>::iterator(mElementIDs[key]);
		else
		{
			return this->end();
		}
	}

	template <class T>
	bool ContiguousHashMap<T>::isExist(IDType key)
	{
		return mElementIDs.find(key) != mElementIDs.end();
	}


	template <typename T>
	template <typename Q, typename... Args>
	std::enable_if_t<std::is_default_constructible_v<Q>, typename ContiguousHashMap<T>::iterator> ContiguousHashMap<T>::Add(IDType elementID, Args... args)
	{
		if (mElementIDs.find(elementID) == mElementIDs.end())
		{
			++mSizeNum;
			if (mSizeNum > mCapacityNum) // extend object pool
			{
				mCapacityNum *= 2;

				value_type* temp = new value_type[mCapacityNum];


				for (int i = 0; i < mSizeNum - 1; ++i)
				{
					temp[i] = mElements[i];
				}
				
				for(auto& hash : mElementIDs)
				{
					hash.second = temp + (hash.second - mElements);
				}
				
				delete[] mElements;
				mElements = temp;
			}

			element_type* compAddress = new(mElements + mSizeNum - 1) element_type{ std::forward<Args>(args)... };
			mElementIDs.insert({ elementID, compAddress });

			return compAddress;
		}
		else
		{
			//error message to logger
			return nullptr;
		}
	}

	template <typename T>
	template <typename Q, typename... Args>
	std::enable_if_t<!std::is_default_constructible_v<Q>, typename ContiguousHashMap<T>::iterator> ContiguousHashMap<T>::Add(IDType elementID, Args... args)
	{
		if (mElementIDs.find(elementID) == mElementIDs.end())
		{
			++mSizeNum;
			if (mSizeNum > mCapacityNum) // extend object pool
			{
				mCapacityNum *= 2;

				value_type* temp = reinterpret_cast<value_type*>(new char[mCapacityNum * sizeof(value_type)]);

				for (int i = 0; i < mSizeNum - 1; ++i)
				{
					temp[i] = mElements[i];
				}

				for (auto& hash : mElementIDs)
				{
					hash.second = temp + (hash.second - mElements);
				}

				delete[] reinterpret_cast<char*>(mElements);
				mElements = temp;
			}

			element_type* compAddress = new(mElements + mSizeNum - 1) element_type{ std::forward<Args>(args)... };
			mElementIDs.insert({ elementID, compAddress });

			return compAddress;
		}
		else
		{
			// error message to logger
			return nullptr;
		}
	}

	template <typename T>
	typename IDType ContiguousHashMap<T>::GetKeyByValueAddress(pointer address)
	{
		for (auto idx : mElementIDs)
		{
			if (idx.second == address)
				return idx.first;
		}

		//Call logger to say error
		return INVALID_ID;
	}
	
	template <typename T>
	template<typename Q, std::enable_if_t<std::is_destructible_v<Q>>*>
	void ContiguousHashMap<T>::Remove(IDType entityID)
	{
		auto destroyComponent = mElementIDs.find(entityID);
		if (destroyComponent != mElementIDs.end())
		{
			*(destroyComponent->second).~T();

			*(destroyComponent->second) = mElements[mSizeNum - 1]; //replace destroying component to last component
			mElementIDs[GetKeyByValueAddress(mElements + mSizeNum - 1)] = destroyComponent->second; // replace destroyed address to last map
			mElementIDs.erase(destroyComponent);

			--mSizeNum;
		}
		else
		{
			// error message to logger
		}
	}

	template <typename T>
	template<typename Q, std::enable_if_t<!std::is_destructible_v<Q>>*>
	void ContiguousHashMap<T>::Remove(IDType entityID)
	{
		auto destroyComponent = mElementIDs.find(entityID);
		if (destroyComponent != mElementIDs.end())
		{
			*(destroyComponent->second) = mElements[mSizeNum - 1]; //replace destroying component to last component
			mElementIDs[GetKeyByValueAddress(mElements + mSizeNum - 1)] = destroyComponent->second; // replace destroyed address to last map
			mElementIDs.erase(destroyComponent);

			--mSizeNum;
		}
		else
		{
			//error message to logger
		}
	}

	template<class T>
	void ContiguousHashMap<T>::Delete(IDType elementID)
	{
		auto destroyComponent = mElementIDs.find(elementID);
		if (destroyComponent != mElementIDs.end())
		{
			*(destroyComponent->second) = mElements[mSizeNum - 1]; //replace destroying component to last component

			mElementIDs[GetKeyByValueAddress(mElements + mSizeNum - 1)] = destroyComponent->second; // replace destroyed address to last map
			mElementIDs.erase(destroyComponent);

			--mSizeNum;
		}
		else
		{
			// error message to logger
		}
	}

	template <class T>
	void ContiguousHashMap<T>::Clear()
	{
		mSizeNum = 0;
		mElementIDs.clear();
	}

	template <class T>
	ContiguousPartContainer<T> ContiguousHashMap<T>::GetPartContainer(size_type maxPart, size_type partNum)
	{
		if (maxPart > mSizeNum)
		{
			size_type partSize = 1;

			if (partNum < mSizeNum)
				return ContiguousPartContainer<T>(mElements + (partSize * partNum), mElements + (partSize * (partNum + 1)));
			else
				return ContiguousPartContainer<T>(mElements + mSizeNum, mElements + mSizeNum);
		}
		{
			size_type partSize = mSizeNum / maxPart;

			if (partNum == maxPart - 1)
				return ContiguousPartContainer<T>(mElements + (partSize * partNum), mElements + mSizeNum);
			else
				return ContiguousPartContainer<T>(mElements + (partSize * partNum), mElements + (partSize * (partNum + 1)));
		}
	}

	template<class T>
	inline void* ContiguousHashMap<T>::GetValuePtr(IDType valueID)
	{
		return reinterpret_cast<void*>(mElementIDs[valueID]);
	}
}
