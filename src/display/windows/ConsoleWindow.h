#ifndef LINEEDIT_CONSOLEWINDOW_H
#define LINEEDIT_CONSOLEWINDOW_H

#include "../Window.h"
#include "../Utilities.h"

namespace Editor {

    class ConsoleWindow : public Window {
    public:

        ConsoleWindow(Editor::WindowSize size, Editor::WindowPos pos, bool drawBorder = true) :
                Editor::Window(size, pos, "Console", 10, drawBorder) {}

        void print(std::vector<std::vector<CursedString>> text);

        std::string readUserInput(int row, std::vector<CursedString> prompt);

    };

} // Editor

#endif //LINEEDIT_CONSOLEWINDOW_H
