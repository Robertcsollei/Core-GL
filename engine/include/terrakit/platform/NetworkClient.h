#pragma once

#include <functional>
#include <memory>
#include <string>

namespace terrakit::platform {

/**
 * @brief Platform-specific network client abstraction
 *
 * Desktop: Uses cpr library for HTTP requests
 * Emscripten: Uses emscripten fetch API
 */
class NetworkClient
{
public:
  using SuccessCallback = std::function<void(const std::string&)>;
  using ErrorCallback = std::function<void(int, const std::string&)>;

  virtual ~NetworkClient() = default;

  /**
   * @brief Perform an asynchronous GET request
   * @param url Target URL
   * @param onSuccess Callback for successful response
   * @param onError Callback for error response
   */
  virtual void GetAsync(const std::string& url,
                        SuccessCallback onSuccess,
                        ErrorCallback onError) = 0;

  /**
   * @brief Create platform-appropriate network client
   * @return Unique pointer to network client implementation
   */
  static std::unique_ptr<NetworkClient> Create();
};

}