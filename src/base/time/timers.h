#pragma once

#include "base/time/time.h"
//#include "Base/Types/Percentage.h"

//-------------------------------------------------------------------------

namespace kmp
{
    //-------------------------------------------------------------------------
    // Timers
    //-------------------------------------------------------------------------
    // Use 'PlatformClock' for the actual platform/OS time
    // Use 'EngineClock' for the engine time (accumulated delta times). This takes into account debugging pauses, etc...
    // There are also a set of manual timers provided to allow for gameplay needs where things like time dilation or manual update of timers is needed

    // Basic timer
    template<typename Clock>
    class Timer {
    public:

        Timer() = default;

        inline void Start() { m_startTime = Clock::GetTime(); }
        inline void Reset() { Start(); }

        inline Nanoseconds GetElapsedTimeNanoseconds() const { return Clock::GetTime() - m_startTime; }
        inline Seconds GetElapsedTimeSeconds() const { return GetElapsedTimeNanoseconds().ToSeconds(); }
        inline Milliseconds GetElapsedTimeMilliseconds() const { return GetElapsedTimeNanoseconds().ToMilliseconds(); }
        inline Microseconds GetElapsedTimeMicroseconds() const { return GetElapsedTimeNanoseconds().ToMicroseconds(); }

    private:

        Nanoseconds m_startTime = Clock::GetTime();
    };

    template<typename Clock>
    class ScopedTimer {

    public:

        ScopedTimer(Milliseconds& result) : m_result(result) {};
        ~ScopedTimer() { m_result = m_timer.GetElapsedTimeMilliseconds(); }

    private:

        ScopedTimer() = delete;
        void operator=(ScopedTimer const&) = delete;

    private:

        Timer<Clock>        m_timer;
        Milliseconds& m_result;
    };

#if 0
 
    
    //-------------------------------------------------------------------------

    // Countdown Timer
    template<typename Clock>
    class CountdownTimer
    {
    public:

        CountdownTimer() = default;

        inline void Start( Seconds cooldownTime )
        {
            m_startTime = Clock::GetTime();
            m_countdownTime = cooldownTime;
            m_isRunning = true;
        }

         // Update the timer. Returns true when the countdown is complete
        inline bool Update()
        {
            KMP_ASSERT( m_isRunning );

            if ( GetElapsedTimeSeconds() > m_countdownTime )
            {
                m_isRunning = false;
                return true;
            }

            return false;
        }

        inline void Stop()
        {
            m_countdownTime = 0.0f;
            m_isRunning = false;
        }

        inline bool IsRunning() const { return m_isRunning; }
        inline bool IsComplete() const { return !m_isRunning; }

        inline Seconds GetRemainingTime() const
        {
            KMP_ASSERT( m_isRunning );
            return math::Max( 0.0f, (float) ( m_countdownTime - GetElapsedTimeSeconds() ) );
        }

        inline Percentage GetPercentageThrough() const
        {
            return Percentage( GetElapsedTimeSeconds() / m_countdownTime ).GetClamped( false );
        }

        inline Seconds GetElapsedTimeSeconds() const { return ( Clock::GetTime() - m_startTime ).ToSeconds(); }

        inline Seconds GetElapsedTimeMilliseconds() const { return ( Clock::GetTime() - m_startTime ).ToMilliseconds(); }

    private:

        Nanoseconds     m_startTime = 0;
        Seconds         m_countdownTime = 0.0f;
        bool            m_isRunning = false;
    };

    //-------------------------------------------------------------------------

 

    //-------------------------------------------------------------------------
    // Manual Timers
    //-------------------------------------------------------------------------
    // These are timers that are manually updated, primarily needed for gameplay reasons

    // Basic timer
    class ManualTimer
    {
    public:

        ManualTimer() { Start(); }

        inline void Start() { m_elapsedTime = 0.0f; }
        inline void Reset() { Start(); }

        // Call this function to update the elapsed time since the timer was started, returns the total elapsed time
        inline Seconds Update( Seconds deltaTime ) 
        {
            m_elapsedTime += deltaTime;
            return m_elapsedTime; 
        }

        KMP_FORCE_INLINE Nanoseconds GetElapsedTimeNanoseconds() const { return m_elapsedTime.ToNanoseconds(); }
        KMP_FORCE_INLINE Seconds GetElapsedTimeSeconds() const { return m_elapsedTime; }
        KMP_FORCE_INLINE Milliseconds GetElapsedTimeMilliseconds() const { return m_elapsedTime.ToMilliseconds(); }
        KMP_FORCE_INLINE Microseconds GetElapsedTimeMicroseconds() const { return m_elapsedTime.ToMicroseconds(); }

    private:

        Seconds m_elapsedTime = 0.0f;
    };

    //-------------------------------------------------------------------------

    // A cooldown timer i.e. counts down from a specified time
    class ManualCountdownTimer
    {
    public:

        ManualCountdownTimer() {}

        inline void Start( Seconds cooldownTime )
        {
            m_cooldownTime = cooldownTime;
            m_remainingTime = cooldownTime;
            m_isRunning = true;
        }

        // Update the timer. Returns true when the countdown is complete
        inline bool Update( Seconds deltaTime )
        {
            KMP_ASSERT( m_isRunning );

            m_remainingTime -= deltaTime;
            m_remainingTime = math::Max( 0.0f, (float) m_remainingTime );
            if ( m_remainingTime == 0.0f )
            {
                m_isRunning = false;
                return true;
            }

            return false;
        }

        inline void Stop()
        {
            m_remainingTime = 0.0f;
            m_isRunning = false;
        }

        inline bool IsRunning() const { return m_isRunning; }
        inline bool IsComplete() const { return !m_isRunning; }

        inline Seconds GetRemainingTime() const
        {
            KMP_ASSERT( m_isRunning );
            return m_remainingTime;
        }

        inline Percentage GetPercentageThrough() const
        {
            return Percentage( GetElapsedTimeSeconds() / m_cooldownTime ).GetClamped( false );
        }

        KMP_FORCE_INLINE Seconds GetElapsedTimeSeconds() const { return m_cooldownTime - m_remainingTime; }
        KMP_FORCE_INLINE Milliseconds GetElapsedTimeMilliseconds() const { return GetElapsedTimeSeconds().ToMilliseconds(); }
        KMP_FORCE_INLINE Microseconds GetElapsedTimeMicroseconds() const { return GetElapsedTimeSeconds().ToMicroseconds(); }

    private:

        Seconds         m_remainingTime = 0.0f;
        Seconds         m_cooldownTime = 0.0f;
        bool            m_isRunning = false;
    };
#endif
}