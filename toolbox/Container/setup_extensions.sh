#!/bin/sh

code --install-extension cantonios.project-templates
code --install-extension dnut.rewrap-revived
code --install-extension filipkarlsson.cxx-tools
code --install-extension fleexo.cpp-class-creator
code --install-extension mhutchie.git-graph
code --install-extension ms-vscode.cmake-tools
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cpptools-extension-pack
code --install-extension ms-vscode.cpptools-themes
code --install-extension theumletteam.umlet

tar -C / -zxvf /install/code-templates.tgz

