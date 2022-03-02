#include "Core/Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

struct QuadVertex
{
	glm::vec2 position;
	float textureId;
	glm::vec2 textureUv;
	glm::vec4 color;
};

struct LineVertex
{
	glm::vec2 position;
	glm::vec4 color;
};

struct RendererData
{
	RendererStats stats;
	OrthoCamera camera;

	/* QUADS */

	const int MAX_QUADS = 10000;

	VertexArray quadsVA;
	VertexBuffer quadsVB;
	IndexBuffer quadsIB;
	QuadVertex* quadsVD;

	int textureSlots;
	int texturesCount;
	int samplers[32];
	unsigned int texturesId[32];
	int quadsCount;

	std::unique_ptr<Shader> quadsShader;

	/* LINES */

	const int MAX_LINES = 10000;

	VertexArray linesVA;
	VertexBuffer linesVB;
	LineVertex* linesVD;

	int linesCount;

	std::unique_ptr<Shader> linesShader;
};

static RendererData rd;

void Renderer::Init()
{
	/* INIT */

	// camera

	rd.camera.SetSize(1280, 720);

	// get texture slots

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &rd.textureSlots);

	for (int i = 0; i < rd.textureSlots; i++)
		rd.samplers[i] = i;

	// texture ids

	memset(rd.texturesId, 0, rd.textureSlots * sizeof(unsigned int));

	rd.quadsCount = 0;
	rd.linesCount = 0;

	/* QUADS */

	// vertex buffer

	rd.quadsVB.Create(4 * rd.MAX_QUADS * sizeof(QuadVertex));

	rd.quadsVD = new QuadVertex[4 * rd.MAX_QUADS];

	// vertex buffer layout

	VertexBufferLayout quadsVBL;

	quadsVBL.AddElement<float>(2); // position
	quadsVBL.AddElement<float>(1); // texture id
	quadsVBL.AddElement<float>(2); // texture uv
	quadsVBL.AddElement<float>(4); // color

	// index buffer

	// generate and set index buffer data

	unsigned int* quadsIBD = new unsigned int[6 * rd.MAX_QUADS];

	int n = 0;

	for (int i = 0; i < 6 * rd.MAX_QUADS; i += 6) {
		quadsIBD[i] = n;
		quadsIBD[i + 1] = n + 1;
		quadsIBD[i + 2] = n + 2;
		quadsIBD[i + 3] = n + 2;
		quadsIBD[i + 4] = n + 3;
		quadsIBD[i + 5] = n;

		n += 4;
	}

	rd.quadsIB.Create(6 * rd.MAX_QUADS, sizeof(unsigned int), quadsIBD);

	delete[] quadsIBD;

	// vertex array

	rd.quadsVA.Create(rd.quadsVB, quadsVBL);

	// shader

	rd.quadsShader = std::make_unique<Shader>("Shaders/quads.vert", "Shaders/quads.frag");

	/* LINES */

	// vertex buffer and data

	rd.linesVB.Create(2 * rd.MAX_LINES * sizeof(LineVertex));

	rd.linesVD = new LineVertex[2 * rd.MAX_LINES];

	// vertex buffer layout

	VertexBufferLayout linesVBL;

	linesVBL.AddElement<float>(2); // position
	linesVBL.AddElement<float>(4); // color

	// vertex array

	rd.linesVA.Create(rd.linesVB, linesVBL);

	// shader

	rd.linesShader = std::make_unique<Shader>("Shaders/lines.vert", "Shaders/lines.frag");
}

void Renderer::Destroy()
{
	delete[] rd.quadsVD;
	delete[] rd.linesVD;
}

const RendererStats& Renderer::GetStats()
{
	return rd.stats;
}

void Renderer::ResetStats()
{
	rd.stats.drawCalls = 0;
	rd.stats.quadsCount = 0;
	rd.stats.linesCount = 0;
}


void Renderer::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Renderer::SetViewportAspectRatio(int windowWidth, int windowHeight, float targetAspectRatio)
{
	// calculate target viewport size

    float window_aspect_ratio = windowWidth / (float)windowHeight;

    int targetX = 0;
    int targetY = 0;
    int targetWidth = windowWidth;
    int targetHeight = windowHeight;

    if (window_aspect_ratio > targetAspectRatio)
    {
        targetWidth = targetHeight * targetAspectRatio;
        targetX = (windowWidth - targetWidth) / 2;
    }
    else
    {
        targetHeight = targetWidth * (1 / targetAspectRatio);
        targetY = (windowHeight - targetHeight) / 2;
    }

    glViewport(targetX, targetY, targetWidth, targetHeight);
}

void Renderer::SetLineWidth(float width)
{
	glLineWidth(width);
}

void Renderer::StartBatch()
{
	// reset for quads

	rd.quadsCount = 0;
	rd.texturesCount = 0;
	
	// reset for lines

	rd.linesCount = 0;
}

static void FlushQuads()
{
	if (rd.quadsCount > 0)
	{
		// bind shader
		
		rd.quadsShader->Bind();
		rd.quadsShader->SetUniformMat4("u_projection", rd.camera.GetProjection());
		rd.quadsShader->SetUniformMat4("u_view", rd.camera.GetView());
		rd.quadsShader->SetUniform1iv("u_textures", rd.textureSlots, rd.samplers);

		// bind textures

		for (int i = 0; i < rd.texturesCount; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, rd.texturesId[i]);
		}

		// bind quads_vbo and set data

		rd.quadsVB.Bind();
		rd.quadsVB.UpdateData(4 * rd.quadsCount * sizeof(QuadVertex), rd.quadsVD);

		// bind vertex array

		rd.quadsVA.Bind();

		// bind index buffer

		rd.quadsIB.Bind();

		// draw call

		glDrawElements(GL_TRIANGLES, 6 * rd.quadsCount, GL_UNSIGNED_INT, nullptr);

		// stats

		rd.stats.drawCalls++;
	}

	// reset

	rd.quadsCount = 0;
	rd.texturesCount = 0;
	// memset(rd.texturesId, 0, rd.textureSlots * sizeof(unsigned int));
}

static void FlushLines()
{
	if (rd.linesCount > 0)
	{
		// bind shader and set uniforms

		rd.linesShader->Bind();
		rd.linesShader->SetUniformMat4("u_projection", rd.camera.GetProjection());

		// set vertex buffer data

		rd.linesVB.Bind();
		rd.linesVB.UpdateData(2 * rd.linesCount * sizeof(LineVertex), rd.linesVD);

		// bind vertex array

		rd.linesVA.Bind();

		// draw call

		glDrawArrays(GL_LINES, 0, 2 * rd.linesCount);

		// stats

		rd.stats.drawCalls++;
	}

	rd.linesCount = 0;
}

void Renderer::Flush()
{
	FlushQuads();

	FlushLines();
}

void Renderer::DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec4& color)
{
	if (texture == nullptr)
		return;

	DrawTexture(texture, position, { texture->GetWidth(), texture->GetHeight() }, color);
}

void Renderer::DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (texture == nullptr)
		return;

	DrawTexture(texture, position, size, { 0, 0 }, { texture->GetWidth(), texture->GetHeight() }, color);
}

void Renderer::DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& srcPosition, const glm::vec2& srcSize, const glm::vec4& color)
{
	if (texture == nullptr)
		return;

	// check if it needs to make a new batch

	if (rd.quadsCount >= rd.MAX_QUADS || rd.texturesCount >= rd.textureSlots)
		FlushQuads();

	// get texture slot

	int slot = -1;

	for (int i = 0; i < rd.texturesCount; i++) {
		if (texture->GetId() == rd.texturesId[i]) {
			slot = i;
			break;
		}
	}

	if (slot == -1) {
		rd.texturesId[rd.texturesCount] = texture->GetId();
		slot = rd.texturesCount;
		rd.texturesCount++;
	}

	// normalize the srcPosition and srcSize

	glm::vec2 srcPositionNormalized = { srcPosition.x / (float)texture->GetWidth(), srcPosition.y / (float)texture->GetHeight() };
	glm::vec2 srcSizeNormalized = { srcSize.x / (float)texture->GetWidth(), srcSize.y / (float)texture->GetHeight() };

	// set the vertex data (position, texture id, texture uv, color)

	int index = rd.quadsCount * 4;

	rd.quadsVD[index]     = { {position.x         , position.y + size.y }, (float)slot, {srcPositionNormalized.x                        , 1 - (srcPositionNormalized.y + srcSizeNormalized.y) }, color };
	rd.quadsVD[index + 1] = { {position.x + size.x, position.y + size.y }, (float)slot, {srcPositionNormalized.x + srcSizeNormalized.x, 1 - (srcPositionNormalized.y + srcSizeNormalized.y) }, color };
	rd.quadsVD[index + 2] = { {position.x + size.x, position.y		  	 }, (float)slot, {srcPositionNormalized.x + srcSizeNormalized.x, 1 - srcPositionNormalized.y                           }, color };
	rd.quadsVD[index + 3] = { {position.x         , position.y		  	 }, (float)slot, {srcPositionNormalized.x                        , 1 - srcPositionNormalized.y                           }, color };

	// increment the number of quads

	rd.quadsCount++;

	// stats

	rd.stats.quadsCount++;
}

void Renderer::DrawTexture(const Texture* texture, const glm::vec2& position, float radians, const glm::vec4& color)
{
	if (texture == nullptr)
		return;

	DrawTexture(texture, position, { texture->GetWidth(), texture->GetHeight() }, radians, color);
}

void Renderer::DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec2& size, float radians, const glm::vec4& color)
{
	if (texture == nullptr)
		return;

	DrawTexture(texture, position, size, { 0.0f, 0.0f }, { texture->GetWidth(), texture->GetHeight() }, radians, color);
}

void Renderer::DrawTexture(const Texture* texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& srcPosition, const glm::vec2& srcSize, float radians, const glm::vec4& color)
{
	if (texture == nullptr)
		return;

	// check if it needs to make a new batch

	if (rd.quadsCount >= rd.MAX_QUADS || rd.texturesCount >= rd.textureSlots)
		FlushQuads();

	// get texture slot

	int slot = -1;

	for (int i = 0; i < rd.texturesCount; i++) {
		if (texture->GetId() == rd.texturesId[i]) {
			slot = i;
			break;
		}
	}

	if (slot == -1) {
		rd.texturesId[rd.texturesCount] = texture->GetId();
		slot = rd.texturesCount;
		rd.texturesCount++;
	}

	// normalize the srcPosition and srcSize

	glm::vec2 srcPositionNormalized = { srcPosition.x / (float)texture->GetWidth(), srcPosition.y / (float)texture->GetHeight() };
	glm::vec2 srcSizeNormalized = { srcSize.x / (float)texture->GetWidth(), srcSize.y / (float)texture->GetHeight() };

	// transformation

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) * glm::rotate(glm::mat4(1.0f), radians, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.0f));

	glm::vec4 normalQuad[4] = {
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.5f,  0.5f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0.0f, 1.0f}
	};

	int index = rd.quadsCount * 4;

	rd.quadsVD[index]     = { transform * normalQuad[0], (float)slot, {srcPositionNormalized.x                        , 1 - (srcPositionNormalized.y)}, color };
	rd.quadsVD[index + 1] = { transform * normalQuad[1], (float)slot, {srcPositionNormalized.x + srcSizeNormalized.x, 1 - (srcPositionNormalized.y)}, color };
	rd.quadsVD[index + 2] = { transform * normalQuad[2], (float)slot, {srcPositionNormalized.x + srcSizeNormalized.x, 1 - (srcPositionNormalized.y + srcSizeNormalized.y)}, color };
	rd.quadsVD[index + 3] = { transform * normalQuad[3], (float)slot, {srcPositionNormalized.x                        , 1 - (srcPositionNormalized.y + srcSizeNormalized.y)}, color };

	rd.quadsCount++;

	// stats

	rd.stats.quadsCount++;
}

void Renderer::DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color)
{
	if (rd.linesCount >= rd.MAX_LINES)
		FlushLines();

	unsigned int index = rd.linesCount * 2;

	rd.linesVD[index] = { p1, color };
	rd.linesVD[index + 1] = { p2, color };

	rd.linesCount++;

	// stats

	rd.stats.linesCount++;
}

void Renderer::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	DrawLine(position, glm::vec2(position.x + size.x, position.y), color);
	DrawLine(glm::vec2(position.x + size.x, position.y), glm::vec2(position.x + size.x, position.y + size.y), color);
	DrawLine(glm::vec2(position.x + size.x, position.y + size.y), glm::vec2(position.x, position.y + size.y), color);
	DrawLine(glm::vec2(position.x, position.y + size.y), position, color);
}

void Renderer::RenderModel(const Model* model, const glm::mat4& transform, const glm::mat4& cameraProjection, const glm::mat4& cameraTransform, const glm::vec3& cameraPosition, const Shader* shader)
{
	shader->Bind();
	shader->SetUniformMat4("u_model", transform);
	shader->SetUniformMat4("u_projection", cameraProjection);
	shader->SetUniformMat4("u_view", cameraTransform);
	shader->SetUniformVec3("u_cameraPosition", cameraPosition);
	// shader->SetUniformMat4("u_normalMatrix", glm::transpose(glm::inverse(transform)));

	const auto& meshes = model->GetMeshes();

	for (auto& mesh : meshes)
	{
		// set the textures

		const Material& material = mesh->material;

		/*if (material.textureDiffuse != nullptr)
			material.textureDiffuse->Active(0);

		if (material.textureNormal != nullptr)
			material.textureNormal->Active(1);

		shader->SetUniform1i("u_textureDiffuse", 0);
		shader->SetUniform1i("u_textureNormal", 1);*/

		if (material.albedoMap != nullptr)
			material.albedoMap->Active(0);
		if (material.normalMap != nullptr)
			material.normalMap->Active(1);
		if (material.metallicRoughnessMap != nullptr)
			material.metallicRoughnessMap->Active(2);
		if (material.aoMap != nullptr)
			material.aoMap->Active(3);

		shader->SetUniform1i("u_albedoMap", 0);
		shader->SetUniform1i("u_normalMap", 1);
		shader->SetUniform1i("u_metallicRoughnessMap", 2);
		// shader->SetUniform1i("u_aoMap", 3);

		// bind vertex array

		glBindVertexArray(mesh->va);

		// bind index buffer

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ib);

		// draw call

		glDrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, nullptr);

		// stats

		rd.stats.drawCalls++;
	}
}