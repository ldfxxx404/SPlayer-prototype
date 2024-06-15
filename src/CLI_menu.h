#include <vector>
#include <iostream>
#include <ncurses.h>
#include "fileSelector.h"


std::string selectFile(const std::string &path)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    std::vector<std::string> files = browseFiles(path);

    int highlight = 0;
    int choice;
    int n_choices = files.size();

    while (1)
    {
        for (int i = 0; i < n_choices; ++i)
        {
            if (i == highlight)
            {
                attron(A_REVERSE);
                mvprintw(i, 0, "%s", files[i].c_str());
                attroff(A_REVERSE);
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
                if (highlight < n_choices - 1)
                {
                    ++highlight;
                }
                break;
            
            case 10: 
                return files[highlight];
            
            default:
                break;
            }
            
        }
        endwin();
        return "";
    }
}
