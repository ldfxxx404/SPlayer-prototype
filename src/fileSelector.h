#include <iostream>
#include <dirent.h>
#include <vector>

std::vector<std::string> browseFiles(const std::string &path)
{
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent -> d_type == DT_REG)
            {
                files.push_back(ent -> d_name);
            }
            
        }
        closedir(dir);
    }
    else
    {
        perror("Failed to open directory");
    }
return files;
}
