# Line Editor

This repository contains the source code for a CLI text editor called
Line Editor.

*see the [features/ui_rewrite](https://github.com/tzuntar/Line-Editor/tree/feature/ui_rewrite) branch for a development version with a much more versatile TUI.*

## Requirements

 - Working C++ compiler (preferably g++), `make`
 - Ncurses and its panels library
 - Terminal with color support

## Compiling

To compile the program, just run `make`. You can also run `make
run` to automatically start the program afterwards.

When the compilation is done, you can start the editor with
`./lineedit filename` (supply a new filename to create
a blank file).

To install the program  and have it available everywhere, run `sudo make install`. Same for uninstalling it,
`sudo make uninstall`.

## Known bugs

 - There are currently some artificial line number size limitations

## Screenshot

![Line Editor Screenshot](https://user-images.githubusercontent.com/35228139/75629682-47977200-5be4-11ea-8149-6326d38d0f81.png)

Here's `lineedit` running on a live system:
![lineedit](https://github.com/tzuntar/Line-Editor/assets/35228139/c34098d9-ea70-4b5c-8a64-f4066a6597d4)
