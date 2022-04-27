#include "Core/Renderer/Buffer.h"
#include <GL/glew.h>
#include <cassert>

/* VERTEX BUFFER LAYOUT */

VertexBufferLayout::VertexBufferLayout()
{
	m_stride = 0;
}

template<>
void VertexBufferLayout::AddElement<float>(unsigned int count)
{
	m_elements.push_back({ count, GL_FLOAT, sizeof(float)});
	m_stride += count * sizeof(float);
}

template<>
void VertexBufferLayout::AddElement<unsigned int>(unsigned int count)
{
	m_elements.push_back({ count, GL_UNSIGNED_INT, sizeof(unsigned int)});
	m_stride += count * sizeof(unsigned int);
}

template<>
void VertexBufferLayout::AddElement<unsigned char>(unsigned int count)
{
	m_elements.push_back({ count, GL_UNSIGNED_BYTE, sizeof(unsigned char)});
	m_stride += count * sizeof(unsigned char);
}

/* VERTEX BUFFER */

VertexBuffer::VertexBuffer()
{
	m_id = 0;
	m_size = 0;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void VertexBuffer::Create(size_t size)
{
	// assert(m_id == 0);

	m_size = size;

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void VertexBuffer::Create(size_t size, const void* data)
{
	// assert(m_id == 0);

	m_size = size;

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::SetData(size_t size, const void* data)
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* INDEX BUFFER */

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

void IndexBuffer::Create(unsigned int count, const void* data)
{
	// assert(m_id == 0);

	m_count = count;
	m_size = count * sizeof(unsigned int);

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
