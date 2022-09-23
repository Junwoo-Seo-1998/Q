/*
	File Name: Profiler.h
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <chrono>
#include <string>
#include <fstream>
namespace q_engine
{
	struct ProfilingResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct ProfilingSession
	{
		std::string Name;
	};


	
	
	class Profiler
	{
	public:
		Profiler();
		static Profiler& GetProfiler();
		void BeginProfiling(const std::string& name, const std::string& saveAs = "Profile.json");
		void WriteProfilingResult(const ProfilingResult& result);
		void EndProfiling();
		
		
	private:
		ProfilingSession* m_CurrentSession;
		std::ofstream m_Writer;
		unsigned int m_ProfilingCounter;
	};
	
	class ProfilingTimer
	{
	public:
		ProfilingTimer(const char* name);

		~ProfilingTimer();

		void StopTimer();
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_TimerStartedTime;
		bool m_IsStopped;
	};
	
}
#ifdef _DEBUG
#define BEGIN_PROFILING(name, filepath) ::q_engine::Profiler::GetProfiler().BeginProfiling(name, filepath);
#define END_PROFILING() ::q_engine::Profiler::GetProfiler().EndProfiling();
#define SCOPE_PROFILING(name) ::q_engine::ProfilingTimer timer##__LINE__(name);
#define FUNCTION_PROFILING() SCOPE_PROFILING(__FUNCSIG__)
#else
#define BEGIN_PROFILING(name, filepath)
#define END_PROFILING()
#define SCOPE_PROFILING(name)
#define FUNCTION_PROFILING()
#endif