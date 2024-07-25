#pragma once
#include "defs.h"
#include "base/types/array.h"

namespace kmp::file
{
    bool LoadFile(char const *file_path, Blob &file_data);
}