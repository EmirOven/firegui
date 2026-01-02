#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <string>

class Widget {
 private:
  int x;
  int y;
  int width;
  int height;
  std::string name;

  // Widget properties
  bool visible;
  bool enabled;

  // Widget events
  void onClick();
  void onHover();

 public:
  int setPosition(int x, int y);
  const std::string& getName() const;
  explicit Widget(std::string name);
  virtual ~Widget() = default;
  virtual int renderContent(TTF_TextEngine* textEngine) = 0;

 private:
  // Widget rendering
  void renderBackground();
  void renderBorder();
};

Widget::Widget(std::string name) {
  this->name = name;
}

const std::string& Widget::getName() const {
  return this->name;
}
