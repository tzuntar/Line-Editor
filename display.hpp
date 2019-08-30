// Line Editor v1.2 by RedCreator37
// ncurses-based verion
// ------------------------------------
// display.hpp - output management header
// Thu 2019-07-25
#ifndef display_hpp
#define display_hpp

#include <vector>
#include <string>
#include <sstream>

#include <panel.h>

// functions
void clr_window(WINDOW* window);
void print_wb(WINDOW* window, const char* text);
void print_refresh(WINDOW* window, const char* text);
void print_error(WINDOW* window, const char* text, int y, int x);
void print_listing(WINDOW* window, std::vector<std::string>& lines);
void print_help(WINDOW* window, int y, int x);
void init_sbar(WINDOW* window, std::string filename);

#endif
