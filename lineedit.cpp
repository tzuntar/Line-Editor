// Line Editor v1.2 by RedCreator37
// ncurses-based version
// ------------------------------------
// lineedit.cpp - main program file
// Thu 2019-07-25
#include "lineedit.hpp"
#include "editing.hpp"

/// Program entry point
int main(int argc, char *argv[]) {
    using namespace std;
    WINDOW *windows[3];
    PANEL *panels[3];

    // if the "list" option is specified just print the listing and quit
    if (argc == 3 && (strcmp(argv[2], "-l") == 0 || strcmp(argv[2], "--list") == 0)) {
        quick_listing(argv[1]);
        return 0;
    }

    // normal entry point
    std::string filename;
    if (argc < 2) {
        cout << "* Enter filename: ";
        cin >> filename;
    } else filename = argv[1];

    if (filename.empty()) {
        cout << endl << "* No file specified." << endl;
        return 1;
    } else {
        cout << endl << "* Line Editor v1.2 by RedCreator37" << endl
             << "* Loading file " << filename << endl << endl;
    }

    TextFile textFile = TextFile(get_lines(filename), filename);
    initialize(windows, panels, textFile);
    loop(windows, textFile);
    return 0;
}

/// Initialization
void initialize(WINDOW** windows, PANEL** panels, TextFile& textFile) {
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

    // attach panels
    for (unsigned int i = 0; i < sizeof(panels) / sizeof(*panels); ++i)
        panels[i] = new_panel(windows[i]);
    update_panels();

    init_sbar(textFile.get_fname());
    print_listing(windows[0], textFile.get_lines());

}

/// The main command loop
void loop(WINDOW** windows, TextFile& textFile) {
    using namespace std;
    bool exit = false;
    WINDOW *mainwin = windows[0], *cmdwin = windows[1];
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

        // reset the string stream
        stream.str("");

        // get input and process commands
        char ch = getch();
        print_refresh(cmdwin, &ch);
        wmove(mainwin, 1, 1);

        switch (ch) {
            case 'l': // full listing of the file
                print_listing(mainwin, textFile.get_lines());
                break;

            case 'a': // append to the end of the file
                append_line(mainwin, textFile.get_lines());
                break;

            case 'e': // edit a line
                edit_line(mainwin, textFile.get_lines());
                break;

            case 'f': // find text in file
                find_text(mainwin, textFile.get_lines());
                break;

            case 'd': // delete a line
                delete_line(mainwin, textFile.get_lines());
                break;

            case '\\': // delete all lines
                delete_all(mainwin, textFile.get_lines());
                break;

            case '?': // display help
                clr_window(mainwin);
                print_help(mainwin, 1, 0);
                box(mainwin, 0, 0);
                wrefresh(mainwin);
                break;

            case 'o': { // open a file
                char *newname = new char[256];

                // clear any possible text on that line
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);

                // now print the prompt
                wmove(cmdwin, 3, 1);
                wattron(cmdwin, A_BOLD);
                wprintw(cmdwin, "* Enter file to open: ");
                wmove(cmdwin, 3, 23);
                echo();
                wgetstr(cmdwin, newname);
                noecho();
                wattroff(cmdwin, A_BOLD);

                textFile.set_fname(newname);
                delete[] newname;
                textFile.set_lines(get_lines(textFile.get_fname()));
                init_sbar(textFile.get_fname());
                print_listing(mainwin, textFile.get_lines());
                break;
            }

            case 'w': // save the text to a file
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wprintw(cmdwin, "* Saving changes...");
                wrefresh(cmdwin);

                save_lines(textFile.get_lines(), textFile.get_fname());

                // update command window status
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wattrset(cmdwin, COLOR_PAIR(2));
                wattron(cmdwin, A_BOLD);
                stream << "* Saved to file " << textFile.get_fname();
                for (unsigned int i = 0; i < textFile.get_fname().length() + 2; ++i) stream << " ";
                wprintw(cmdwin, stream.str().c_str());
                wattroff(cmdwin, A_BOLD);
                wrefresh(mainwin);
                break;

            case 'r': { // save to another file
                char *newname = new char[256];

                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wattron(cmdwin, A_BOLD);
                wprintw(cmdwin, "* Enter new filename: ");
                wmove(cmdwin, 3, 23);
                echo();
                wgetstr(cmdwin, newname);
                noecho();
                wattroff(cmdwin, A_BOLD);

                textFile.set_fname(newname);

                // save text to the specified file
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wprintw(cmdwin, "* Saving the text...");
                wrefresh(cmdwin);

                save_lines(textFile.get_lines(), textFile.get_fname());

                // update command window status
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wattrset(cmdwin, COLOR_PAIR(2));
                wattron(cmdwin, A_BOLD);
                stream << "* Saved to file " << textFile.get_fname();
                for (unsigned int i = 0; i < textFile.get_fname().length() + 2; ++i) stream << " ";
                wprintw(cmdwin, stream.str().c_str());
                wattroff(cmdwin, A_BOLD);
                init_sbar(textFile.get_fname());
                wrefresh(mainwin);

                delete[] newname;
                break;
            }

            case 'x': // save the file and exit
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wprintw(cmdwin, "* Saving changes...");
                wrefresh(cmdwin);

                save_lines(textFile.get_lines(), textFile.get_fname());
                exit = true;
                break;

            case 'q': // exit without saving
                exit = true;
                break;
        }
    }

    // show everything on screen
    doupdate();

    // clean up and exit
    endwin();
    cout << "* Exiting." << endl;
}

/// Print all the text in the file with line numbers at the left
/// to the standard output
void quick_listing(const std::string &fname) {
    std::vector<std::string> text;
    std::ifstream file(fname);

    if (file.is_open()) { // get the text
        std::string line;
        while (getline(file, line)) text.push_back(line);
        file.close();
    }

    for (unsigned long i = 0; i < text.size(); ++i)
        std::cout << "[" << i + 1 << "] " << text[i] << std::endl;
}
