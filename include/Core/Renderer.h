#pragma once

#include <glm/glm.hpp>
#include "Gfx/Texture.h"
#include "Gfx/Shader.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/IndexBuffer.h"
#include "Gfx/VertexArray.h"
#include "Gfx/FrameBuffer.h"
#include "Model.h"
#include "OrthoCamera.h"

struct RendererStats
{
	int drawCalls;
	int quadsCount;
	int linesCount;
};

class Renderer
{
public:
	static void Init();
	static void Destroy();

	static const RendererStats& GetStats();
	static void ResetStats();
	
	static void SetClearColor(const glm::vec4& color);
	static void Clear();
	static void SetViewport(int x, int y, int width, int height);
	static void SetViewportAspectRatio(int windowWidth, int windowHeight, float targetAspectRatio);
	static void SetLineWidth(float width);
	
	static void StartBatch();
	static void Flush();
	
	static void DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static void DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static void DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& srcPosition, const glm::vec2& srcSize, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	static void DrawTexture(const Texture* texture, const glm::vec2& position, float radians, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static void DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec2& size, float radians, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static void DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& srcPposition, const glm::vec2& srcSize, float radians, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	static void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	static void RenderModel(const Model* model, const glm::mat4& transform, const glm::mat4& cameraProjection, const glm::mat4& cameraTransform, const glm::vec3& cameraPosition, const Shader* shader);

private:
	Renderer() {}
	~Renderer() {}
};
