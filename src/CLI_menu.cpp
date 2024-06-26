#include "CLI_menu.h"
#include <vector>
#include <iostream>
#include <ncurses.h>
#include <dirent.h>
#include <unistd.h>

std::vector<std::pair<std::string, bool>> getFiles(const std::string &initPath) {

    std::vector<std::pair<std::string, bool>> files;
    DIR *dir = opendir(initPath.c_str());

    if (dir == nullptr) {

        perror("Failed to open directory");
        return files;
    }

    files.push_back({"..", true});

    struct dirent *ent;

    while ((ent = readdir(dir)) != nullptr) {

        if (ent->d_name[0] == '.')
            continue;

        files.push_back({ent->d_name, ent->d_type == DT_DIR});
    }

    closedir(dir);
    return files;
}

void initializeNcurses() {

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
}

void finalizeNcurses() {

    endwin();
}

void displayFiles(const std::string& currentPath, const std::vector<std::pair<std::string, bool>>& files, int highlight) {

    clear();

    mvprintw(0, 0, "Current directory: %s", currentPath.c_str());

    for (size_t i = 0; i < files.size(); ++i) {

        if (static_cast<int>(i) == highlight) {

            attron(A_REVERSE);
            mvprintw(i + 1, 0, "%s", files[i].first.c_str());
            attroff(A_REVERSE);

        } else {

            mvprintw(i + 1, 0, "%s", files[i].first.c_str());
        }
    }
}

std::string handleDirectorySelection(const std::string& currentPath, const std::string& selectedDir) {

    if (selectedDir == "..") {

        size_t position = currentPath.find_last_of("/");

        if (position != std::string::npos) {

            return currentPath.substr(0, position);
        }

        return currentPath;
    }

    return currentPath + "/" + selectedDir;
}

std::string browseFile(const std::string &initPath) {

    initializeNcurses();

    std::string currentPath = initPath;
    std::vector<std::pair<std::string, bool>> files = getFiles(currentPath);
    
    files.insert(files.begin(), {"AnonFM", false});
    
    if (files.empty()) {

        finalizeNcurses();
        std::cerr << "No files found in the directory." << std::endl;
        return "";
    }

    int highlight = 0;
    int choice;
    int n_choices = files.size();

    while (true) {
        
        displayFiles(currentPath, files, highlight);

        choice = getch();

        switch (choice) {

            case KEY_UP:
                if (highlight > 0) {
                    --highlight;
                }
                break;

            case KEY_DOWN:
                if (highlight < n_choices - 1) {
                    ++highlight;
                }
                break;

            case 10: 
                if (highlight == 0) {
                    
                    finalizeNcurses();
                    return "http://anon.fm:8000/radio";
                }
                
                if (files[highlight].second) { 
                    
                    currentPath = handleDirectorySelection(currentPath, files[highlight].first);
                    files = getFiles(currentPath);
                    highlight = 0;
                    n_choices = files.size();
                
                } else {
                    
                    finalizeNcurses();
                    return currentPath + "/" + files[highlight].first;
                }
                break;
            
            case 'q':
                finalizeNcurses();
                return "";
                
            default:
                break;
        }
    }

    finalizeNcurses();
    return "";
}
