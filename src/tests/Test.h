#pragma once
#include <string>
#include <array>
#include <vector>
#include "../Renderer.h"
#include <glm/glm.hpp>
#include "../VertexBufferLayout.h"

namespace test {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;

		Vertex(const glm::vec3& position, const glm::vec4& color)
			: Position(position), Color(color), TexCoord(0.f, 0.f) {}

		Vertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& textCoord)
			: Position(position), Color(color), TexCoord(textCoord) {}

		static void AppendLayout(VertexBufferLayout* layout) {
			layout->Push<float>(3); // Position
			layout->Push<float>(4); // Color
			layout->Push<float>(2); // TexCoord
		
		}
	};

	class Test {
	public:
		Test() = default;
		virtual ~Test() = default;

	public:
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender(Renderer* renderer) = 0;
		virtual void OnImGuiRender() = 0;

		virtual std::string GetName() const { return "Test";  }


		static std::array<Vertex, 4> CreateRect(float x, float y, float w, float h, const glm::vec4& color)
		{
			return {
				Vertex(glm::vec3(x, y,			0.0f), color, {0.f, 0.f}),
				Vertex(glm::vec3(x + w, y,		0.0f), color, {1.f, 0.f}),
				Vertex(glm::vec3(x + w, y - h,	0.0f), color, {1.f, 1.f}),
				Vertex(glm::vec3(x, y - h,		0.0f), color, {0.f, 1.f})
			};
		};
		
		static std::vector<uint32_t> CreateRectIndices(size_t vertexCount)
		{
			std::vector<uint32_t> indices;
			const size_t quadCount = vertexCount / 4;
			indices.reserve(quadCount * 6);
			for (int q = 0; q < quadCount; q++)
			{
				const uint32_t base = static_cast<uint32_t>(q * 4);
				indices.push_back(base + 0);
				indices.push_back(base + 1);
				indices.push_back(base + 2);
				indices.push_back(base + 2);
				indices.push_back(base + 3);
				indices.push_back(base + 0);
			}

			return indices;
		};
	};
}