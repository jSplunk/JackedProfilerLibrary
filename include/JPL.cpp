#include "pch.h"
#include "JPL.h"
// JPL : Defines the functions for the static library.
//

namespace JPL
{
	//Creates a new instance of the class (Singleton)
	Tracer* Tracer::instance = new Tracer();

	Tracer::~Tracer()
	{
		endSession();
	}

	//Starts the session for tracing the timers
	void Tracer::beginSession(const char* _name, const char* _filePath)
	{
		if (m_isInSession)
			endSession();
		m_outputStream.open(_filePath);
		m_isInSession = true;
		header();
		m_sessionName = _name;
	}

	//Ends the current session
	void Tracer::endSession()
	{
		if (!m_isInSession) return;
		m_isInSession = false;
		footer();
		m_outputStream.close();
		m_count = 0;
	}

	//Creates the header for the json file
	void Tracer::header()
	{
		m_outputStream << "{\"otherData\": {}, \"traceEvents\":[";
	}

	//Creates the footer for the json file
	void Tracer::footer()
	{
		m_outputStream << "]}";
	}

	//Sets the profile data in json file
	void Tracer::profile(const ProfileResult& result)
	{
		std::lock_guard<std::mutex> lock(m_lock);

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
		m_outputStream << "\"tid\":\""<< result.ThreadID <<"\",";
		m_outputStream << "\"ts\":" << result.Start << "}";
	}

	Timer::~Timer()
	{
		if (!m_isStopped)
			Stop();
	}

	//Calculates the time between when the Timer object was created and when the function was called
	void Timer::Stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		m_result.Start = std::chrono::time_point_cast<std::chrono::microseconds>(m_timeStartPoint).time_since_epoch().count();
		m_result.End = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		m_result.ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

		Tracer::instance->profile(m_result);

		m_isStopped = true;

	}



}