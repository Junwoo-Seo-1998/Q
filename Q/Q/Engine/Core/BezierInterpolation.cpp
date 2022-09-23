/*
    File Name: BezierInterpolation.cpp
    Project Name: Q
    Author(s):
        Primary: Minjae Kyung
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "BezierInterpolation.h"

#include <iostream>


namespace q_engine
{
	BezierInterpolation::BezierInterpolation(glm::vec<2, double>* track, glm::vec<2, double> P1, glm::vec<2, double> P2, glm::vec<2, double> P3, glm::vec<2, double> P4, double duration)
		: track(track), P1(P1), P2(P2), P3(P3), P4(P4), mDurationTime(duration), mNowTime(0), mIsDead(false)
	{

	}

	glm::vec<2, double> BezierInterpolation::GetNowPosition(double progress)
	{
		return (P1 * pow(1 - progress, 3)) + (P2 * progress * pow(1 - progress, 2) * 3.0) + (P3 * pow(progress, 2) * (1 - progress) * 3.0) + (P4 * pow(progress, 3));
	}

	void BezierInterpolation::Update(double dt)
	{
		mNowTime += dt;
		double progress = mNowTime / mDurationTime;

		if (progress <= 1)
		{
			*track = GetNowPosition(progress);
		}
		else
		{
			*track = P4;
			mIsDead = true;
		}
	}



	TimeInterpolation::TimeInterpolation(double* track, double start, double end, double duration, double startBezierStrength, double endBezierStrength)
		: mTrack(track), mLastT(0)
	{
		mDurationTime = duration;
		mNowTime = 0;

		mIsDead = false;


		if ((startBezierStrength < -1 || 1 < startBezierStrength)
			|| (endBezierStrength < -1 || 1 < endBezierStrength))
		{
			std::cout << "error : beziers must between -1 ~ 1";
		}
		else if ((startBezierStrength < 0)
			&& (endBezierStrength < 0))
		{
			std::cout << "error : one of beziers must bigger than 0";
		}

		if (endBezierStrength == 2)
			endBezierStrength = startBezierStrength;

		P1 = { 0, start };

		if (startBezierStrength > 0)
			P2 = { startBezierStrength, start };
		else
			P2 = { 0, end + startBezierStrength };

		if (endBezierStrength > 0)
			P3 = { 1 - endBezierStrength, end };
		else
			P3 = { 1, start - endBezierStrength };

		P4 = { 1, end };


	}

	void TimeInterpolation::Update(double dt)
	{
		mNowTime += dt;
		double progress = mNowTime / mDurationTime;

		if (progress <= 1)
		{
			*mTrack = FindPosition(progress);
		}
		else
		{
			*mTrack = P4.y;
			mIsDead = true;
		}
	}


	constexpr double SEARCH_ACCURACY = 1000;

	double TimeInterpolation::FindPosition(double timeProgress)
	{
		glm::vec<2, double> result = { 0,0 };
		double progress;

		for (progress = mLastT; result.x < timeProgress; progress += (1 / SEARCH_ACCURACY))
		{
			result = GetNowPosition(progress);
		}

		mLastT = progress;
		return result.y;
	}



	InterpolationManager::~InterpolationManager()
	{
		
	}

	
	void InterpolationManager::AddTimeLerp(double* track, double start, double end, double duration, double startBezierStrength, double endBezierStrength)
	{
		mInterpolations.push_back(new TimeInterpolation{ track, start, end, duration, startBezierStrength, endBezierStrength });
	}

	void InterpolationManager::AddSpaceLerp(glm::vec<2, double>* track, glm::vec<2, double> P1, glm::vec<2, double> P2, glm::vec<2, double> P3, glm::vec<2, double> P4, double duration)
	{
		mInterpolations.push_back(new BezierInterpolation{ track, P1, P2, P3, P4, duration });
	}

	void InterpolationManager::Load()
	{
		
	}

	
	void InterpolationManager::Update(double dt)
	{
		for (BezierInterpolation* i : mInterpolations)
		{
			i->Update(dt);
			if (i->IsDead())
			{
				mDestroyList.push_back(i);
			}
		}

		for (BezierInterpolation* i : mDestroyList)
		{
			mInterpolations.remove(i);
			delete i;
		}
		mDestroyList.clear();
	}

	void InterpolationManager::Unload()
	{
		
	}

}

