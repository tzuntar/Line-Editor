// Line Editor v1.2 by RedCreator37
// ncurses-based verion
// ------------------------------------
// lineedit.cpp - main program file
// Thu 2019-07-25
#include "lineedit.hpp"
#include "editing.hpp"
using namespace lineeditor;

// Program entry point
int main(int argc, char* argv[]) {
    using namespace std;
    WINDOW* windows[3];
    PANEL* panels[3];

    // if the "list" option is specified just print the listing and quit
    if (argc == 3 && (strcmp(argv[2], "-l") == 0 || strcmp(argv[2], "--list") == 0)) {
        quick_listing(argv[1]);
        return 0;
    }

    // normal start
    if (argc < 2) {
        cout << "* Enter filename: ";
        cin >> filename;
    } else filename = argv[1];    

    if (filename == "") {
        cout << endl
             << "* No file specified." << endl;
        return 1;
    } else cout << endl
             << "* Line Editor v1.0 by RedCreator37" << endl
             << "* Loading file " << filename << endl
             << endl;

    lines = get_lines(filename);

    // I N I T I A L I Z A T I O N

    // initialize ncurses
    initscr();
    start_color();
    cbreak();
    noecho();

    // colors
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);  // inversed white on blue
    init_pair(4, COLOR_RED, COLOR_BLACK);   // used for error messages

    // create windows for panels
    windows[0] = newwin(LINES - 6, COLS, 0, 0); // main window
    windows[1] = newwin(5, COLS, LINES - 6, 0); // command window
    windows[2] = newwin(1, COLS, LINES, 0);     // status bar

    // create window borders
    box(windows[0], 0, 0);
    box(windows[1], 0, 0);

    // attact panels
    panels[0] = new_panel(windows[0]);
    panels[1] = new_panel(windows[1]);
    panels[2] = new_panel(windows[2]);
    update_panels();

    init_sbar(windows[2], filename);
    print_listing(windows[0], lines);

    // M A I N     C O M M A N D     L O O P

    bool exit = false;
    WINDOW* mainwin = windows[0];
    WINDOW* cmdwin = windows[1];
    stringstream stream;

    while (!exit) {
        // reinitialize the command and main window
        wmove(cmdwin, 1, 1);
        wattrset(cmdwin, A_NORMAL | COLOR_PAIR(1));
        wattrset(mainwin, A_BOLD);  // text will be bold while editing
        wprintw(cmdwin, "* Enter command (? for help):");
        wmove(cmdwin, 2, 1);
        wprintw(cmdwin, "> ");
        wrefresh(cmdwin);

        // reset the stringstream
        stream.str("");

        // get input and process commands
        int ch = getch();
        switch(ch) {
        case 'l': // full listing of the file
            print_refresh(cmdwin, "l");
            print_listing(mainwin, lines);
            break;

        case 'a': // append to the end of the file
            print_refresh(cmdwin, "a");
            wmove(mainwin, 1, 1);
            append_line(mainwin, lines);
            break;

        case 'e': // edit line
            print_refresh(cmdwin, "e");
            wmove(mainwin, 1, 1);
            edit_line(mainwin, lines);
            break;

        case 'f':   // find text
            print_refresh(cmdwin, "f");
            wmove(mainwin, 1, 1);
            find_text(mainwin, lines);
            break;

        case 'd': // delete line
            print_refresh(cmdwin, "d");
            wmove(mainwin, 1, 1);
            delete_line(mainwin, lines);
            break;

        case '\\': // delete all lines
            print_refresh(cmdwin, "\\");
            wmove(mainwin, 1, 1);
            delete_all(mainwin, lines);
            break;

        case '?': // help
            print_refresh(cmdwin, "?");
            clr_window(mainwin);
            print_help(mainwin, 1, 0);
            box(mainwin, 0, 0);
            wrefresh(mainwin);
            break;

        case 'o': { // open another file
            char* newname = new char[256];

            print_refresh(cmdwin, "o");
            wmove(cmdwin, 3, 1);
            wattron(cmdwin, A_BOLD);
            wprintw(cmdwin, "* Enter file to open: ");
            wmove(cmdwin, 3, 23);
            echo();
            wgetstr(cmdwin, newname);
            noecho();
            wattroff(cmdwin, A_BOLD);

            filename = newname;
            delete[] newname;
            lines = get_lines(filename);
            init_sbar(cmdwin, filename);
            print_listing(mainwin, lines);
            break;
        }

        case 'w': // write file
            print_refresh(cmdwin, "w");
            wmove(cmdwin, 3, 1);
            wprintw(cmdwin, "* Saving changes...");
            wrefresh(cmdwin);

            save_lines(lines, filename);

            // update command window status
            wmove(cmdwin, 3, 1);
            wattrset(cmdwin, COLOR_PAIR(2));
            wattron(cmdwin, A_BOLD);
            stream << "* Saved to file " << filename;
            for (int i = 0; i < filename.length() + 2; ++i)
                stream << " ";
            wprintw(cmdwin, stream.str().c_str());
            wattroff(cmdwin, A_BOLD);
            wrefresh(mainwin);
            break;

        case 'r': { // write to another file
            char* newname = new char[256];

            print_refresh(cmdwin, "r");
            wmove(cmdwin, 3, 1);
            wattron(cmdwin, A_BOLD);
            wprintw(cmdwin, "* Enter new filename: ");
            wmove(cmdwin, 3, 23);
            echo();
            wgetstr(cmdwin, newname);
            noecho();
            wattroff(cmdwin, A_BOLD);

            filename = newname;

            // save text to the specified file
            wmove(cmdwin, 3, 1);
            wprintw(cmdwin, "* Saving the text...");
            wrefresh(cmdwin);

            save_lines(lines, filename);

            // update command window status
            wmove(cmdwin, 3, 1);
            wattrset(cmdwin, COLOR_PAIR(2));
            wattron(cmdwin, A_BOLD);
            stream << "* Saved to file " << filename;
            for (int i = 0; i < filename.length() + 2; ++i)
                stream << " ";
            wprintw(cmdwin, stream.str().c_str());
            wattroff(cmdwin, A_BOLD);
            init_sbar(cmdwin, filename);
            wrefresh(mainwin);

            delete[] newname;
            break;
        }

        case 'x': // write file and exit
            print_refresh(cmdwin, "x");
            wmove(cmdwin, 3, 1);
            wprintw(cmdwin, "* Saving changes...");
            wrefresh(cmdwin);

            save_lines(lines, filename);
            exit = true;
            break;

        case 'q': // exit without saving
            print_refresh(cmdwin, "q");
            exit = true;
            break;
        }
    }

    // show everything on screen
    doupdate();

    // clean up and exit
    endwin();
    cout << "* Exiting." << endl;
    return 0;
}

/// Print out all the text in the file with line
/// numbers on the left to std::cout
void quick_listing(std::string fname) {
    std::vector<std::string> text;
    std::ifstream file(fname);

    if (file.is_open()) { // get the text
        std::string line;
        while (getline(file, line))
            text.push_back(line);
        file.close();
    }

    for (int i = 0; i < text.size(); ++i)
        std::cout << "[" << i + 1 << "] " << text[i] << std::endl;
}
