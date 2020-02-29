#pragma once

#include <chrono>
#include <iostream>
#include <fstream>

namespace JPL
{
	struct ProfileResult
	{
		const char* Name;
		long long Start, End;
	};

	struct Session
	{
		const char* Name;
	};

	class Tracer : public Session
	{
	private:
		std::ofstream m_outputStream;
		int m_count;
		Tracer()
			: m_count(0)
		{}
	public:
		~Tracer();
		Tracer(const Tracer&) = delete;
		void operator= (const Tracer&) = delete;
		static Tracer* instance;
		void beginSession(const char* _name, const char* _filePath = "results.json");
		void endSession();
		void header();
		void footer();
		void profile(const ProfileResult& result);

	};

	class Timer
	{
	private:
		bool m_isStopped;
		std::chrono::time_point<std::chrono::steady_clock> m_timeStartPoint;
		const char* m_name;
	public:
		Timer(const char *);
		~Timer();

		void Stop();
	};

#define Profiling 1
#if Profiling
#define PROFILE(name) Timer timer##__LINE__(name)
#define PROFILE_FUNC() PROFILE(__FUNCSIG__)
#else
#define PROFILE(name) 
#endif
}
