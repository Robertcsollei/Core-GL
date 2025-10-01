#include <terrakit/core/config/ResourceManager.h>

namespace terrakit::core::config {

ResourceManager::ResourceManager(const std::string& assetsPath,
                               const std::string& texturesPath)
    : m_AssetsPath(assetsPath)
    , m_TexturesPath(texturesPath)
{
}

MeshHandle ResourceManager::GetMesh(const std::string& id) {
    auto mesh = m_Meshes.Get(id);
    if (mesh) {
        return MeshHandle(mesh); // Non-owning
    }
    return MeshHandle();
}

MeshHandle ResourceManager::CreateMesh(const std::string& id, MeshHandle mesh) {
    if (mesh.IsValid()) {
        auto addedMesh = m_Meshes.Add(id, std::make_unique<Mesh>(*mesh.get()));
        return MeshHandle(addedMesh); // Non-owning
    }
    return MeshHandle();
}

MaterialHandle ResourceManager::CreateMaterial(MaterialHandle material) {
    if (material.IsValid()) {
        auto addedMaterial = m_Materials.Add(std::make_unique<terrakit::renderer::Material>(*material.get()));
        return MaterialHandle(addedMaterial); // Non-owning
    }
    return MaterialHandle();
}

MaterialHandle ResourceManager::GetMaterial(const std::string& id) {
    auto material = m_Materials.Get(id);
    if (material) {
        return MaterialHandle(material); // Non-owning
    }
    return MaterialHandle();
}

TextureHandle ResourceManager::LoadTexture(const std::string& path) {
    auto texture = m_Textures.Load(m_TexturesPath + path);
    if (texture) {
        return TextureHandle(texture); // Already a shared_ptr
    }
    return TextureHandle();
}

TextureHandle ResourceManager::GetTexture(const std::string& id) {
    auto texture = m_Textures.Get(id);
    if (texture) {
        return TextureHandle(texture); // Non-owning
    }
    return TextureHandle();
}

} // namespace terrakit::core::config