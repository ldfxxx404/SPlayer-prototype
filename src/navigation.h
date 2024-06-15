#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void dirList()
{
    fs::path dirPath = "/";
    if(fs::exists(dirPath) && fs::is_directory (dirPath))
    {
        for(const auto& entry: fs::directory_iterator(dirPath))
        {
            std::cout << "Directory: " << entry.path().string() << std::endl;
        }
    }
    else
    {
        std::cerr << "Directory not found" << std::endl;
    }
}
