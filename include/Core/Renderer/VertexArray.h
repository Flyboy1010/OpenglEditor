#pragma once

#include "Buffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Create(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_id;
};