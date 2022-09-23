/*
    File Name: BezierInterpolation.h
    Project Name: Q
    Author(s):
        Primary: Minjae Kyung
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <list>
#include <vector>

#include <glm/glm.hpp>

#include "../../Engine/EngineComponent.h"


namespace q_engine
{
	class InterpolationManager;

	struct BezierInterpolation
	{
		glm::vec<2, double>* track;

	protected:
		glm::vec<2, double> P1{ 0,0 };// (start,0)
		glm::vec<2, double> P2{ 0,0 };// (start, startBezier) 
		glm::vec<2, double> P3{ 0,0 };// (end,0)
		glm::vec<2, double> P4{ 0,0 };// (end, endBezier) 

		double mDurationTime{ 0 }; // Interplortation duration time;
		double mNowTime{ 0 }; // now progression time

		bool mIsDead{ false };

	public:
		BezierInterpolation() = default;
		BezierInterpolation(glm::vec<2, double>* track, glm::vec<2, double> P1, glm::vec<2, double> P2, glm::vec<2, double> P3, glm::vec<2, double> P4, double duration);

		glm::vec<2, double> GetNowPosition(double progress);
		[[nodiscard]] double GetRemainTime() const { return mDurationTime - mNowTime; }
		virtual void Update(double dt);
		[[nodiscard]] bool IsDead(void) const { return mIsDead; }

		friend class interpolationManager;
	};

	class TimeInterpolation : public BezierInterpolation
	{
		double* mTrack;

		double mLastT;

		double FindPosition(double time);

	public:
		TimeInterpolation(double* track, double start, double end, double duration, double startBezierStrength = 0, double endBezierStrength = 2);
		//beziers must between -1 ~ 1 (if endBezier == 2, same as startBezier)
		//if bezier strength is out of range, it will do undefined thing

		void Update(double dt) override;
	};

	class InterpolationManager : public EngineComponent<InterpolationManager>
	{
		std::list<BezierInterpolation*> mInterpolations;
		std::vector<BezierInterpolation*> mDestroyList;

	public:
		InterpolationManager() = default;
		
		void AddTimeLerp(double* track, double start, double end, double duration, double startBezierStrength = 0, double endBezierStrength = 2);
		void AddSpaceLerp(glm::vec<2, double>* track, glm::vec<2, double> P1, glm::vec<2, double> P2, glm::vec<2, double> P3, glm::vec<2, double> P4, double duration);
		void Load() override;
		void Update(double deltaTime) override;
		void Unload() override;

		~InterpolationManager();
	};
}
