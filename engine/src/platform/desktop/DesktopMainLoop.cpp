#include <memory>
#include <terrakit/platform/MainLoop.h>

namespace terrakit::platform {

class DesktopMainLoop : public MainLoop
{
public:
  int Run(LoopFunction loopFunc) override
  {
    while (true) {
      if (!loopFunc()) {
        break;
      }
    }
    return 0;
  }
};

std::unique_ptr<MainLoop>
MainLoop::Create()
{
  return std::make_unique<DesktopMainLoop>();
}

}