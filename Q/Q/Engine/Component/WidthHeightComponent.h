/*
	File Name: WidthHeightComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>
#include <sol/sol.hpp>


#include "../ECS/Component.h"

namespace q_engine
{
	class WidthHeightComponent : public Component<WidthHeightComponent>
	{
	public:
		using vec2i = glm::vec<2, int, glm::defaultp>;
		
		WidthHeightComponent();
		WidthHeightComponent(float Width, float Height, bool UI_relative = false);
	    WidthHeightComponent(glm::vec2 WidthHeight, bool UI_relative = false);
		void SetWidth(float Width);
		void SetHeight(float Height);
		void SetWidthHeight(float Width, float Height);

		void SetWidthHeight(glm::vec2 WidthHeight);
		glm::vec2 GetWidthHeight() const;
		template <typename T>
		T GetWidth() const { return static_cast<T>(mWidth); }
		template <typename T>
		T GetHeight() const { return static_cast<T>(mHeight); }

		glm::vec2 GetMiddlePos() const { return GetWidthHeight() * 0.5f; }

        [[nodiscard]] bool IsUIRelative() const { return m_is_UI_relative; }
		virtual Json::Value Serialize() override;
		virtual void Deserialize(Json::Value& jsonValue) override;;


	private:
		float mWidth = 0.f;
		float mHeight = 0.f;
		bool m_is_UI_relative = false;
	};

	void RegisterWidthHeightComponent(sol::state& state);
}