#pragma once
#include <stdarg.h>

namespace kmp {
	namespace log {
		enum class Severity {
			kInfo = 0,
			kWarning,
			kError,
			kFatalError,
		};


		void Initialize();
		void Shutdown();
		bool IsInitialized();

		void AddEntry(Severity severity, char const* category, char const* source_info, char const* filename, int line_number, char const* message_format, ...);
		void AddEntryVarArgs(Severity severity, char const* category, char const* source_info, char const* filename, int line_number, char const* message_format, va_list args);


		void TraceMessage(const char* format, ...);
	};
}
#define KMP_LOG(...) log::AddEntry( log::Severity::kInfo, "", nullptr, __FILE__, __LINE__, __VA_ARGS__ )
#define KMP_LOG_INFO( category, source, ... ) log::AddEntry( log::Severity::kInfo, category, source, __FILE__, __LINE__, __VA_ARGS__ )
#define KMP_LOG_WARNING( category, source, ... ) log::AddEntry( log::Severity::kWarning, category, source, __FILE__, __LINE__, __VA_ARGS__ )
#define KMP_LOG_ERROR( category, source, ... ) log::AddEntry( log::Severity::kError, category, source, __FILE__, __LINE__, __VA_ARGS__ )
#define KMP_LOG_FATAL_ERROR( category, source, ... ) log::AddEntry( log::Severity::kFatalError, category, source, __FILE__, __LINE__, __VA_ARGS__ )




