#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

// glew.
#define GLEW_STATIC
#include <GL/glew.h>
// glfw.
#include <GLFW/glfw3.h>

#define GLAssert(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError(); x; GLAssert(GLLogCall(#x, __LINE__));

extern void GLClearError();
extern bool GLLogCall(const char *function, int line);
