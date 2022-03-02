#pragma once

#include <string>

class Texture
{
public:
	Texture();
	Texture(int width, int height);
	Texture(const std::string& path, bool keepData = false);
	Texture(const Texture&) = delete; // delete copy ctor
	~Texture();

	unsigned int GetId() const { return m_id; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	const unsigned char* GetPixels() const { return m_pixels; }
	int GetBpp() const { return m_bpp; }
	const std::string& GetPath() const { return m_path; }

	void Create(int width, int height);
	void Load(const std::string& path, bool keepData = false);

	void Bind() const;
	void UnBind() const;
	void Active(unsigned int slot = 0) const;

	void SetPixels(int width, int height, const void* pixels);

	// delete assign operator

	Texture& operator=(const Texture&) = delete;

private:
	std::string m_path;
	unsigned int m_id;
	int m_width, m_height;
	int m_bpp;
	unsigned char* m_pixels;
};
