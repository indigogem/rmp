#include "base/application/app.h"

static inline void DebugCrtInit(long break_alloc)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW);
    if (break_alloc != 0)
        _CrtSetBreakAlloc(break_alloc);
}

static inline void DebugCrtDumpLeaks()
{
    _CrtDumpMemoryLeaks();
}

int main(int argc, char *argv[])
{
#if KMP_DEVELOPMENT
    DebugCrtInit(0);
#endif
    int result = EXIT_SUCCESS;
    {
        kmp::AppGlobalState state;
        kmp::App app;

        result = app.Run(argc, argv);
    }

#if KMP_DEVELOPMENT
    DebugCrtDumpLeaks();
#endif

    return result;
}
