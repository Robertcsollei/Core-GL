#pragma once
#include <cstddef>

class Geodetic3D;

class Geodetic2D
{
public:
  Geodetic2D(double longitude, double latitude);
  explicit Geodetic2D(const Geodetic3D& geodetic3D);

  double Longitude() const { return m_Longitude; }
  double Latitude() const { return m_Latitude; }

  bool equalsEpsilon(const Geodetic2D& other, double epsilon) const;
  bool operator==(const Geodetic2D& other) const;
  bool operator!=(const Geodetic2D& other) const { return !(*this == other); }

private:
  double m_Longitude;
  double m_Latitude;
};
