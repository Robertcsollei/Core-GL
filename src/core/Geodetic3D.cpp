#include <core/Geodetic2D.h>
#include <core/Geodetic3D.h>
#include <cstdint>
#include <functional> // std::hash

// ---- Constructors ----

Geodetic3D::Geodetic3D(double Longitude, double Latitude, double height)
  : m_Longitude(Longitude)
  , m_Latitude(Latitude)
  , m_Height(height)
{
}

Geodetic3D::Geodetic3D(double Longitude, double Latitude)
  : Geodetic3D(Longitude, Latitude, 0.0)
{
}

Geodetic3D::Geodetic3D(const Geodetic2D& geodetic2D)
  : Geodetic3D(geodetic2D.Longitude(), geodetic2D.Latitude(), 0.0)
{
}

Geodetic3D::Geodetic3D(const Geodetic2D& geodetic2D, double height)
  : Geodetic3D(geodetic2D.Longitude(), geodetic2D.Latitude(), height)
{
}

bool
Geodetic3D::operator==(const Geodetic3D& other) const
{
  return m_Longitude == other.m_Longitude && m_Latitude == other.m_Latitude &&
         m_Height == other.m_Height;
}
