#ifndef LINEEDIT_WINDOW_H
#define LINEEDIT_WINDOW_H

#include <string>
#include <vector>
#include <panel.h>

namespace Editor {

    struct WindowSize {
        int lines;
        int columns;
    };

    struct WindowPos {
        int row;
        int column;
    };

    class Window {
    public:

        void move(WindowPos newPos);

        void resize(WindowSize newSize);

        void setVisibility(bool visible);

        void clear();

        WINDOW* getWindowHandle() { return wHandle; }

        PANEL* getPanelHandle() { return pHandle; }

        Window(WindowSize size, WindowPos pos, std::string title,
               unsigned int initialBufSize = 1024, bool drawBorder = false);

    protected:
        WINDOW *wHandle;
        PANEL *pHandle;
        WindowSize wSize;
        WindowPos wPos;
        std::string wTitle;
        std::vector<std::string> buffer;
        bool panelVisible = true;
        bool hasBorder = false;

        static void refresh();
    };

} // Editor

#endif //LINEEDIT_WINDOW_H
