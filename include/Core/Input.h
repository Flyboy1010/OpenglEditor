#pragma once

#include <GLFW/glfw3.h>

class Input
{
public:
	static void SetFocusWindow(GLFWwindow* window);
	static void Update();

	static bool KeyPressed(int key);
	static bool KeyReleased(int key);
	static bool KeyJustPressed(int key);
	static bool KeyJustReleased(int key);

	static bool MouseButtonPressed(int button);
	static bool MouseButtonReleased(int button);
	static bool MouseButtonJustPressed(int button);
	static bool MouseButtonJustReleased(int button);

	static float GetMouseX();
	static float GetMouseY();
	static float GetMouseScrollDelta();
	static void GetMousePos(float* x, float* y);

private:
	Input() {}
	~Input() {}
};