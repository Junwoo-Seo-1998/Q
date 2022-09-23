/*
	File Name: Profiler.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Profiler.h"

#include <algorithm>
#include <thread>

namespace q_engine
{
	

	Profiler::Profiler()
		:m_CurrentSession(nullptr),m_ProfilingCounter(0)
	{
	}

	Profiler& Profiler::GetProfiler()
	{
		//singleton
		static Profiler profiler;
		return profiler;
	}

	void Profiler::BeginProfiling(const std::string& name, const std::string& saveAs)
	{
		m_Writer.open(saveAs);
		m_Writer << "{\"otherData\": {},\"traceEvents\":[";
		m_Writer.flush();
		m_CurrentSession = new ProfilingSession{ name };
	}

	void Profiler::WriteProfilingResult(const ProfilingResult& result)
	{
		if (m_ProfilingCounter++ > 0)
			m_Writer << ",";

		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_Writer << "{";
		m_Writer << "\"cat\":\"function\",";
		m_Writer << "\"dur\":" << (result.End - result.Start) << ',';
		m_Writer << "\"name\":\"" << name << "\",";
		m_Writer << "\"ph\":\"X\",";
		m_Writer << "\"pid\":0,";
		m_Writer << "\"tid\":" << result.ThreadID << ",";
		m_Writer << "\"ts\":" << result.Start;
		m_Writer << "}";

		m_Writer.flush();
	}

	void Profiler::EndProfiling()
	{
		m_Writer << "]}";
		m_Writer.flush();
		m_Writer.close();
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfilingCounter = 0;
	}

	ProfilingTimer::ProfilingTimer(const char* name)
		:m_Name(name), m_IsStopped(false)
	{
		m_TimerStartedTime = std::chrono::high_resolution_clock::now();;
	}

	ProfilingTimer::~ProfilingTimer()
	{
		if (!m_IsStopped)
			StopTimer();
	}

	void ProfilingTimer::StopTimer()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTime
			= std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_TimerStartedTime).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Profiler::GetProfiler().WriteProfilingResult({ m_Name, start, end, threadID });

		m_IsStopped = true;
	}
}
