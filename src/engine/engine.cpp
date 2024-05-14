#include "engine.h"
#include "base/logging/log.h"

namespace kmp
{
    void Engine::ShwowFatalError(const String &error)
    {
        error_handler_(error);
    }

    Engine::~Engine()
    {
    }
}