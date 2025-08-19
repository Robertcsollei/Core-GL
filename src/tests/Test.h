#pragma once
#include <string>
#include "../Renderer.h"

namespace test {

	class Test {
	public:
		Test() = default;
		virtual ~Test() = default;

	public:
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender(Renderer* renderer) = 0;
		virtual void OnImGuiRender() = 0;

		virtual const std::string& GetName() const { return "Test";  }
	};
}