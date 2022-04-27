#pragma once

#include <vector>
#include <cstddef>

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	size_t elementSize;
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();

	const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
	size_t GetStride() const { return m_stride; }

	template<typename T>
	void AddElement(unsigned int count);

private:
	std::vector<VertexBufferElement> m_elements;
	size_t m_stride;
};

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Create(size_t size);
	void Create(size_t size, const void* data);

	void SetData(size_t size, const void* data);

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_id;
	size_t m_size;
};

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void Create(unsigned int count, const void* data);

	void Bind() const;
	void UnBind() const;

	unsigned int GetCount() const { return m_count; }

private:
	unsigned int m_id;
	unsigned int m_count;
	size_t m_size;
};
