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

//#include <SDL3/SDL.h>

// int main(int argc, char *argv[]) {
//     (void)argc;
//     (void)argv;

//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         SDL_Log("SDL_Init failed (%s)", SDL_GetError());
//         return 1;
//     }

//     SDL_Window *window = NULL;
//     SDL_Renderer *renderer = NULL;

//     if (SDL_CreateWindowAndRenderer("test", 640, 480, 0, &window, &renderer) < 0) {
//         SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
//         SDL_Quit();
//         return 1;
//     }
//     SDL_SetWindowTitle(window, rmp::greet().c_str());

//     while (1) {
//         int finished = 0;
//         SDL_Event event;
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_EVENT_QUIT) {
//                 finished = 1;
//                 break;
//             }
//         }
//         if (finished) {
//             break;
//         }

//         SDL_SetRenderDrawColor(renderer, 80, 80, 80, SDL_ALPHA_OPAQUE);
//         SDL_RenderClear(renderer);
//         SDL_RenderPresent(renderer);
//     }

//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);

//     SDL_Quit();
//     return 0;
// }
