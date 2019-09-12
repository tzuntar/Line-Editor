// Line Editor v1.2 by RedCreator37
// ncurses-based verion
// ------------------------------------
// lineedit.hpp - main headerfile
// Thu 2019-07-25
#ifndef lineedit_hpp
#define lineedit_hpp

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

#include <panel.h>
#include "display.hpp"

namespace lineeditor {
    std::vector<std::string> lines;
    std::string filename;
}

void quick_listing(const std::string& fname);

#endif
