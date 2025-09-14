#pragma once
#include <core/entities/camera/Camera.h>

class Satellite;

struct SceneState
{
  // SatelliteStore sats; // positions, ids, selection, filters
  // EphemerisSystem ephemeris;
  // OrbitTrailSystem trails;
  // PickingSystem picking;
  // ModelManager models;
  double satelliteSpeedMultiplier =
    60.0; // how many seconds per real-time second
  Satellite* selectedSat = nullptr;
  Camera camera;

  SceneState()
    : camera(45.f, 800, 640, 0.1f, 10000.f)
  {
  }
};