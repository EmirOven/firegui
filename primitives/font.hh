#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <map>
#include <string>
#include <vector>

std::map<std::string, int> styleAliases = {
    {"Bold", TTF_STYLE_BOLD},
    {"Italic", TTF_STYLE_ITALIC},
    {"Underline", TTF_STYLE_UNDERLINE},
    {"Strikethrough", TTF_STYLE_STRIKETHROUGH},
    {"Regular", TTF_STYLE_NORMAL},
};

struct font {
  std::string path;
  std::string name;
  std::vector<std::string> availableStyles;
};
