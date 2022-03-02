#pragma once

#include <vector>

enum FramebufferAttachment
{
	RGBA8,
	RED,
	DEPTH
};

struct FramebufferDeff
{
	int width, height;
	std::vector<FramebufferAttachment> fbAttachments;
};

class Framebuffer
{
public:
	Framebuffer(const FramebufferDeff& fbDeff);
	~Framebuffer();

	void Bind() const;
	void UnBind() const;

	unsigned int GetColorAttachment(unsigned int index);
	unsigned int GetDepthAttachment() const { return m_depthAttachment; }
	
	int ReadPixel(unsigned int colorAttachmentIndex, int x, int y);

	void ClearColorAttachment(unsigned int colorAttachmentIndex, int value);

	void Resize(int width, int height);

private:
	void AttachColorTexture(int internalFormat, int format);
	void AttachDepthTexture();
	void Create();

private:
	unsigned int m_id;
	FramebufferDeff m_deff;
	std::vector<unsigned int> m_colorAttachments;
	unsigned int m_depthAttachment;
};