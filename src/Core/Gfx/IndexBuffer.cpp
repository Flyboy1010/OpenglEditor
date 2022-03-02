#include "Core/Gfx/IndexBuffer.h"
#include <GL/glew.h>
#include <cassert>

IndexBuffer::IndexBuffer()
{
	m_id = 0;
	m_size = 0;
	m_count = 0;
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void IndexBuffer::Create(unsigned int count, size_t elementSize, const void* data)
{
	assert(m_id == 0);

	m_count = count;
	m_size = count * elementSize;

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
