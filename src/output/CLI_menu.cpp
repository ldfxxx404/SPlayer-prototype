#include "CLI_menu.h"
#include <iostream>
#include <dirent.h>
#include <ncurses.h>
#include <vector>
#include <string>
#include <algorithm>

std::string browseFile(const std::string &directory)
{
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> files;

    if ((dir = opendir(directory.c_str())) != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            if (ent->d_type == DT_REG)
            {
                files.push_back(ent->d_name);
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Could not open directory");
        return "";
    }

    if (files.empty())
    {
        return "";
    }

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    size_t highlight = 0;
    int choice;
    while (1)
    {
        clear();
        for (size_t i = 0; i < files.size(); ++i)
        {
            if (i == highlight)
            {
                attron(A_REVERSE);
            }
            mvprintw(i, 0, files[i].c_str());
            if (i == highlight)
            {
                attroff(A_REVERSE);
            }
        }
        choice = getch();

        switch (choice)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                --highlight;
            }
            break;
        case KEY_DOWN:
            if (highlight < files.size() - 1)
            {
                ++highlight;
            }
            break;
        case 10: // Enter key
            endwin();
            return files[highlight];
        case 'q': // Exit key
            endwin();
            return "";
        default:
            break;
        }
    }
}
