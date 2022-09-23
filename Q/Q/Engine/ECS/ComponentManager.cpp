/*
	File Name: ComponentManager.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "ComponentManager.h"

namespace q_engine
{
	std::unordered_multimap<IDType, IDType>& ComponentManager::GetAllComponentInfo()
	{
		return mComponentInfo;
	}

	std::vector<IDType> ComponentManager::GetComponentList(IDType entityID)
	{
		std::vector<IDType> existList;
		for (auto& idx : mComponentArrays)
		{
			if (idx.second->isExist(entityID))
				existList.push_back(idx.first);
		}

		return std::move(existList);
	}
	
	bool ComponentManager::HasComponent(IDType typeID, IDType entityID)
	{
		if (mComponentArrays.find(typeID) == mComponentArrays.end())
			return false;

		return mComponentArrays[typeID]->isExist(entityID);
	}

	void ComponentManager::DeleteComponents(IDType entityID)
	{
		for (auto& idx : mComponentArrays)
		{
			if (idx.second->isExist(entityID))
			{
				reinterpret_cast<IComponent*>(idx.second->GetValuePtr(entityID))->Unload();
				idx.second->Delete(entityID);
			}
		}
		mComponentInfo.erase(entityID);
	}

	void ComponentManager::DeleteComponent(IDType typeID, IDType entityID)
	{
		if(IComponent* deleteComp = reinterpret_cast<IComponent*>(mComponentArrays[typeID]->GetValuePtr(entityID)); deleteComp)
		{
			deleteComp->Unload();
		}
		else
		{
			return;
		}
		mComponentArrays[typeID]->Delete(entityID);
		auto& iters = mComponentInfo.equal_range(entityID);

		for (auto iter = iters.first; iter != iters.second; ++iter)
		{
			if (iter->second == typeID)
			{
				mComponentInfo.erase(iter);
				break;
			}
		}
	}

	void ComponentManager::DeleteComponentLater(IDType typeID, IDType entityID)
	{
		mSingleComponentToBeDeleted.push({ typeID,entityID });
	}

	void ComponentManager::DeleteComponentsLater(IDType entityID)
	{
		mComponentsToBeDeleted.push(entityID);
	}


	void ComponentManager::ClearComponentToDelete()
	{
		while (!mComponentsToBeDeleted.empty())
		{
			IDType id = mComponentsToBeDeleted.front();
			mComponentsToBeDeleted.pop();
			DeleteComponents(id);
		}

		while (!mSingleComponentToBeDeleted.empty())
		{
			auto [typeID,enttID] = mSingleComponentToBeDeleted.front();
			mSingleComponentToBeDeleted.pop();
			DeleteComponent(typeID, enttID);
		}
	}

	void ComponentManager::Unload()
	{
		mComponentArrays.clear();
	}
}
