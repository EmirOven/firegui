#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "widgets/text.cc"
#include "window/window.cc"

// THIS IS A TEST FILE FOR THE FireGUI LIBRARY
// THIS IS SERVED AS IS FOR TESTING AND DEBUGGING PURPOSES ONLY
// THIS FILE IS NOT PART OF THE FireGUI LIBRARY ITSELF
// AND WILL BE EXEMPT FROM ALL FORMS OF SUPPORT OR WARRANTY
// USE AT YOUR OWN RISK

void textUpdater(Window& win) {
  try {
    auto widget = win.getWidgetByName("TextBox");
    std::shared_ptr<Text> textWidget = std::dynamic_pointer_cast<Text>(widget);
    if (textWidget) {
      static int counter = 0;
      textWidget->setText("Frame count: " + std::to_string(counter++));
    }
  } catch (const std::runtime_error& e) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error in textUpdater: %s\n", e.what());
  }
}

int main() {
  // This is a simple C++ program that prints "Hello, World!" to the console.
  std::cout << "Hello, World!" << std::endl;

  std::string textboxcontent = "Hello, FireGUI!";

  Window window = Window();

  if (window.create("Test", 800, 600) != 0) {
    return 1;
  }

  std::vector<std::shared_ptr<Widget>> widgets = {
      std::make_shared<Text>(
          "TextBox",
          textboxcontent,
          window.findFont("Adwaita Sans", "Bold", 72),
          SDL_Color{255, 0, 0, 255}),
  };

  window.setContentWidgets(widgets);

  window.addToRunningLoop("TextUpdater", textUpdater);

  return window.start();
}
