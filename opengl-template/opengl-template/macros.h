#pragma once
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

// glew.
#define GLEW_STATIC
#include <GL/glew.h>
// glfw.
#include <GLFW/glfw3.h>

// glm.
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

// imgui.
#include <CoreFoundation/CoreFoundation.h>

#include "libs/imgui/imconfig.h"
#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_impl_glfw.h"
#include "libs/imgui/imgui_impl_opengl3.h"
#include "libs/imgui/imstb_rectpack.h"
#include "libs/imgui/imstb_textedit.h"

#define GLAssert(x) \
  if (!(x)) __builtin_trap();
#define GLCall(x) \
  GLClearError(); \
  x;              \
  GLAssert(GLLogCall(#x, __LINE__));

extern void GLClearError();
extern bool GLLogCall(const char *function, int line);
