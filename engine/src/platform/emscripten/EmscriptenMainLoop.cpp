#include <emscripten.h>
#include <memory>
#include <terrakit/platform/MainLoop.h>

namespace terrakit::platform {

class EmscriptenMainLoop : public MainLoop
{
private:
  static LoopFunction s_loopFunc;

  static void EmscriptenCallback()
  {
    if (s_loopFunc && !s_loopFunc()) {
      emscripten_cancel_main_loop();
    }
  }

public:
  int Run(LoopFunction loopFunc) override
  {
    s_loopFunc = loopFunc;
    emscripten_set_main_loop(EmscriptenCallback, 0, 1);
    return 0;
  }
};

// Static member definition
EmscriptenMainLoop::LoopFunction EmscriptenMainLoop::s_loopFunc = nullptr;

std::unique_ptr<MainLoop>
MainLoop::Create()
{
  return std::make_unique<EmscriptenMainLoop>();
}

}