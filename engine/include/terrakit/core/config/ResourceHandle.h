#pragma once
#include <memory>

namespace terrakit::renderer {
    class Material;
    class Shader;
    class Texture;
}

namespace terrakit::core {
    class Mesh;
}

namespace terrakit::core::config {

template<typename T>
class ResourceHandle {
public:
    ResourceHandle() = default;
    explicit ResourceHandle(std::shared_ptr<T> resource) : m_Resource(resource) {}

    T* get() const { return m_Resource.get(); }
    T& operator*() const { return *m_Resource; }
    T* operator->() const { return m_Resource.get(); }

    bool IsValid() const { return m_Resource != nullptr; }
    explicit operator bool() const { return IsValid(); }

    bool operator==(const ResourceHandle<T>& other) const { return m_Resource == other.m_Resource; }
    bool operator!=(const ResourceHandle<T>& other) const { return !(*this == other); }

private:
    std::shared_ptr<T> m_Resource;
};

using MeshHandle = ResourceHandle<terrakit::core::Mesh>;
using MaterialHandle = ResourceHandle<terrakit::renderer::Material>;
using TextureHandle = ResourceHandle<terrakit::renderer::Texture>;
using ShaderHandle = ResourceHandle<terrakit::renderer::Shader>;

} // namespace terrakit::core::config