#ifndef CLI_MENU_H
#define CLI_MENU_H

#include <vector>
#include <string>

std::vector<std::string> getFiles(const std::string &path);
std::string browseFile(const std::string &path);

#endif
