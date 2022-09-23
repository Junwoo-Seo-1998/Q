/*
	File Name: Event.inl
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary: Minjae Kyung
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once


namespace q_engine
{
	template<typename Sys, typename = void>
	struct HasOnEvent : std::false_type {};

	template<typename Sys>
	struct HasOnEvent<Sys, decltype(std::declval<Sys>().OnEvent(std::declval<IEvent*>()))>
		: std::true_type {};

	
	template<typename Sys>
	void notify(IEvent* event)
	{
		static_assert(HasOnEvent<Sys>::value);

		if(Engine::GetEngineComponent<GameStateManager>().HasState(GameStateType::GAME))
		{
			const auto& currentLevel = Engine::GetEngineComponent<GameStateManager>().GetPlayState();
			currentLevel->GetUpdateSystem<Sys>().OnEvent(event);
		}
	}
	
	template<typename T, typename ...ListeningSystems_>
	void Event<T, ListeningSystems_...>::NotifyAll()
	{
		if constexpr (sizeof...(ListeningSystems_) == 0)
		{
			return;
		}
		else
		{
			using dummy = int[];
			(void)dummy {
				(notify<ListeningSystems_>(static_cast<IEvent*>(this)), 0)...
			};
		}
	}
}
