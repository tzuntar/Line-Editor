#ifndef LINEEDIT_TEXTWINDOW_H
#define LINEEDIT_TEXTWINDOW_H

#include <utility>

#include "../Window.h"

namespace Editor {

    class TextWindow : public Window {
    public:
        TextWindow(WindowSize size, WindowPos pos, std::string title = "Editor") :
                Window(size, pos, std::move(title), true) {}

    };

} // Editor

#endif //LINEEDIT_TEXTWINDOW_H
