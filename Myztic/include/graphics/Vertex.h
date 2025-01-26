#pragma once
#include <vector>
#include <type_traits>
#include <glm\glm.hpp>

struct BGRAColor
{
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class VertexLayout
{
public:
	enum ElementType
	{
		Position2D,
		Position3D,
		Texture2D,
		Normal,
		Float3Color,
		Float4Color,
		BGRAColor,
	};
	template<ElementType> struct Map;
	template<> struct Map<Position2D>
	{
		using SysType = glm::vec2;
		static constexpr GLint elementCount = 2;
		static constexpr GLenum glFormat = GL_FLOAT;
		static constexpr unsigned long long sizeOfFormat = sizeof(GLfloat);
		static constexpr InputPropertyType semantic = Position;
	};
	template<> struct Map<Position3D>
	{
		using SysType = glm::vec3;
		static constexpr GLint elementCount = 3;
		static constexpr GLenum glFormat = GL_FLOAT;
		static constexpr unsigned long long sizeOfFormat = sizeof(GLfloat);
		static constexpr InputPropertyType semantic = Position;
	};
	template<> struct Map<Texture2D>
	{
		using SysType = glm::vec2;
		static constexpr GLint elementCount = 2;
		static constexpr GLenum glFormat = GL_FLOAT;
		static constexpr unsigned long long sizeOfFormat = sizeof(GLfloat);
		static constexpr InputPropertyType semantic = TextureCoordinate;
	};
	template<> struct Map<Normal>
	{
		using SysType = glm::vec3;
		static constexpr GLint elementCount = 3;
		static constexpr GLenum glFormat = GL_FLOAT;
		static constexpr unsigned long long sizeOfFormat = sizeof(GLfloat);
		static constexpr InputPropertyType semantic = InputPropertyType::Normal;
	};
	template<> struct Map<Float3Color>
	{
		using SysType = glm::vec3;
		static constexpr GLint elementCount = 3;
		static constexpr GLenum glFormat = GL_FLOAT;
		static constexpr unsigned long long sizeOfFormat = sizeof(GLfloat);
		static constexpr InputPropertyType semantic = Color;
	};
	template<> struct Map<Float4Color>
	{
		using SysType = glm::vec4;
		static constexpr GLint elementCount = 4;
		static constexpr GLenum glFormat = GL_FLOAT;
		static constexpr unsigned long long sizeOfFormat = sizeof(GLfloat);
		static constexpr InputPropertyType semantic = Color;
	};
	template<> struct Map<BGRAColor>
	{
		using SysType = glm::u8vec4;
		static constexpr GLint elementCount = 4;
		//this was originally a UNORM variable in DirectX, unsure what it translates to here, make sure this works if ever used.
		static constexpr GLenum glFormat = GL_UNSIGNED_BYTE;
		static constexpr unsigned long long sizeOfFormat = sizeof(GLubyte);
		static constexpr InputPropertyType semantic = Color;
	};

	class Element
	{
	public:
		Element(ElementType type, GLint offset)
			:
			type(type),
			offset(offset)
		{}
		GLint GetOffsetAfter() const noexcept
		{
			return offset + Size();
		}
		GLint GetOffset() const
		{
			return offset;
		}
		GLint Size() const noexcept
		{
			return SizeOf(type);
		}
		static constexpr GLint SizeOf(ElementType type)
		{
			switch (type)
			{
			case Position2D:
				return sizeof(Map<Position2D>::SysType);
			case Position3D:
				return sizeof(Map<Position3D>::SysType);
			case Texture2D:
				return sizeof(Map<Texture2D>::SysType);
			case Normal:
				return sizeof(Map<Normal>::SysType);
			case Float3Color:
				return sizeof(Map<Float3Color>::SysType);
			case Float4Color:
				return sizeof(Map<Float4Color>::SysType);
			case BGRAColor:
				return sizeof(Map<BGRAColor>::SysType);
			}
			assert("Invalid element type" && false);
			return 0u;
		}
		ElementType GetType() const noexcept
		{
			return type;
		}
		InputProperty GetLayout() const noexcept
		{
			switch (type)
			{
			case Position2D:
				return GenerateLayout<Position2D>(GetOffset());
			case Position3D:
				return GenerateLayout<Position3D>(GetOffset());
			case Texture2D:
				return GenerateLayout<Texture2D>(GetOffset());
			case Normal:
				return GenerateLayout<Normal>(GetOffset());
			case Float3Color:
				return GenerateLayout<Float3Color>(GetOffset());
			case Float4Color:
				return GenerateLayout<Float4Color>(GetOffset());
			case BGRAColor:
				return GenerateLayout<BGRAColor>(GetOffset());
			}
			return {ShaderInputLayout::POSITION};
		}
	private:
		template<ElementType type>
		static constexpr InputProperty GenerateLayout(GLint offset)
		{
			return { Map<type>::semantic, Map<type>::elementCount, Map<type>::glFormat, offset, Map<type>::sizeOfFormat };
		}
	private:
		ElementType type;
		GLint offset;
	};
public:
	template<ElementType Type>
	const Element& Resolve() const
	{
		for (auto& e : elements)
		{
			if (e.GetType() == Type)
			{
				return e;
			}
		}
		assert("Could not resolve element type" && false);
		return elements.front();
	}
	const Element& ResolveByIndex(size_t i) const
	{
		return elements[i];
	}
	VertexLayout& Append(ElementType type)
	{
		elements.emplace_back(type, Size());
		return *this;
	}
	GLint Size() const
	{
		return static_cast<GLint>(elements.empty() ? 0u : elements.back().GetOffsetAfter());
	}
	GLint GetElementCount() const noexcept
	{
		return static_cast<GLint>(elements.size());
	}
	LayoutDescription GetDescription() const
	{
		LayoutDescription desc;
		desc.elementCount = GetElementCount();
		for (const auto& e : elements)
		{
			desc.inputProperties.push_back(e.GetLayout());
		}
		return desc;
	}
private:
	std::vector<Element> elements;
};

class Vertex
{
	friend class VertexBuffer;
public:
	template<VertexLayout::ElementType Type>
	auto& Attr()
	{
		auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
		return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
	}
	template<typename T>
	void SetAttributeByIndex(size_t i, T&& val)
	{
		const auto& element = layout.ResolveByIndex(i);
		auto pAttribute = pData + element.GetOffset();
		switch (element.GetType())
		{
		case VertexLayout::Position2D:
			SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Position3D:
			SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Texture2D:
			SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Normal:
			SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Float3Color:
			SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Float4Color:
			SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::BGRAColor:
			SetAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
			break;
		default:
			assert("Bad element type" && false);
		}
	}
protected:
	Vertex(char* pData, const VertexLayout& layout)
		:
		pData(pData),
		layout(layout)
	{
		assert(pData != nullptr);
	}
private:
	template<typename First, typename ...Rest>
	// enables parameter pack setting of multiple parameters by element index
	void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest)
	{
		SetAttributeByIndex(i, std::forward<First>(first));
		SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
	}
	// helper to reduce code duplication in SetAttributeByIndex
	template<VertexLayout::ElementType DestLayoutType, typename SrcType>
	void SetAttribute(char* pAttribute, SrcType&& val)
	{
		using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
		if constexpr (std::is_assignable<Dest, SrcType>::value)
		{
			*reinterpret_cast<Dest*>(pAttribute) = val;
		}
		else
		{
			assert("Parameter attribute type mismatch" && false);
		}
	}
private:
	char* pData = nullptr;
	const VertexLayout& layout;
};

class VertexBuffer
{
public:
	VertexBuffer(VertexLayout layout)
		:
		layout(std::move(layout))
	{}
	const char* GetData() const
	{
		return buffer.data();
	}
	const VertexLayout& GetLayout() const noexcept
	{
		return layout;
	}
	GLint Size() const
	{
		return static_cast<GLint>(buffer.size() / layout.Size());
	}
	GLint SizeBytes() const
	{
		return static_cast<GLint>(buffer.size());
	}
	template<typename ...Params>
	void EmplaceBack(Params&&... params)
	{
		assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
		buffer.resize(buffer.size() + layout.Size());
		Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
	}
	Vertex Back()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
	}
	Vertex Front()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(),layout };
	}
	Vertex operator[](size_t i)
	{
		assert(i < Size());
		return Vertex{ buffer.data() + layout.Size() * i,layout };
	}
private:
	std::vector<char> buffer;
	VertexLayout layout;
};