#include "filesystem.h"
#include <fstream>
#include <filesystem>

namespace kmp::file
{
    bool LoadFile(char const *file_path, Blob &file_data)
    {
        std::ifstream ifs(file_path, std::ios::in | std::ios::binary | std::ios::ate);
        if (ifs.is_open() == false)
            return false;

        std::ifstream::pos_type file_size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        file_data.resize(file_size);
        ifs.read(reinterpret_cast<char *>(file_data.data()), file_size);

        return true;
    }
}