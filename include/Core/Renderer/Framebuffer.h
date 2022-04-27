#pragma once

#include <vector>

struct FramebufferAttachment2
{
	unsigned int id;
	int format;
	int internalFormat;
};

enum FramebufferAttachmentFormat
{
	RGBA8,
	RED,
	DEPTH
};

struct FramebufferSpecification
{
	int width, height;
	std::vector<FramebufferAttachmentFormat> fbAttachments;
};

class Framebuffer
{
public:
	Framebuffer(const FramebufferSpecification& fbDeff);
	~Framebuffer();

	void Bind() const;
	void UnBind() const;

	unsigned int GetColorAttachment(unsigned int index) const;
	unsigned int GetDepthAttachment() const { return m_depthAttachment; }
	
	void ReadPixels(unsigned int colorAttachmentIndex, int x, int y, int width, int height, void* pixels);

	void ClearColorAttachment(unsigned int colorAttachmentIndex, int value);

	void Resize(int width, int height);

private:
	void AttachColorBuffer(int internalFormat, int format);
	void AttachDepthBuffer();
	void Create();

private:
	unsigned int m_id;
	FramebufferSpecification m_deff;
	std::vector<unsigned int> m_colorAttachments;
	unsigned int m_depthAttachment;
};