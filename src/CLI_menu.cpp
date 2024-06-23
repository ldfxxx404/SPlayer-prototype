#include "CLI_menu.h"
#include <vector>
#include <iostream>
#include <ncurses.h>
#include <dirent.h>
#include <unistd.h>

std::vector<std::pair<std::string, bool>> getFiles(const std::string &initPath) {
    std::vector<std::pair<std::string, bool>> files;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(initPath.c_str())) != NULL) {
        files.push_back({"..", true});
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] == '.')
                continue;

            files.push_back({ent->d_name, ent->d_type == DT_DIR});
        }
        closedir(dir);
    }
    else {
        perror("Failed to open directory");
    }

    return files;
}

std::string browseFile(const std::string &initPath) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    std::string currentPath = initPath;
    std::vector<std::pair<std::string, bool>> files = getFiles(currentPath);

    if (files.empty()) {
        endwin();
        std::cerr << "No files found in the directory." << std::endl;
        return "";
    }

    int highlight = 0;
    int choice;
    int n_choices = files.size();

    while (1) {

        clear();
        mvprintw(0, 0, "Current directory: %s", currentPath.c_str());

        for (int i = 0; i < n_choices; ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
                mvprintw(i + 1, 0, "%s", files[i].first.c_str());
                attroff(A_REVERSE);
            }
            else {
                mvprintw(i + 1, 0, "%s", files[i].first.c_str());
            }
        }

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
        case 10: // Enter key
            if (files[highlight].second) {
                if (files[highlight].first == "..")
                {
                    size_t position = currentPath.find_last_of("/");

                    if (position != std::string::npos) {
                        currentPath = currentPath.substr(0, position);
                    }

                    files = getFiles(currentPath);
                    highlight = 0;
                    n_choices = files.size();
                }
                else if (files[highlight].first != ".") {
                    currentPath += "/" + files[highlight].first;
                    files = getFiles(currentPath);
                    highlight = 0;
                    n_choices = files.size();
                }
            }
            else {
                endwin();
                return currentPath + "/" + files[highlight].first;
            }
            break;

        case 'q':
            endwin();
            return "";
        default:
            break;
        }
    }

    endwin();
    return "";
}
