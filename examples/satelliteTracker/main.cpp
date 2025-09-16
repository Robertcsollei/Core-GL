#include "SatelliteTracker.h"

int
main(int, char**)
{
  SatelliteTracker app(860, 640);
  app.Init();

  return app.Run();
}
