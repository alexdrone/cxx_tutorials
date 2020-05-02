#pragma once
#include "macros.h"

extern void InitImGui(GLFWwindow *window);

class GuiWindow {
 public:
  GuiWindow(const std::string &title);
  void Render() const;

  static void init(GLFWwindow *window);
  static void destroy();
};
