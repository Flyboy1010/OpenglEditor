#include "Core/Input.h"
#include <string.h>

#define NUM_KEYS GLFW_KEY_LAST
#define NUM_MOUSE_BUTTONS GLFW_MOUSE_BUTTON_LAST

static bool keysOld[NUM_KEYS];
static bool keys[NUM_KEYS];

static bool buttonsOld[NUM_MOUSE_BUTTONS];
static bool buttons[NUM_MOUSE_BUTTONS];

static float mouseX;
static float mouseY;

static GLFWwindow* windowPtr;

static void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (key >= 0 && key < NUM_KEYS)
	{
    	keys[key] = (action != GLFW_RELEASE);
	}
}

static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	mouseX = xPos;
	mouseY = yPos;
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	buttons[button] = (action != GLFW_RELEASE);
}

void Input::SetFocusWindow(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyCallback);

	glfwSetCursorPosCallback(window, CursorPositionCallback);

	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	windowPtr = window;
}

void Input::Update()
{
	memcpy(keysOld, keys, NUM_KEYS);
	memcpy(buttonsOld, buttons, NUM_MOUSE_BUTTONS);
}

bool Input::KeyPressed(int key)
{
	return keys[key];
}

bool Input::KeyReleased(int key)
{
	return !keys[key];
}

bool Input::KeyJustPressed(int key)
{
	return (keys[key] && !keysOld[key]);
}

bool Input::KeyJustReleased(int key)
{
	return (!keys[key] && keysOld[key]);
}

bool Input::MouseButtonPressed(int button)
{
	return buttons[button];
}

bool Input::MouseButtonReleased(int button)
{
	return !buttons[button];
}

bool Input::MouseButtonJustPressed(int button)
{
	return (buttons[button] && !buttonsOld[button]);
}

bool Input::MouseButtonJustReleased(int button)
{
	return (!buttons[button] && buttonsOld[button]);
}

float Input::GetMouseX()
{
	return mouseX;
}

float Input::GetMouseY()
{
	return mouseY;
}

void Input::GetMousePos(float* x, float* y)
{
	*x = mouseX;
	*y = mouseY;
}

void Input::SetMode(int mode, int value)
{
	glfwSetInputMode(windowPtr, mode, value);
}
