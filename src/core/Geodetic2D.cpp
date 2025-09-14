#include <cmath>
#include <core/Geodetic2D.h>
#include <core/Geodetic3D.h>
#include <functional>

Geodetic2D::Geodetic2D(double longitude, double latitude)
  : m_Longitude(longitude)
  , m_Latitude(latitude)
{
}

Geodetic2D::Geodetic2D(const Geodetic3D& geodetic3D)
  : m_Longitude(geodetic3D.Longitude())
  , m_Latitude(geodetic3D.Latitude())
{
}

bool
Geodetic2D::equalsEpsilon(const Geodetic2D& other, double epsilon) const
{
  return (std::abs(m_Longitude - other.m_Longitude) <= epsilon) &&
         (std::abs(m_Latitude - other.m_Latitude) <= epsilon);
}

bool
Geodetic2D::operator==(const Geodetic2D& other) const
{
  return m_Longitude == other.m_Longitude && m_Latitude == other.m_Latitude;
}
