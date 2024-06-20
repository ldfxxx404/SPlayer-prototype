#ifndef CLI_MENU_H
#define CLI_MENU_H

#include <vector>
#include <string>

std::vector<std::pair<std::string, bool>> getFiles(const std::string &initPath);
std::string browseFile(const std::string &initPath);

#endif
