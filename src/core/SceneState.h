#pragma once
#include "core/entities/camera/Camera.h"

struct SceneState
{
  // SatelliteStore sats; // positions, ids, selection, filters
  // EphemerisSystem ephemeris;
  // OrbitTrailSystem trails;
  // PickingSystem picking;
  // ModelManager models;
  Camera camera;

  SceneState()
    : camera(45.f, 800, 640, 0.1f, 10000.f)
  {
  }
};