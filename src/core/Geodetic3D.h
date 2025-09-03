#pragma once
#include <cstddef>

class Geodetic2D;

class Geodetic3D
{
public:
  Geodetic3D(double longitude, double latitude, double height);
  Geodetic3D(double longitude, double latitude);
  explicit Geodetic3D(const Geodetic2D& geodetic2D);
  Geodetic3D(const Geodetic2D& geodetic2D, double height);

  double Longitude() const { return m_Longitude; }
  double Latitude() const { return m_Latitude; }
  double Height() const { return m_Height; }

  bool operator==(const Geodetic3D& other) const;
  bool operator!=(const Geodetic3D& other) const { return !(*this == other); }

private:
  double m_Longitude;
  double m_Latitude;
  double m_Height;
};
