#include "Window.h"
#include "../display.hpp"

namespace Editor {

    Window::Window(Editor::WindowSize size, Editor::WindowPos pos, std::string title,
                   unsigned int initialBufSize, bool drawBorder) {
        wSize = size;
        wPos = pos;
        wTitle = std::move(title);
        buffer = std::vector<std::string>(initialBufSize);

        wHandle = newwin(wSize.lines, wSize.columns,
                         wPos.column, wPos.row);
        if (wHandle == nullptr) {
            exit(-1);   // todo: propagate the error instead
        }
        hasBorder = drawBorder;
        if (hasBorder)
            box(wHandle, 0, 0);
        pHandle = new_panel(wHandle);
        if (pHandle == nullptr) {
            exit(-1);   // todo: propagate the error instead
        }
        refresh();
    }

    void Window::move(Editor::WindowPos newPos) {
        wPos = newPos;
        move_panel(pHandle, wPos.column, wPos.row);
        refresh();
    }

    void Window::resize(Editor::WindowSize newSize) {
        wSize = newSize;
        wresize(wHandle, wSize.lines, wSize.columns);
        replace_panel(pHandle, wHandle);
        refresh();
    }

    void Window::setVisibility(bool visible) {
        panelVisible = visible;
        if (panelVisible) {
            show_panel(pHandle);
        } else {
            hide_panel(pHandle);
        }
        refresh();
    }

    void Window::clear() {
        wclear(wHandle);
        if (hasBorder)
            box(wHandle, 0, 0);
        wrefresh(wHandle);

        // ToDo: test if update_panels(); handles the boxing & refreshing automatically
        update_panels();
    }

    // Protected

    void Window::refresh() {
        update_panels();
    }

} // Editor