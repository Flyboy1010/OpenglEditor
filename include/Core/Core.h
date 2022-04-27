#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imguizmo/ImGuizmo.h>

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Model.h"
#include "Renderer/Renderer.h"

#include "Debug/Logger.h"
#include "Debug/Profiler.h"

#include "OrthoCamera.h"

#include "Input.h"
