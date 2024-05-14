#pragma once

#include "defs.h"
#include "base/types/string.h"
#include <chrono>

//-------------------------------------------------------------------------
// Timestamp
//-------------------------------------------------------------------------
// This is a utility type to help track low precision timestamps
// DO NOT use this for timing - Use the timers provided in Timers.h instead

// WARNING: this class using non-threadsafe localtime() calls to convert to string.

namespace kmp
{
    class KMP_BASE_API TimeStamp
    {
    public:

        TimeStamp();

        // Update the timestamp to the current time
        void Update();

        // Get time as HH:MM:SS
        String GetTime() const;

        // Get time as HH:MM:SS:MS
        String GetTimeDetailed() const;

        // Get date as DD/MM/YYYY
        String GetDayMonthYear() const;

        // Get date as DD/MM
        String GetDayMonth() const;

        // Get date as YY/MM/DD - HH::MM::SS
        String GetDateTime() const;

        inline bool operator<( TimeStamp const& rhs ) const { return m_time < rhs.m_time; }
        inline bool operator<=( TimeStamp const& rhs ) const { return m_time <= rhs.m_time; }
        inline bool operator>( TimeStamp const& rhs ) const { return m_time > rhs.m_time; }
        inline bool operator>=( TimeStamp const& rhs ) const { return m_time >= rhs.m_time; }

    private:

        time_t  m_time;
    };
}