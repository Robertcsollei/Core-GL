#pragma once

// Core functionality (always available)
#include "core/App.h"
#include "core/Mesh.h"
#include "core/Renderable.h"
#include "core/Scene.h"
#include "core/Transform.h"

#include "renderer/Material.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

// Optional modules
#ifdef TERRAKIT_NETWORKING_ENABLED
#include "Networking/Fetch.h"
#include "Networking/NetworkingModule.h"
#endif

#ifdef TERRAKIT_SIMULATION_ENABLED
#include "Simulation/OrbitPropagator.h"
#include "Simulation/SimulationModule.h"
#endif

// Convenience namespace alias
namespace tk = TerraKit;