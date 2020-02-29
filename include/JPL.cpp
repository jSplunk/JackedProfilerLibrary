#include "pch.h"
#include "JPL.h"
// JPL : Defines the functions for the static library.
//

namespace JPL
{
	Tracer* Tracer::instance = new Tracer();

	Tracer::~Tracer()
	{
	}

	void Tracer::beginSession(const char* _name, const char* _filePath)
	{
		m_outputStream.open(_filePath);
		header();
		Name = _name;
	}

	void Tracer::endSession()
	{
		footer();
		m_outputStream.close();
		m_count = 0;
	}

	void Tracer::header()
	{
		m_outputStream << "{\"otherData\": {}, \"traceEvents\":[";
		m_outputStream.flush();
	}

	void Tracer::footer()
	{
		m_outputStream << "]}";

		m_outputStream.flush();
	}

	void Tracer::profile(const ProfileResult& result)
	{
		if (m_count++ > 0)
		{
			m_outputStream << ",";
		}

		const char* name = result.Name;

		m_outputStream << "{";
		m_outputStream << "\"cat\":\"function\",";
		m_outputStream << "\"dur\":" << (result.End - result.Start) << ",";
		m_outputStream << "\"name\":\"" << name << "\",";
		m_outputStream << "\"ph\":\"X\",";
		m_outputStream << "\"pid\":\"0\",";
		m_outputStream << "\"tid\":\"0\",";
		m_outputStream << "\"ts\":" << result.Start << "}";

		m_outputStream.flush();
	}

	Timer::Timer(const char * _name)
	{
		m_name = _name;
		m_timeStartPoint = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer()
	{
		if (!m_isStopped)
			Stop();
	}

	void Timer::Stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_timeStartPoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		Tracer::instance->profile(ProfileResult{ m_name, start, end });

		m_isStopped = true;

	}



}