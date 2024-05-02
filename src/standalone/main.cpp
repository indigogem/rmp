#include "base/application/app.h"

int main(int argc, char* argv[]) {
    int result = 0;
    {
    //     kmp::AppGlobalState state;
        kmp::App app;

        result = app.Run(argc, argv);
    }

#if KMP_DEVELOPMENT
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif

    return result;
}

