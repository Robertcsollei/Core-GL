#include <emscripten/fetch.h>
#include <memory>
#include <terrakit/platform/NetworkClient.h>

namespace terrakit::platform {

struct FetchContext
{
  NetworkClient::SuccessCallback onSuccess;
  NetworkClient::ErrorCallback onError;
};

void
onFetchSuccess(emscripten_fetch_t* fetch)
{
  if (fetch->userData) {
    auto* context = static_cast<FetchContext*>(fetch->userData);
    std::string response(fetch->data, fetch->numBytes);

    if (context->onSuccess) {
      context->onSuccess(response);
    }

    delete context;
  }
  emscripten_fetch_close(fetch);
}

void
onFetchError(emscripten_fetch_t* fetch)
{
  if (fetch->userData) {
    auto* context = static_cast<FetchContext*>(fetch->userData);

    if (context->onError) {
      context->onError(fetch->status, "Network request failed");
    }

    delete context;
  }
  emscripten_fetch_close(fetch);
}

class EmscriptenNetworkClient : public NetworkClient
{
public:
  void GetAsync(const std::string& url,
                SuccessCallback onSuccess,
                ErrorCallback onError) override
  {

    auto* context = new FetchContext{ onSuccess, onError };

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onFetchSuccess;
    attr.onerror = onFetchError;
    attr.userData = context;

    emscripten_fetch(&attr, url.c_str());
  }
};

std::unique_ptr<NetworkClient>
NetworkClient::Create()
{
  return std::make_unique<EmscriptenNetworkClient>();
}

}