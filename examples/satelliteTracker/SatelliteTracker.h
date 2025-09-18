#pragma once
#include <terrakit/core/App.h>
#include <terrakit/core/entities/Satellite.h>
#include <terrakit/network/Queue.h>

class SatelliteTracker : public terrakit::core::Application
{
public:
  struct OrbitWithName
  {
    terrakit::core::Satellite::Orbit orbit;
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
  terrakit::network::ConcurrentQueue<OrbitWithName> m_OrbitQueue;
  bool m_FetchingData = false;

private:
  void FetchSatellitesData();
  void ParseTLEBody(const std::string& body);
};
