#include "text.hh"

#include <SDL3_ttf/SDL_ttf.h>

int Text::renderContent(TTF_TextEngine* textEngine) {
  TTF_Text* textContent = TTF_CreateText(textEngine, this->_font, this->content.c_str(), 0);
  if (textContent == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create text: %s\n", SDL_GetError());
    return 1;
  }

  TTF_SetTextColor(textContent, this->color.r, this->color.g, this->color.b, this->color.a);

  TTF_DrawRendererText(textContent, 20, 20);

  return 0;  // Placeholder return value
}

int Text::setText(std::string newText) {
  this->content = newText;
  return 0;  // Placeholder return value
}