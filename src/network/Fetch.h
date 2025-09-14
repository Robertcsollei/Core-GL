#pragma once
#include <functional>
#include <string>

class Fetch
{
public:
  using Callback = std::function<void(const std::string& body)>;
  using ErrorCallback = std::function<void(const std::string& error)>;

  // Simple GET request
  static void Get(const std::string& url,
                  Callback onSuccess,
                  ErrorCallback onError = nullptr);
};
