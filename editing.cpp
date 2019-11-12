// Line Editor v1.2 by RedCreator37
// ncurses-based version
// ------------------------------------
// editing.cpp - editor routines
// Thu 2019-07-25
#include "editing.hpp"

/// Append a line to the end of the file
void append_line(WINDOW *window, std::vector<std::string> &textlines) {
    using namespace std;
    stringstream output;
    char *text = new char[2048];
    unsigned int num = textlines.size() + 1;

    // print the prompt and last line
    clr_window(window);
    output << endl << " * Enter text to append:" << endl
           << " [" << num - 1 << "] " << textlines[num - 2] << endl;
    wmove(window, 0, 0);
    print_wb(window, output.str().c_str());
    output.str("");

    // print the input line
    output << " [" << num << "] ";
    wmove(window, 3, 0);
    print_wb(window, output.str().c_str());

    // get the input
    echo();
    int offset = output.str().length();
    wmove(window, 3, offset);
    wgetstr(window, text);
    noecho();

    // finish up
    textlines.emplace_back(text);
    delete[] text;
}

/// Edit a specific line
void edit_line(WINDOW *window, std::vector<std::string> &textlines) {
    using namespace std;
    char *linenum = new char[10];
    char *text = new char[2048];
    stringstream output;

    // print the first prompt
    clr_window(window);
    output << " * Enter line number [NUMBERS ONLY!]: ";
    wmove(window, 1, 0);
    print_wb(window, output.str().c_str());

    // get line number input
    echo();
    wmove(window, 1, output.str().length());
    wgetstr(window, linenum);
    int num = string_lnum(linenum, window, 3, 0, textlines.size());

    if (num > 0) {
        // print the second prompt and current text in the specified line
        output.str("");
        output << " * Enter the text (1 line!):" << endl
               << "   [" << linenum << "] " << textlines[num - 1] << endl;
        print_wb(window, output.str().c_str());
        output.str("");

        // print the input prompt and get the text
        output << "   [" << linenum << "] ";
        int offset = output.str().length();
        print_wb(window, output.str().c_str());
        wmove(window, 4, offset);
        wgetstr(window, text);
        noecho();

        textlines[num - 1] = text;
    }

    // clean up
    delete[] text;
    delete[] linenum;
}

/// Delete a specified line
void delete_line(WINDOW *window, std::vector<std::string> &textlines) {
    using namespace std;
    char *linenum = new char[10];

    clr_window(window);
    wmove(window, 1, 0);
    print_wb(window, " * Enter line number: ");

    // get the input
    echo();
    wmove(window, 1, 22);
    wgetstr(window, linenum);
    noecho();

    // do the processing
    int num = string_lnum(linenum, window, 3, 0, textlines.size());
    if (num > textlines.size()) return; // safety check to avoid segfaults
    textlines.erase(textlines.begin() + (num - 1));

    // don't return an empty vector
    if (textlines.empty()) textlines.emplace_back("");
    delete[] linenum;
}

/// Delete all lines
void delete_all(WINDOW *window, std::vector<std::string> &textlines) {
    using namespace std;
    stringstream output;

    // print the message with a blinking "Warning" text
    // (hence all these cursor position calls)
    clr_window(window);
    wmove(window, 1, 0);
    print_wb(window, " * ");
    wattron(window, A_BLINK);
    wmove(window, 1, 3);
    print_wb(window, "WARNING:");
    wattroff(window, A_BLINK);
    wmove(window, 1, 11);
    output << " this will delete ALL lines in the current file!" << endl
           << " * This operation is irreversible! Enter [Y]es/[N]o: ";
    print_wb(window, output.str().c_str());
    wmove(window, 2, 53);

    // get the input
    echo();
    char choice = wgetch(window);
    noecho();

    // clear the array if confirmed
    if (tolower(choice) == 'y') {
        textlines.clear();
        textlines.emplace_back("");
    }
}

/// Find a string in the file
/// TODO: not working properly
void find_text(WINDOW *window, std::vector<std::string> &textlines) {
    char *text = new char[1024];
    clr_window(window);
    wmove(window, 1, 0);
    print_wb(window, " * Enter the text to look for: ");

    // get the input
    echo();
    wmove(window, 1, 31);
    wgetstr(window, text);
    noecho();

    // do the processing
    wmove(window, 2, 0);
    if (std::find(textlines.begin(), textlines.end(), text) != textlines.end())
        print_wb(window, "\n > The specified text has been found in file.");
    else
        print_wb(window, "\n > The specified text hasn't been found in file.");
    delete[] text;
}

/// Get lines from a text file
std::vector<std::string> get_lines(const std::string &current_fname) {
    using namespace std;
    vector<string> contents;

    ifstream file(current_fname);
    if (file.is_open()) { // get the lines
        string line;
        while (getline(file, line)) contents.push_back(line);
        file.close();
    }

    // add an empty line to avoid a segfault on empty files
    if (contents.empty()) contents.emplace_back("");
    return contents;
}

/// Save lines to the file
int save_lines(const std::vector<std::string> &textlines, const std::string &current_fname) {
    using namespace std;
    ofstream file(current_fname);

    if (file.is_open()) {
        for (const auto &line : textlines)
            file << line << endl;
        file.close();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

/// Try to convert a string to a line number (a positive integer).
/// Print out an error if the conversion fails or the number isn't
/// a valid line number.
/// Parameters: char* string, WINDOW* window to print the possible
/// errors on; int y and x - coords of the message on the screen;
/// int max - the number of lines
int string_lnum(char *string, WINDOW *window, int y, int x, int max) {
    std::stringstream intstream(string);
    int num;
    intstream >> num;
    if (intstream.fail() || num < 1 || num > max) {
        print_error(window, "* ERROR: invalid line number.", y, x + 1);
        return -1;
    } else return num;
}
