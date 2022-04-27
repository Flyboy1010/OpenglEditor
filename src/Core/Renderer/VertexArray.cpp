#include "Core/Renderer/VertexArray.h"
#include <GL/glew.h>
#include <cassert>

VertexArray::VertexArray()
{
	m_id = 0;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::Create(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	assert(m_id == 0);

	// create and bind vertex array

	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);

	// bind vertex buffer

	vb.Bind();

	// set layout

	auto& elements = layout.GetElements();
	size_t stride = layout.GetStride();
	size_t offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		auto& e = elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, e.count, e.type, GL_FALSE, stride, (const void*)offset);

		offset += e.count * e.elementSize;
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_id);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}
