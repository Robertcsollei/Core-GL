#pragma once
#include <core/entities/Mesh.h>
#include <core/geometry/Ellipsoid.h>
#include <glm/vec3.hpp>
#include <memory>

class Ellipsoid;
class Satellite;

namespace MeshFactory {
std::unique_ptr<Mesh>
CreateEllipsoid(int slices,
                int stacks,
                const glm::vec3& center,
                Ellipsoid& ellipsoid);

std::unique_ptr<Mesh>
CreatePoint(const glm::vec3& center, uint32_t color);

std::unique_ptr<Mesh>
CreateOrbitRibbon(Satellite& sat,
                  double t,
                  int segments,
                  uint32_t color,
                  const glm::vec3& camPos,
                  float thickness);
}
