#include "Core/Gfx/Framebuffer.h"
#include <GL/glew.h>
#include <cassert>

Framebuffer::Framebuffer(const FramebufferDeff& fbDeff)
{
    m_id = -1;
    m_depthAttachment = -1;
    m_deff = fbDeff;

	Create();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_id);
	glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
	glDeleteTextures(1, &m_depthAttachment);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void Framebuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::GetColorAttachment(unsigned int index)
{
	if (index >= m_colorAttachments.size())
		return -1;

	return m_colorAttachments[index];
}

int Framebuffer::ReadPixel(unsigned int colorAttachmentIndex, int x, int y)
{
    int pixel;

    glReadBuffer(GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);

    return pixel;
}

void Framebuffer::ClearColorAttachment(unsigned int colorAttachmentIndex, int value)
{
    glClearTexImage(m_colorAttachments[colorAttachmentIndex], 0, GL_RED_INTEGER, GL_INT, &value);
}

void Framebuffer::AttachColorTexture(int internalFormat, int format)
{
    unsigned int colorAttachment;
    int colorAttachmentIndex = m_colorAttachments.size();

    glGenTextures(1, &colorAttachment);
    glBindTexture(GL_TEXTURE_2D, colorAttachment);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_deff.width, m_deff.height, 0, format, GL_UNSIGNED_BYTE, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, GL_TEXTURE_2D, colorAttachment, 0);

    m_colorAttachments.push_back(colorAttachment);
}

void Framebuffer::AttachDepthTexture()
{
    glGenTextures(1, &m_depthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_depthAttachment);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_deff.width, m_deff.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);
}

void Framebuffer::Create()
{
    // create framebuffer

    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    // add attachments

    for (int i = 0; i < m_deff.fbAttachments.size(); i++)
    {
        const FramebufferAttachment& fbAttachment = m_deff.fbAttachments[i];

        switch (fbAttachment)
        {
        case FramebufferAttachment::RGBA8:
            AttachColorTexture(GL_RGBA8, GL_RGBA);
            break;
        case FramebufferAttachment::RED:
            AttachColorTexture(GL_R32I, GL_RED_INTEGER);
            break;
        case FramebufferAttachment::DEPTH:
            AttachDepthTexture();
            break;
        }
    }

    // check if there is at least one color attachment

    if (m_colorAttachments.size() > 0)
    {
        // buffers to draw

        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_colorAttachments.size(), buffers);
    }
    else
    {
        // if not (example just a depth buffer) then we need to explicitly tell OpenGL we're not going to render any color data

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    // check if the framebuffer is succesfully created

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    // unbind the framebuffer

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(int width, int height)
{
    // delete the framebuffer and color and depth attachments

    glDeleteFramebuffers(1, &m_id);
    glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
    glDeleteTextures(1, &m_depthAttachment);
    m_colorAttachments.clear();

    // recreate with new dimensions

    m_deff.width = width;
    m_deff.height = height;

    Create();
}
