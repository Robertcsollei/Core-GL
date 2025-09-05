#pragma once
#include "core/entities/Mesh.h"
#include "core/geometry/Ellipsoid.h"
#include <glm/vec3.hpp>
#include <memory>

class Ellipsoid;

namespace MeshFactory {
std::unique_ptr<Mesh>
CreateEllipsoid(float radius,
                int slices,
                int stacks,
                const glm::vec3& center,
                Ellipsoid& ellipsoid);

std::unique_ptr<Mesh>
CreatePoint(const glm::vec3& center, const glm::vec4& color);
}
