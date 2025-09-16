#include <cpr/cpr.h>
#include <terrakit/network/Fetch.h>
#include <thread>

using namespace terrakit::network;

void
Fetch::Get(const std::string& url, Callback onSuccess, ErrorCallback onError)
{
  // Run async in a background thread so it doesn't block
  std::thread([url, onSuccess, onError]() {
    auto response = cpr::Get(cpr::Url{ url });

    if (response.error) {
      if (onError) {
        onError(response.error.message);
      }
      return;
    }

    if (response.status_code >= 200 && response.status_code < 300) {
      if (onSuccess) {
        onSuccess(response.text);
      }
    } else {
      if (onError) {
        onError("HTTP " + std::to_string(response.status_code));
      }
    }
  }).detach();
}
