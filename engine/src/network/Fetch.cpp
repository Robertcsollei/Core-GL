#include <terrakit/network/Fetch.h>
#include <terrakit/platform/NetworkClient.h>

using namespace terrakit::network;

void
Fetch::Get(const std::string& url, Callback onSuccess, ErrorCallback onError)
{
    auto client = terrakit::platform::NetworkClient::Create();

    // Convert the error callback signature from ErrorCallback(string) to ErrorCallback(int, string)
    auto errorWrapper = [onError](int statusCode, const std::string& message) {
        if (onError) {
            std::string error = (statusCode > 0) ?
                ("HTTP " + std::to_string(statusCode)) :
                message;
            onError(error);
        }
    };

    client->GetAsync(url, onSuccess, errorWrapper);
}
