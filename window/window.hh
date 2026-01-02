#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../primitives/font.hh"
#include "../widgets/widgets.hh"

class Window {
 private:
  std::vector<std::shared_ptr<Widget>> contentWidgets;
  std::vector<std::shared_ptr<Widget>> alreadyPrintedWidgets;
  std::vector<font> availableFonts;
  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_TextEngine* textEngine;

  std::map<std::string, std::function<void(Window&)>> runningLoopCallbacks;
  int initialize();

 public:
  // Widget management
  std::shared_ptr<Widget> getWidgetByName(const std::string& name);
  int setWidgetByName(std::string name, std::shared_ptr<Widget> widget);
  int removeWidgetByName(std::string name);

  // Loop management
  int addToRunningLoop(std::string name, std::function<void(Window&)> callback);
  int removeFromRunningLoop(std::string name);

  int setContentWidgets(std::vector<std::shared_ptr<Widget>> widgets);
  int render();
  int start();
  int create(std::string title, int width, int height);
  TTF_Font* findFont(std::string name, std::string style = "Regular", int size = 12);
  Window();
  ~Window();
};

Window::Window() {
}

Window::~Window() {
}
