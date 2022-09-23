/*
    File Name: CreditState.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary: Sieun choi
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/GameState.h"

enum CREDITORDER
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX
};

class CreditState : public q_engine::UIState
{
public:
    void Load() override;
	void Update(double deltaTime) override;
	void Unload() override;
private:
	int order = ONE;
};

