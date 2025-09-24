#include <terrakit/network/Fetch.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/fetch.h>
#else
#include <cpr/cpr.h>
#include <thread>
#endif

using namespace terrakit::network;

#ifdef __EMSCRIPTEN__

struct FetchContext {
    Fetch::Callback onSuccess;
    Fetch::ErrorCallback onError;
};

void onFetchSuccess(emscripten_fetch_t* fetch) {
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

void onFetchError(emscripten_fetch_t* fetch) {
    if (fetch->userData) {
        auto* context = static_cast<FetchContext*>(fetch->userData);
        std::string error = std::string("HTTP ") + std::to_string(fetch->status);

        if (context->onError) {
            context->onError(error);
        }

        delete context;
    }
    emscripten_fetch_close(fetch);
}

void
Fetch::Get(const std::string& url, Callback onSuccess, ErrorCallback onError)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

    auto* context = new FetchContext{onSuccess, onError};
    attr.userData = context;
    attr.onsuccess = onFetchSuccess;
    attr.onerror = onFetchError;

    emscripten_fetch(&attr, url.c_str());
}

#else

void
Fetch::Get(const std::string& url, Callback onSuccess, ErrorCallback onError)
{
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
        onError(std::string("HTTP ") + std::to_string(response.status_code));
      }
    }
  }).detach();
}

#endif
