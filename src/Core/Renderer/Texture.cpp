#include "Core/Renderer/Texture.h"
#include <GL/glew.h>
#include <stb_image/stb_image.h>
#include <iostream>
#include "Core/Debug/Logger.h"

Texture::Texture()
{
	m_id = 0;
	m_width = 0;
	m_height = 0;
	m_bpp = 0;
	m_pixels = nullptr;
}

Texture::Texture(Texture&& other) noexcept
{
	other.m_id = m_id;
	other.m_width = m_width;
	other.m_height = m_height;
	other.m_bpp = m_bpp;
	other.m_path = std::move(m_path);
	other.m_pixels = m_pixels;

	m_id = 0;
	m_width = 0;
	m_height = 0;
	m_bpp = 0;
	m_pixels = nullptr;
}

Texture::Texture(int width, int height)
{
	Create(width, height);
}

Texture::Texture(const std::string& path, bool keepData)
{
	Load(path, keepData);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
	stbi_image_free(m_pixels);

	LOG_ASSET("Texture destroyed \"%s\"", m_path.c_str());
}

void Texture::Create(int width, int height)
{
	// set properties

	m_width = width;
	m_height = height;
	m_bpp = 4;
	m_pixels = nullptr;

	// opengl create texture

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// opengl texture parameters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// set data

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void Texture::Load(const std::string& path, bool keepData)
{
	// init

	m_path = path;

	// load image

	stbi_set_flip_vertically_on_load(true);
	m_pixels = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

	// check if the image is loaded

	if (m_pixels == nullptr)
	{
		LOG_ERROR("Texture loading \"%s\"", path.c_str());
		return;
	}

	// if loaded succesfully then create the texture

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// opengl texture parameters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set data

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);

	// generate mipmaps

	glGenerateMipmap(GL_TEXTURE_2D);

	// keep or not the pixel data

	if (!keepData)
	{
		stbi_image_free(m_pixels);
		m_pixels = nullptr;
	}

	// log

	LOG_ASSET("Texture loaded \"%s\"", path.c_str());
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Active(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::SetPixels(int width, int height, const void* pixels)
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}
