#include "time_stamp.h"
//#include "base/math/math.h"
#include <cmath>

//-------------------------------------------------------------------------

namespace kmp
{
    TimeStamp::TimeStamp()
    {
        Update();
    }

    void TimeStamp::Update()
    {
        m_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
    }

    String TimeStamp::GetTime() const
    {
        tm new_time;
        localtime_s(&new_time, &m_time);

        String s;
        s.resize( 9 );
        strftime( s.data(), 9, "%H:%M:%S", &new_time);
        return s;
    }

    kmp::String TimeStamp::GetTimeDetailed() const
    {
        tm new_time;
        localtime_s(&new_time, &m_time);

        // TODO: get millisecond precision
        String s;
        s.resize( 12 );
        strftime( s.data(), 12, "%H:%M:%S", &new_time);
        return s;
    }

    String TimeStamp::GetDayMonthYear() const
    {
        tm new_time;
        localtime_s(&new_time, &m_time);

        String s;
        s.resize( 11 );
        strftime( s.data(), 11, "%d/%m/%Y", &new_time);
        return s;
    }

    String TimeStamp::GetDayMonth() const
    {
        tm new_time;
        localtime_s(&new_time, &m_time);
        
        String s;
        s.resize( 6 );
        strftime( s.data(), 6, "%d/%m", &new_time);
        return s;
    }

    String TimeStamp::GetDateTime() const
    {
        tm new_time;
        localtime_s(&new_time, &m_time);

        String s;
        s.resize( 21 );
        strftime( s.data(), 21, "%Y/%m/%d %H:%M:%S", &new_time);
        return s;
    }
}