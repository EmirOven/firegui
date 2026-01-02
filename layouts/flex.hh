#include <SDL3/SDL.h>

#include <vector>

#include "../layouts.hh"

class FlexLayout : public Layout {
 private:
  std::vector<Widget> widgets;

 public:
  FlexLayout(/* args */);
  ~FlexLayout();
};

FlexLayout::FlexLayout(/* args */) {
}

FlexLayout::~FlexLayout() {
}
