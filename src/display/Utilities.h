#ifndef LINEEDIT_UTILITIES_H
#define LINEEDIT_UTILITIES_H

#include <string>

namespace Editor {

    struct CursedString {
        std::string string;
        int attributes = 0;;
    };

} // Editor

#endif //LINEEDIT_UTILITIES_H
