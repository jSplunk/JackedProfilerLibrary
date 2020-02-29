#pragma once

#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

#define Profiling 1
#if Profiling
#define PROFILE(name) Timer timer##__LINE__(name)
#define PROFILE_FUNC() PROFILE(__FUNCSIG__)
#define START_SESSION(name) Tracer::instance->beginSession(name)
#define END_SESSION() Tracer::instance->endSession()
#else
#define PROFILE(name)
#endif

namespace JPL
{
	struct ProfileResult
	{
		const char* Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	class Tracer
	{
	private:
		std::ofstream m_outputStream;
		int m_count;
		std::mutex m_lock;
		bool m_isInSession;
		const char * m_sessionName;
		Tracer()
			: m_count(0), m_sessionName(nullptr), m_isInSession(false)
		{}
		void header();
		void footer();
	public:
		~Tracer();
		Tracer(const Tracer&) = delete;
		void operator= (const Tracer&) = delete;
		static Tracer* instance;
		void beginSession(const char* _name, const char* _filePath = "results.json");
		void endSession();
		void profile(const ProfileResult& result);
	};

	class Timer
	{
	private:
		ProfileResult m_result;

		bool m_isStopped;
		std::chrono::time_point<std::chrono::steady_clock> m_timeStartPoint;
		const char* m_name;
	public:
		Timer(const char * _name)
			: m_result({_name, 0,0,0}), m_isStopped(false)
		{
			m_timeStartPoint = std::chrono::high_resolution_clock::now();
		}
		~Timer();

		void Stop();
	};
}
