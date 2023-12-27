#include "app.h"

int main(void) {

    int res = 0;
    {
        rmp::App app;
        res = app.Run();
    }
        

    return res;
}