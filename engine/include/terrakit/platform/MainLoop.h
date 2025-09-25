#pragma once

#include <functional>
#include <memory>

namespace terrakit::platform {

/**
 * @brief Abstract interface for platform-specific main loop handling
 *
 * Desktop platforms use a standard while loop,
 * Emscripten uses emscripten_set_main_loop for browser compatibility
 */
class MainLoop
{
public:
  using LoopFunction = std::function<bool()>; // Returns false to exit

  virtual ~MainLoop() = default;

  /**
   * @brief Start the main loop with the provided iteration function
   * @param loopFunc Function to call each frame, returns false to exit
   * @return Exit code
   */
  virtual int Run(LoopFunction loopFunc) = 0;

  /**
   * @brief Create platform-appropriate main loop instance
   * @return Unique pointer to main loop implementation
   */
  static std::unique_ptr<MainLoop> Create();
};

}