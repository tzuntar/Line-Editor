// Line Editor v1.2 by RedCreator37
// ncurses-based version
// ------------------------------------
// display.cpp - output management
// Thu 2019-07-25
#include "lineedit.hpp"

/// Clears this window
void clr_window(WINDOW *window) {
    wclear(window);
    box(window, 0, 0);
    wrefresh(window);
}

/// Clears any previous text on the line (with -2 offset)
void clr_line(WINDOW *window) {
    for (int i = 0; i < COLS - 2; i++) wprintw(window, " ");
    wrefresh(window);
}

/// Prints the text to this window and redraws its border
void print_border(WINDOW *window, const char *text) {
    wprintw(window, text);
    box(window, 0, 0);
    wrefresh(window);
}

/// Prints the text to this window and refreshes it
void print_refresh(WINDOW *window, const char *text) {
    wprintw(window, text);
    wrefresh(window);
}

/// Prints an error message to this window
/// Ints are the coords of the message on the screen
void print_error(WINDOW *window, const char *text, int y, int x) {
    attron(COLOR_PAIR(4));
    mvprintw(y, x, text);
    attroff(COLOR_PAIR(4));
    wrefresh(window);
}

/// Prints the listing to the current file
/// This function is implementation specific, use print_listing instead
void listing(WINDOW *window, std::vector<std::string> &lines, bool mark, unsigned int line) {
    wmove(window, 1, 1);
    int loc = 1;

    clr_window(window);
    for (unsigned long i = 0; i < lines.size(); ++i) {
        if (loc == LINES - 8) {   // we are at the end of the screen
            attron(A_REVERSE | COLOR_PAIR(1));
            mvprintw(LINES - 8, 1, "> Press any key <");
            attroff(A_REVERSE | COLOR_PAIR(1));
            getch();
            clr_window(window);

            loc = 1;    // reset the location
            --i;
        } else {    // not at the end yet, print the next line
            std::stringstream ss;
            ss << "[" << i + 1 << "] ";
            if (mark && i == line) {
                attron(COLOR_PAIR(4));
                mvprintw(loc, 1, ss.str().c_str());
                attroff(COLOR_PAIR(4));
            } else {
                attron(COLOR_PAIR(2));
                mvprintw(loc, 1, ss.str().c_str());
                attroff(COLOR_PAIR(2));
            }
            mvprintw(loc, static_cast<int>(ss.str().length() + 1), lines[i].c_str());
            ++loc;
        }
        wrefresh(window);
    }
}

/// Prints listing of the current file to this window
void print_listing(WINDOW *window, std::vector<std::string> &lines) {
    listing(window, lines, false, 0);
}

/// Prints listing of the current file to this window,
/// highlighting the specified line
void print_listing(WINDOW *window, std::vector<std::string> &lines, unsigned int marked) {
    listing(window, lines, true, marked);
}

/// Prints the in-program help to this window
void print_help(WINDOW *window) {
    std::vector<std::string> help;  // vvv + 1 space on the left because of the number column
    help.emplace_back("  * Line Editor Help                           General");
    help.emplace_back("  ----------------------------------------------------");
    help.emplace_back("    A V A I L A B L E        C O M M A N D S");
    help.emplace_back("  ----------------------------------------------------");
    help.emplace_back("  l - display the full file listing");
    help.emplace_back("  a - append text to the end of the file");
    help.emplace_back("  e - edit a specific line");
    help.emplace_back("  i - insert lines after a specific line");
    help.emplace_back("  f - find text in the file");
    help.emplace_back(" d - delete a specific line");
    help.emplace_back(" \\ - delete all lines (irreversible!)");
    help.emplace_back(" o - open another file (discards current changes)");
    help.emplace_back(" w - write changes to the file (save)");
    help.emplace_back(" r - write changes to another file (save as)");
    help.emplace_back(" x - write changes to the file and exit");
    help.emplace_back(" q - exit without saving changes");
    help.emplace_back(" ? - display this page");
    help.emplace_back(" ----------------------------------------------------");
    help.emplace_back(" " + TextFile::get_version());
    help.emplace_back(" WARNING: This program is distributed \"as is\" and");
    help.emplace_back("          comes without warranty of any kind. You");
    help.emplace_back("          are using it at your own risk!");
    help.emplace_back(" ----------------------------------------------------");
    print_listing(window, help);
}

/// Displays the status bar on the screen
void init_statusbar(const std::string &filename, int num_lines) {
    std::stringstream statusbar;
    unsigned int offset = TextFile::get_version().length() + 6 + filename.length()
                          + statusbar.str().length() + 9
                          + (std::to_string(num_lines).length());

    statusbar << "File: " << filename << " | " << num_lines << " lines";
    attron(A_BOLD | COLOR_PAIR(3));
    mvprintw(LINES - 1, 0, statusbar.str().c_str());

    // fill the status bar with spaces to make it fully colored
    for (unsigned int i = 0; i < COLS - offset; ++i) printw(" ");
    printw(TextFile::get_version().c_str());
    attroff(A_BOLD | COLOR_PAIR(3));
}
