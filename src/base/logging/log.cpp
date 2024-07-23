#include "defs.h"
#include "log.h"
#include "base/types/string.h"
#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
#include <chrono>
// #include <ctime>

namespace kmp
{
    namespace log
    {
        static bool is_log_system_initialized_ = false;

        static char const *const severity_labels_[] = {"Message", "Warning", "Error", "Fatal Error"};

        void Initialize()
        {
            is_log_system_initialized_ = true;
        }

        void Shutdown()
        {
            is_log_system_initialized_ = false;
        }

        bool IsInitialized()
        {
            return is_log_system_initialized_;
        }

        void AddEntry(Severity severity, char const *category, char const *source_info, char const *filename, int line_number, char const *message_format, ...)
        {
            KMP_ASSERT(log::IsInitialized());
            va_list args;
            va_start(args, message_format);
            AddEntryVarArgs(severity, category, source_info, filename, line_number, message_format, args);
            va_end(args);
        }

        void AddEntryVarArgs(Severity severity, char const *category, char const *source_info, char const *filename, int line_number, char const *message_format, va_list args)
        {

            KMP_ASSERT(log::IsInitialized());
            KMP_ASSERT(category != nullptr && filename != nullptr && message_format != nullptr);

            String timestamp;
            String message = "msg";

            constexpr auto time_len = 9;
            timestamp.resize(time_len);
            //
            time_t const t = std::time(nullptr);
            tm new_time;

            localtime_s(&new_time, &t);
            std::strftime(timestamp.data(), time_len, "%H:%M:%S", &new_time);

            message.sprintf_va_list(message_format, args);
            va_end(args);

            InlineStringSized traceMessage;
            if (!source_info)
            {
                traceMessage.sprintf("[%s][%s][%s] %s", timestamp.c_str(), severity_labels_[(int32_t)severity], category, message.c_str());
            }
            else
            {
                traceMessage.sprintf("[%s][%s][%s][%s] %s", timestamp.c_str(), severity_labels_[(int32_t)severity], category, source_info, message.c_str());
            }

            // Print to debug trace
            KMP_TRACE_MSG(traceMessage.c_str());

            // Print to std out
            printf_s("%s\n", traceMessage.c_str());
        }
#ifdef _WIN32
        void TraceMessage(const char *format, ...)
        {
            constexpr size_t bufferSize = 2048;
            char messageBuffer[bufferSize]; // Dont make this static as we need this to be threadsafe!!!

            va_list args;
            va_start(args, format);
            int32_t numCharsWritten = _vsnprintf_s(messageBuffer, bufferSize, bufferSize - 1, format, args);
            va_end(args);

            // Add newlines
            if (numCharsWritten > 0 && numCharsWritten < 509)
            {
                messageBuffer[numCharsWritten] = '\r';
                messageBuffer[numCharsWritten + 1] = '\n';
                messageBuffer[numCharsWritten + 2] = 0;
            }

            OutputDebugStringA(messageBuffer);
        }
#endif
    }
}
