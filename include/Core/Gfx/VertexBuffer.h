#pragma once

#include <vector>
#include <GL/glew.h>
#include <cstddef>

struct Layout
{
	unsigned int count;
	unsigned int type;
	size_t elementSize;
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();

	const std::vector<Layout>& GetElements() const { return m_elements; }
	size_t GetStride() const { return m_stride; }

	template<typename T>
	void AddElement(unsigned int count);

private:
	std::vector<Layout> m_elements;
	size_t m_stride;
};

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Create(size_t size);
	void CreateWithData(size_t size, const void* data);

	void UpdateData(size_t size, const void* data);

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_id;
	size_t m_size;
};
