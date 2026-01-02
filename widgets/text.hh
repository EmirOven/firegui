#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "../primitives/font.hh"
#include "widgets.hh"

class Text : public Widget {
 private:
  TTF_Font* _font;
  std::string content;

  SDL_Color color = {255, 255, 255, 255};

 public:
  int renderContent(TTF_TextEngine* textEngine) override;
  int setText(std::string newText);
  Text(std::string name, std::string content, TTF_Font* font, SDL_Color color);
  ~Text() override = default;
};

Text::Text(std::string name, std::string content, TTF_Font* font, SDL_Color color = {255, 255, 255, 255}) : Widget(name) {
  this->color = color;
  this->content = content;
  this->_font = font;
}
