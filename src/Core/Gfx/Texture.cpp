#include "Core/Gfx/Texture.h"
#include <GL/glew.h>
#include <stb_image/stb_image.h>
#include <iostream>
#include "Core/Logger.h"

Texture::Texture()
{
	m_id = 0;
	m_width = 0;
	m_height = 0;
	m_bpp = 0;
	m_pixels = nullptr;
	m_path = "";
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

	if (m_pixels != nullptr)
	{
		stbi_image_free(m_pixels);
	}

	LOG_INFO("Texture destroyed \"%s\"", m_path.c_str());
}

void Texture::Create(int width, int height)
{
	// set properties

	m_width = width;
	m_height = height;
	m_bpp = 3;
	m_pixels = nullptr;
	m_path = "User created texture";

	// opengl create texture

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// opengl texture parameters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

	// opengl create texture

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

	// check if the texture is loaded

	if (m_pixels != nullptr)
	{
		// if you want to keep the pixel data

		if (!keepData)
		{
			stbi_image_free(m_pixels);

			m_pixels = nullptr;
		}

		LOG_INFO("Texture loaded \"%s\"", path.c_str());
	}
	else
	{
		LOG_ERROR("Texture loading \"%s\"", path.c_str());
	}
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
