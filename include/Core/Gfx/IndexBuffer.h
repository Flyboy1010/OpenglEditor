#pragma once

#include <cstddef>

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void Create(unsigned int count, size_t elementSize, const void* data);

	void Bind() const;
	void UnBind() const;

	unsigned int GetCount() const { return m_count; }

private:
	unsigned int m_id;
	unsigned int m_count;
	size_t m_size;
};
