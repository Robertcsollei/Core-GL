#include <cpr/cpr.h>
#include <memory>
#include <terrakit/platform/NetworkClient.h>
#include <thread>

namespace terrakit::platform {

class DesktopNetworkClient : public NetworkClient
{
public:
  void GetAsync(const std::string& url,
                SuccessCallback onSuccess,
                ErrorCallback onError) override
  {

    std::thread([url, onSuccess, onError]() {
      try {
        cpr::Response response = cpr::Get(cpr::Url{ url });

        if (response.status_code == 200) {
          if (onSuccess) {
            onSuccess(response.text);
          }
        } else {
          if (onError) {
            onError(response.status_code, response.error.message);
          }
        }
      } catch (const std::exception& e) {
        if (onError) {
          onError(-1, e.what());
        }
      }
    }).detach();
  }
};

std::unique_ptr<NetworkClient>
NetworkClient::Create()
{
  return std::make_unique<DesktopNetworkClient>();
}

}