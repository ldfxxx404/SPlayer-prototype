#pragma once

#include <vector>
#include <iostream>
#include <ncurses.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

const std::string mainDirectory = "/"; /* Base directory */

std::vector<std::string> getFiles(const std::string &path) /* list the files in the directory. Start */
{
    std::vector<std::string> files;
    DIR *currentDir;
    struct dirent *entry;

    std::string fullPath = mainDirectory + "/" + path;

    if ((currentDir = opendir(fullPath.c_str())) != NULL)
    {
        files.push_back("...");

        while ((entry = readdir(currentDir)) != NULL)
        {
            std::string name = entry->d_name;
            if (name == "." || name == ".." || name[0] == '.')
                continue;

            files.push_back(name);
        }
        closedir(currentDir);
    }
    else
    {
        perror("Failed to open directory");
    }

    return files;
} /* End */

std::string browseFile(const std::string &initPath) /* Browse files in current dir. Start */
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    std::string currentPath = initPath;
    std::vector<std::string> files = getFiles(currentPath);

    if (files.empty())
    {
        endwin();
        return "";
    }

    int highlight = 0;
    int choice;
    int n_choices = files.size();

    while (true)
    {
        clear();
        mvprintw(0, 0, "Current dir: %s", currentPath.c_str());

        for (int i = 0; i < n_choices; ++i)
        {
            if (i == highlight)
            {
                attron(A_REVERSE);
                mvprintw(i + 1, 0, "%s", files[i].c_str());
                attroff(A_REVERSE);
            }
            else
            {
                mvprintw(i + 1, 0, "%s", files[i].c_str());
            }
        }

        choice = getch();
        std::string selected = files[highlight];

        switch (choice)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                --highlight;
            }
            break;

        case KEY_DOWN:
            if (highlight < n_choices - 1)
            {
                ++highlight;
            }
            break;

        case 10: /* Enter key */
            if (selected == "...")
            {
                size_t position = currentPath.find_last_of("/");
                if (position != std::string::npos)
                {
                    currentPath = currentPath.substr(0, position);
                    if (currentPath.empty())
                    {
                        currentPath = "/";
                    }
                    files = getFiles(currentPath);
                    highlight = 0;
                    n_choices = files.size();
                }
            }
            else
            {
                std::string fullPath = currentPath + "/" + selected;
                struct stat statbuf;
                if (stat(fullPath.c_str(), &statbuf) == 0)
                {
                    if (S_ISREG(statbuf.st_mode))
                    {
                        endwin();
                        return fullPath;
                    }
                    else if (S_ISDIR(statbuf.st_mode))
                    {
                        currentPath = fullPath;
                        files = getFiles(currentPath);
                        highlight = 0;
                        n_choices = files.size();
                    }
                }
            }
            break;

        default:
            break;
        }
    }

    endwin();
    return "";
}
/*End*/
