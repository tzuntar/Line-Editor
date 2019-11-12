// Line Editor v1.2 by RedCreator37
// ncurses-based version
// ------------------------------------
// display.cpp - output management
// Thu 2019-07-25
#include "display.hpp"

/// Clear the specified window
void clr_window(WINDOW *window) {
    wclear(window);
    box(window, 0, 0);
    wrefresh(window);
}

// clear any previous text on the line (with -2 offset)
void clr_line(WINDOW *window) {
    for (int i = 0; i < COLS - 2; i++) wprintw(window, " ");
    wrefresh(window);
}

/// Print the text to a window and redraw its border
void print_wb(WINDOW *window, const char *text) {
    wprintw(window, text);
    box(window, 0, 0);
    wrefresh(window);
}

/// Print the text to a window and refresh the window
void print_refresh(WINDOW *window, const char *text) {
    wprintw(window, text);
    wrefresh(window);
}

/// Print an error message to a window
/// Ints are the coords of the message on the screen
void print_error(WINDOW *window, const char *text, int y, int x) {
    attron(COLOR_PAIR(4));
    mvprintw(y, x, text);
    attroff(COLOR_PAIR(4));
    wrefresh(window);
}

/// Print listing of the current file to a specified window
void print_listing(WINDOW *window, std::vector<std::string> &lines) {
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

/// Print the in-program help to the window on specified location
void print_help(WINDOW *window, int y, int x) {
    std::stringstream stream;
    wmove(window, y, x);
    stream << " * Line Editor Help" << std::endl
           << " ----------------------------------------------------" << std::endl
           << "   A V A I L A B L E        C O M M A N D S" << std::endl
           << " ----------------------------------------------------" << std::endl
           << " l - display full file listing" << std::endl
           << " a - append text to the end of the file" << std::endl
           << " e - edit a specific line" << std::endl
           << " f - find text in file" << std::endl
           << " d - delete a specific line" << std::endl
           << " \\ - delete all lines (irreversible!)" << std::endl
           << " o - open another file (discards current changes)" << std::endl
           << " w - write changes to file (save)" << std::endl
           << " r - write changes to another file (save as)" << std::endl
           << " x - write changes to file and exit" << std::endl
           << " q - exit without saving changes" << std::endl
           << " ? - display this page" << std::endl
           << " ----------------------------------------------------" << std::endl
           << " Line Editor v1.2 by RedCreator37" << std::endl
           << " WARNING: This program is distributed \"as is\" and" << std::endl
           << "          comes without warranty of any kind! You" << std::endl
           << "          are using it at your own risk!";
    wprintw(window, stream.str().c_str());
}

/// Display the status bar on the screen
void init_sbar(const std::string &filename) {
    std::stringstream statusbar;
    std::string text = "Line Editor v1.2 by RedCreator37";
    int offset = text.length() + 6 + filename.length() + statusbar.str().length();

    statusbar << "File: " << filename;
    attron(A_BOLD | COLOR_PAIR(3));
    mvprintw(LINES - 1, 0, statusbar.str().c_str());

    // fill the status bar with spaces to make it fully colored
    for (int i = 0; i < COLS - offset; ++i) printw(" ");
    printw(text.c_str());
    attroff(A_BOLD | COLOR_PAIR(3));
}
