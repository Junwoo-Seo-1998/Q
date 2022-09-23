/*
    File Name: TagComponent.h
    Project Name: Q
    Author(s):
        Primary: Junwoo Seo
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"
class TagComponent : public q_engine::Component<TagComponent>
{
public:
    TagComponent() = default;
    TagComponent(const std::string& tag)
	    :Tag(tag){}
    std::string Tag;
};
