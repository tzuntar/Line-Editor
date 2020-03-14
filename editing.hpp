// Line Editor v1.2 by RedCreator37
// ncurses-based version
// ------------------------------------
// editing.hpp - editor routines header
// Thu 2019-07-25
#ifndef editing_hpp
#define editing_hpp

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <panel.h>
#include "display.hpp"

// editing
void append_line(WINDOW *window, std::vector<std::string> &textlines);
void edit_line(WINDOW *window, std::vector<std::string> &textlines);
void delete_line(WINDOW *window, std::vector<std::string> &textlines);
void delete_all(WINDOW *window, std::vector<std::string> &textlines);
void find_text(WINDOW *window, std::vector<std::string> &textlines);
std::vector<std::string> get_lines(const std::string &current_fname);
int save_lines(const std::vector<std::string> &textlines, const std::string &current_fname);

// general
int string_lnum(char *string, WINDOW *window, int y, int x, int max);
int find_string(const std::vector<std::string> &textlines, const std::string &query);

#endif
