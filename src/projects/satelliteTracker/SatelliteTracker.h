#pragma once
#include <core/App.h>
#include <core/entities/objects/Satellite.h>
#include <network/Queue.h>

class SatelliteTracker : public Application
{
public:
  struct OrbitWithName
  {
    Satellite::Orbit orbit;
    std::string name;
  };

public:
  SatelliteTracker(int width, int height)
    : Application(width, height){};

  void Init() override;

protected:
  void Update(double deltaTime) override;
  void Render() override;
  void RenderUI() override;

private:
  ConcurrentQueue<OrbitWithName> m_OrbitQueue;
  bool m_FetchingData = false;

private:
  void FetchSatellitesData();
  void ParseTLEBody(const std::string& body);
};
