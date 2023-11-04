// Line Editor v1.2 by RedCreator37
// ncurses-based version
// ------------------------------------
// lineedit.cpp - main program file
// Thu 2019-07-25
#include "lineedit.hpp"
#include "editing.hpp"
#include "display/windows/TextWindow.h"
#include "display/windows/ConsoleWindow.h"

/// Program entry point
int main(int argc, char *argv[]) {
    using namespace std;

    // if the "list" option is specified just print the listing and quit
    if (argc == 3 && (strcmp(argv[2], "-l") == 0 || strcmp(argv[2], "--list") == 0)) {
        quick_listing(argv[1]);
        return 0;
    }

    // normal entry point
    string filename;
    if (argc < 2) {
        cout << "* Enter filename: ";
        cin >> filename;
    } else filename = argv[1];

    if (filename.empty()) {
        cout << endl << "* No file specified. Exiting." << endl;
        return 1;
    }

    TextFile textFile = TextFile(get_lines(filename), filename);
    Editor::GlobalInit(textFile);
    Editor::GlobalLoop();
    return 0;
}

namespace Editor {

    void GlobalInit(TextFile &file) {
        initscr();
        start_color();
        cbreak();
        noecho();

        // colors
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLUE);  // inverted white on blue
        init_pair(4, COLOR_RED, COLOR_BLACK);   // used for error messages

        auto editorWindow = new TextWindow({LINES - 6, COLS}, {0, 0});
        auto consoleWindow = new ConsoleWindow({5, COLS}, {LINES - 6, 0});
        activeWindows[0] = editorWindow;
        activeWindows[1] = consoleWindow;

        // init_statusbar(file.get_fname(), 0);
        // print_listing(windows[0], file.get_lines());
    }

    void GlobalLoop() {

    }

}   // !Editor

/// The main command loop
void loop(WINDOW **windows, TextFile &file) {
    bool exit = false;
    WINDOW *mainwin = windows[0], *cmdwin = windows[1];
    std::stringstream stream;

    while (!exit) {
        init_statusbar(file.get_fname(), file.get_lines().size());
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

        switch (tolower(ch)) {
            case 'l': // full listing of the file
                print_listing(mainwin, file.get_lines());
                break;

            case 'a': // append to the end of the file
                append_line(mainwin, file.get_lines());
                break;

            case 'e': // edit a line
                edit_line(mainwin, file.get_lines());
                break;

            case 'i': // insert lines
                insert_line(mainwin, file.get_lines());
                break;

            case 'f': // find text in file
                find_text(mainwin, file.get_lines());
                break;

            case 'd': // delete a line
                delete_line(mainwin, file.get_lines());
                break;

            case '\\': // delete all lines
                delete_all(mainwin, file.get_lines());
                break;

            case '?': // display help
                clr_window(mainwin);
                print_help(mainwin);
                box(mainwin, 0, 0);
                wrefresh(mainwin);
                break;

            case 'o': { // open a file
                char *newname = new char[256]();

                // clear any possible text on that line
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);

                // now print the prompt
                wmove(cmdwin, 3, 1);
                wattron(cmdwin, A_BOLD);
                wprintw(cmdwin, "* Enter filename to read: ");
                wmove(cmdwin, 3, 23);
                echo();
                wgetstr(cmdwin, newname);
                noecho();
                wattroff(cmdwin, A_BOLD);

                file.set_fname(newname);
                delete[] newname;
                file.set_lines(get_lines(file.get_fname()));
                print_listing(mainwin, file.get_lines());
                break;
            }

            case 'w': // save the text to a file
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wprintw(cmdwin, "* Saving changes...");
                wrefresh(cmdwin);

                save_lines(file.get_lines(), file.get_fname());

                // update command window status
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wattrset(cmdwin, COLOR_PAIR(2));
                wattron(cmdwin, A_BOLD);
                stream << "* Saved to file " << file.get_fname();
                for (unsigned int i = 0; i < file.get_fname().length() + 2; ++i)
                    stream << " ";
                wprintw(cmdwin, stream.str().c_str());
                wattroff(cmdwin, A_BOLD);
                wrefresh(mainwin);
                break;

            case 'r': { // save to another file
                char *newname = new char[256]();

                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wattron(cmdwin, A_BOLD);
                wprintw(cmdwin, "* Enter a new filename: ");
                wmove(cmdwin, 3, 25);
                echo();
                wgetstr(cmdwin, newname);
                noecho();
                wattroff(cmdwin, A_BOLD);

                file.set_fname(newname);

                // save text to the specified file
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wprintw(cmdwin, "* Saving the file...");
                wrefresh(cmdwin);

                save_lines(file.get_lines(), file.get_fname());

                // update command window status
                wmove(cmdwin, 3, 1);
                clr_line(cmdwin);
                wmove(cmdwin, 3, 1);
                wattrset(cmdwin, COLOR_PAIR(2));
                wattron(cmdwin, A_BOLD);
                stream << "* Saved to file " << file.get_fname();
                for (unsigned int i = 0; i < file.get_fname().length() + 2; ++i)
                    stream << " ";
                wprintw(cmdwin, stream.str().c_str());
                wattroff(cmdwin, A_BOLD);
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

                save_lines(file.get_lines(), file.get_fname());
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
    std::cout << "* Exiting." << std::endl;
}

/// Prints all text in the file with line numbers on the left
/// to the standard output
void quick_listing(const std::string &filename) {
    std::vector<std::string> text;
    std::ifstream file(filename);

    if (file.is_open()) { // get the text
        std::string line;
        while (getline(file, line)) text.push_back(line);
        file.close();
    }

    for (unsigned long i = 0; i < text.size(); ++i)
        std::cout << "[" << i + 1 << "] " << text[i] << std::endl;
}
