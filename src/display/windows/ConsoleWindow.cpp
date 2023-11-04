#include "ConsoleWindow.h"

namespace Editor {

    void ConsoleWindow::print(std::vector<std::vector<CursedString>> text) {
        int rowPos = 0;
        for (const auto &line: text) {
            int colPos = 0;
            for (const auto &textFragment: line) {
                attron(textFragment.attributes);
                mvwprintw(wHandle, rowPos, colPos, textFragment.string.c_str());
                attrset(A_NORMAL);
                colPos += textFragment.string.length();
            }
            rowPos++;
        }
    }

    std::string ConsoleWindow::readUserInput(int row, std::vector<CursedString> prompt) {
        int colPos = 0;
        for (const auto &textFragment: prompt) {
            attron(textFragment.attributes);
            mvwprintw(wHandle, row, colPos, textFragment.string.c_str());
            attrset(A_NORMAL);
            colPos += textFragment.string.length();
        }
        colPos += 1;

        int charsLeft = wSize.columns - colPos;
        char buffer[charsLeft + 1];
        mvwgetnstr(wHandle, row, colPos, buffer, charsLeft);
    }

} // Editor