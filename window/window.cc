#include "window.hh"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../primitives/font.hh"
#include "../widgets/widgets.hh"

int Window::create(std::string title, int width, int height) {
  SDL_Init(SDL_INIT_VIDEO);  // Initialize SDL3

  this->window = SDL_CreateWindow(
      title.c_str(),
      width,
      height,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN | SDL_WINDOW_HIGH_PIXEL_DENSITY);

  if (this->window == NULL) {
    // In the case that the window could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  this->renderer = SDL_CreateRenderer(this->window, nullptr);

  if (this->renderer == NULL) {
    // In the case that the renderer could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    return 1;
  }

  if (!SDL_SetRenderVSync(this->renderer, 1)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not set VSync: %s\n", SDL_GetError());
  }

  if (initialize() != 0) {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    return 1;
  }

  return 0;
}

int Window::start() {
  bool done = false;

  //! THIS IS THE MAIN RENDERING LOOP
  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    for (const auto& [name, callback] : this->runningLoopCallbacks) {
      callback(*this);
    }

    SDL_RenderClear(this->renderer);

    for (const auto& widget : this->contentWidgets) {
      if (!widget) {
        continue;
      }
      widget->renderContent(this->textEngine);
    }

    SDL_RenderPresent(this->renderer);
  }

  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_Quit();

  return 0;
}

int Window::initialize() {
  /*
    Initializes available fonts
  */

  system("fc-cache -f -v");

  FILE* fontlistFile = popen("fc-list -f '%{family}|%{file}|%{style}\\n' | sort | uniq", "r");

  if (fontlistFile == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not open font list pipe: %s\n", SDL_GetError());
    return 1;
  }

  std::array<char, 4096> buf{};
  std::string fcout;

  while (fgets(buf.data(), buf.size(), fontlistFile) != NULL) {
    fcout += buf.data();
  }

  std::istringstream f(fcout);
  std::string line;

  auto trim = [](const std::string& s) {
    const size_t start = s.find_first_not_of(" \t");
    if (start == std::string::npos) {
      return std::string();
    }
    const size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
  };

  while (std::getline(f, line)) {
    size_t firstSep = line.find('|');
    size_t secondSep = line.find('|', firstSep + 1);
    if (firstSep == std::string::npos || secondSep == std::string::npos) {
      continue;  // Malformed line
    }

    std::string families = trim(line.substr(0, firstSep));
    std::string path = trim(line.substr(firstSep + 1, secondSep - firstSep - 1));
    std::string stylesStr = trim(line.substr(secondSep + 1));

    size_t comma = families.find(',');
    std::string name = trim(comma == std::string::npos ? families : families.substr(0, comma));

    font newFont;
    newFont.name = name;
    newFont.path = path;

    std::istringstream stylesStream(stylesStr);
    std::string style;
    while (std::getline(stylesStream, style, ',')) {
      std::string trimmed = trim(style);
      if (!trimmed.empty()) {
        newFont.availableStyles.push_back(trimmed);
      }
    }

    this->availableFonts.push_back(newFont);

    std::cout << "Loaded font: " << name << " (" << path << ") with styles: ";
    for (const auto& s : newFont.availableStyles) {
      std::cout << s << " ";
    }
    std::cout << std::endl;
  }

  pclose(fontlistFile);

  if (!TTF_Init()) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize TTF: %s\n", SDL_GetError());
    return 1;
  }

  this->textEngine = TTF_CreateRendererTextEngine(this->renderer);
  if (this->textEngine == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create text engine: %s\n", SDL_GetError());
    return 1;
  }

  return 0;
}

TTF_Font* Window::findFont(std::string name, std::string style, int size) {
  for (const auto& f : this->availableFonts) {
    if (f.name == name) {
      for (const auto& s : f.availableStyles) {
        if (s == style) {
          TTF_Font* font = TTF_OpenFont(f.path.c_str(), size);
          if (font == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not open font: %s\n", SDL_GetError());
            return nullptr;
          }

          if (styleAliases.find(style) != styleAliases.end()) {
            TTF_SetFontStyle(font, styleAliases[style]);
          }

          return font;
        }
      }
    }
  }
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Font not found: %s %s\n", name.c_str(), style.c_str());
  return nullptr;
}

int Window::setContentWidgets(std::vector<std::shared_ptr<Widget>> widgets) {
  this->contentWidgets = widgets;
  return 0;  // Placeholder return value
}

std::shared_ptr<Widget> Window::getWidgetByName(const std::string& name) {
  for (const auto& widget : this->contentWidgets) {
    if (widget && widget->getName() == name) {
      return widget;
    }
  }
  throw std::runtime_error("Widget not found: " + name);
}

int Window::setWidgetByName(std::string name, std::shared_ptr<Widget> widget) {
  for (size_t i = 0; i < this->contentWidgets.size(); ++i) {
    if (this->contentWidgets[i] && this->contentWidgets[i]->getName() == name) {
      this->contentWidgets[i] = widget;
      return 0;  // Placeholder return value
    }
  }
  return 1;  // Widget not found
}

int Window::addToRunningLoop(std::string name, std::function<void(Window&)> callback) {
  this->runningLoopCallbacks[name] = callback;
  return 0;  // Placeholder return value
}