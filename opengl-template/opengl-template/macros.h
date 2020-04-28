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

#define GLAssert(x) \
  if (!(x)) __builtin_trap();
#define GLCall(x) \
  GLClearError(); \
  x;              \
  GLAssert(GLLogCall(#x, __LINE__));

extern void GLClearError();
extern bool GLLogCall(const char *function, int line);
