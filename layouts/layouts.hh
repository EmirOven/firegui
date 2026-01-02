#include <SDL3/SDL.h>

#include <vector>

#include "../widgets/widgets.hh"

class Layout {
 private:
  std::vector<Widget> widgets;

 public:
  Layout(/* args */);
  ~Layout();
};

Layout::Layout(/* args */) {
}

Layout::~Layout() {
}
