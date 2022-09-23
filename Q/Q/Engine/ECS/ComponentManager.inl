/*
	File Name: ComponentManager.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:Junwoo Seo
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

namespace q_engine
{
	template<class Comp>
	ContiguousHashMap<Comp>& ComponentManager::GetComponents()
	{
		static_assert(std::is_base_of_v<IComponent, Comp>, "T is not Component");

		if (mComponentArrays.find(Comp::GetTypeID()) == mComponentArrays.end())
		{
		
			mComponentArrays.insert({ Comp::GetTypeID(),
				std::reinterpret_pointer_cast<IContiguousHashMap>(std::make_shared<ContiguousHashMap<Comp>>()) });
		}
		return *(std::reinterpret_pointer_cast<ContiguousHashMap<Comp>>(mComponentArrays[Comp::GetTypeID()]));
	}

	template<class Comp, class Ent, typename... Args>
	IComponent* ComponentManager::AddComponent(Ent* entityID, Args... args)
	{
		static_assert(std::is_base_of_v<IComponent, Comp>, "T is not Component when Add Component");
		//static_assert(std::is_base_of_v<IEntity, Ent>, "T is not Entity when Add Component");
		if (mComponentArrays.find(Comp::GetTypeID()) == mComponentArrays.end())
		{
			mComponentArrays.insert({ Comp::GetTypeID(),
				std::reinterpret_pointer_cast<IContiguousHashMap>(std::make_shared<ContiguousHashMap<Comp>>()) });
		}

		auto component = std::reinterpret_pointer_cast<ContiguousHashMap<Comp>>(mComponentArrays[Comp::GetTypeID()])
			->Add(entityID->GetEntityID(), std::forward<Args>(args)...);

		component->mEntityID = entityID->GetEntityID();
		component->mEntityTypeID = entityID->GetRuntimeTypeID();
		component->OnComponentCreation();
		mComponentInfo.insert({ component->mEntityID, Comp::GetTypeID() });
		
		return &(*component);
	}

	template<class Comp, typename... Args>
	IComponent* ComponentManager::AddComponent(IDType entityType, IDType entityID, Args... args)
	{
		static_assert(std::is_base_of_v<IComponent, Comp>, "T is not Component when Add Component");
		//static_assert(std::is_base_of_v<IEntity, Ent>, "T is not Entity when Add Component");
		if (mComponentArrays.find(Comp::GetTypeID()) == mComponentArrays.end())
		{
			mComponentArrays.insert({ Comp::GetTypeID(),
				std::reinterpret_pointer_cast<IContiguousHashMap>(std::make_shared<ContiguousHashMap<Comp>>()) });
		}

		auto component = std::reinterpret_pointer_cast<ContiguousHashMap<Comp>>(mComponentArrays[Comp::GetTypeID()])
			->Add(entityID, std::forward<Args>(args)...);

		component->mEntityID = entityID;
		component->mEntityTypeID = entityType;
		component->OnComponentCreation();
		mComponentInfo.insert({ component->mEntityID, Comp::GetTypeID() });

		return &(*component);
	}

	template<class Comp>
	bool ComponentManager::HasComponent(IDType entityID)
	{
		if (mComponentArrays.find(Comp::GetTypeID()) == mComponentArrays.end())
			return false;
		
		return mComponentArrays[Comp::GetTypeID()]->isExist(entityID);
	}
	
	template<class Comp>
	void ComponentManager::DeleteComponent(IDType entityID)
	{
		mComponentArrays[Comp::GetTypeID()]->Delete(entityID);
		auto& iters = mComponentInfo.equal_range(entityID);
		
		for (auto iter=iters.first; iter!=iters.second; ++iter)
		{
			if(iter->second==Comp::GetTypeID())
			{
				mComponentInfo.erase(iter);
				break;
			}
		}
	}
}