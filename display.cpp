// Line Editor v1.2 by RedCreator37
// ncurses-based verion
// ------------------------------------
// display.cpp - output management
// Thu 2019-07-25
#include "display.hpp"

/// Clear the specified window
void clr_window(WINDOW* window) {
    wclear(window);
    box(window, 0, 0);
    wrefresh(window);
}

/// Print the text to a window and redraw its border
void print_wb(WINDOW* window, const char* text) {
    wprintw(window, text);
    box(window, 0, 0);
    wrefresh(window);
}

/// Print the text to a window and refresh the window
void print_refresh(WINDOW* window, const char* text) {
    wprintw(window, text);
    wrefresh(window);
}

/// Print an error message to a window
/// Ints are the coords of the message on the screen
void print_error(WINDOW* window, const char* text, int y, int x) {
    attron(COLOR_PAIR(4));
    mvprintw(y, x, text);
    attroff(COLOR_PAIR(4));
    wrefresh(window);
}

/// Print listing of the current file to a specified window
void print_listing(WINDOW* window, std::vector<std::string>& lines) {
    wmove(window, 1, 1);
    int loc = 1;

    clr_window(window);
    for (int i = 0; i < lines.size(); ++i) {
        if (loc == LINES - 8) {   // we are at the end of the screen
            attron(A_REVERSE | COLOR_PAIR(1));
            mvprintw(LINES - 8, 1, "> Press any key <");
            attroff(A_REVERSE | COLOR_PAIR(1));
            getch();
            clr_window(window);

            loc = 1;    // reset location
            --i;
        } else {    // not at the end yet, print the next line
            std::stringstream stream;
            stream << "[" << i + 1 << "] ";

            attron(COLOR_PAIR(2));
            mvprintw(loc, 1, stream.str().c_str());
            attroff(COLOR_PAIR(2));

            mvprintw(loc, stream.str().length() + 1, lines[i].c_str());
            ++loc;
        }
        wrefresh(window);
    }
}

/// Display the status bar on the screen
void init_sbar(WINDOW* window, std::string filename) {
    std::stringstream statusbar;
    std::string text = "Line Editor v1.2 by RedCreator37";
    int offset = text.length() + 6 + filename.length()
        + statusbar.str().length();

    statusbar << "File: " << filename;
    attron(A_BOLD | COLOR_PAIR(3));
    mvprintw(LINES - 1, 0, statusbar.str().c_str());

    // fill the status bar with spaces to make it fully colored
    for (int i = 0; i < COLS - offset; ++i) printw(" ");
    printw(text.c_str());
    attroff(A_BOLD | COLOR_PAIR(3));
}
